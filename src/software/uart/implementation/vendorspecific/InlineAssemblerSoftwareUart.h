//
// Created by Sebastian on 21.05.2019.
//

#pragma once

#include "../SoftwareUart.h"
#include "external/Sync.h"
#include "../../../../utils/Array.h"

namespace lib::software {
    extern "C" {
    volatile uint8_t receiveBuffer;
    volatile uint16_t counterBuffer = 0;
    }
    template<typename mcu, auto pinNumber>
    requires mcu::family == MCUFamilies::AVR && pin::isAbstractPin<pin::Pin<mcu, pinNumber>>
    class SoftwareUart<mcu, pinNumber, SoftUartMethod::InlineAssembler> {
    private:
        static constexpr auto RXBIT = 0;
        using selectedPin = pin::Pin<mcu, pinNumber>::value;
        //https://rn-wissen.de/wiki/index.php?title=Inline-Assembler_in_avr-gcc
        //We can't have a return value here, as it'll manipulate the timings
        static void asmReceiveData() {
            asm volatile(R"(
                rjmp receiveByte
                WaitBitcell:
                        movw xl, %A[cb]
                wbc0:
                        sbiw xl, 4
                        brcc wbc0
                wbcx:
                        sts %[rb], r20
                        ret
                receiveByte:
                        sbic %[pin],%[bit]
                        rjmp receiveByte
                        ldi r21, lo8(8)
                CL9:
                        movw xl, %A[cb]
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
            )"
            : [rb] "=m" (receiveBuffer)
            : [pin] "I" (_SFR_IO_ADDR(PIND)), [bit] "n" (RXBIT), [cb] "w" (counterBuffer)
            : "r20", "r21");
        }




    public:
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
                        clr %A[cb]
                        clr %B[cb]
                CL2:
                        sbic %[pin],%[bit]
                        rjmp CL2
                CL3:
                        adiw %A[cb], 5
                        sbis %[pin],%[bit]
                        rjmp CL3
                CL4:
                        sbic %[pin],%[bit]
                        rjmp CL4
                CL5:
                        sbiw %A[cb], 5
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
            : [rb] "=m" (receiveBuffer), [cb] "=w" (counterBuffer)
            : [pin] "I" (_SFR_IO_ADDR(PIND)), [bit] "n" (RXBIT)
            : "r20", "r21");
        }

        [[nodiscard]] static auto receiveData() {
            asmReceiveData();
            return receiveBuffer;
        }

        template<auto minBaud, auto maxBaud>
        static constexpr void init() {
            pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
            pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();
        }
    };
}
