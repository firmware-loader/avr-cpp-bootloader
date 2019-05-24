//
// Created by Sebastian on 21.05.2019.
//

#pragma once

#include "SoftwareUart.h"
#include "external/Sync.h"

namespace lib::software {
    extern "C" {
        volatile uint8_t receiveBuffer;
    }
    template<typename mcu, auto pinNumber>
        requires mcu::family == MCUFamilies::AVR && pin::isAbstractPin<pin::Pin<mcu, pinNumber>>
    class SoftwareUart<mcu, pinNumber, SoftUartMethod::InlineAssembler> {
    private:
        static constexpr auto RXBIT = 0;
        //https://rn-wissen.de/wiki/index.php?title=Inline-Assembler_in_avr-gcc
        static auto receiveData() {
            asm volatile(R"(
                receiveByte:
                        sbic %[pin],%[bit]
                        rjmp receiveByte
                        ldi r21, lo8(8)
                CL9:
                        movw xl, r24
                        lsr xh
                        ror xl
                        ldi r21, 9
                rxb3:
                        rcall WaitBitcell
                        lsr r20
                        sbic %[pin],%[bit]
                        ori r20, 128
                        dec r21
                        brne rxb3
                        ret
                WaitBitcell:
                        movw xl, r24
                wbc0:
                        sbiw xl, 4
                        brcc wbc0
                wbcx:
                        sts %[rb], r20
                        ret
            )"
            : [rb] "=m" (receiveBuffer)
            : [pin] "I" (_SFR_IO_ADDR(PIND)), [bit] "n" (RXBIT)
            : "r20", "r21", "r24", "r25");
        }

        static auto waitForSync() {
            asm volatile(R"(
                rjmp waitForSyncASM
                skipHigh:
                        sbic %[pin],%[bit]
                        rjmp skipHigh
                skipLow:
                        sbis %[pin],%[bit]
                        rjmp skipLow
                waitForSyncASM:
                        clr r24
                        clr r25
                CL2:
                        sbic %[pin],%[bit]
                        rjmp CL2
                CL3:
                        adiw r24, 5
                        sbis %[pin],%[bit]
                        rjmp CL3
                CL4:
                        sbic %[pin],%[bit]
                        rjmp CL4
                CL5:
                        sbiw r24, 5
                        sbis %[pin],%[bit]
                        rjmp CL5
                        brmi skipHigh
                wop0:
                        rcall receiveByte
                        lds r20, %[rb]
                        cpi r20, lo8(85)
                        brne wop0
                ;        ret
            )"
            : [rb] "=m" (receiveBuffer)
            : [pin] "I" (_SFR_IO_ADDR(PIND)), [bit] "n" (RXBIT)
            : "r20", "r21", "r24", "r25");
        }
    public:
        static auto syncAndReceiveBytes(uint8_t* input, uint8_t elements) {
            waitForSync();
            for(uint8_t i=0; i < elements; i++) {
                receiveData();
                input[i] = receiveBuffer;
            }
            return input;
        }

        template<auto minBaud, auto maxBaud>
        static constexpr void init() {
            pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
            pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();
        }
    };
}
