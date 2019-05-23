//
// Created by Sebastian on 21.05.2019.
//

#pragma once

#include "SoftwareUart.h"

namespace lib::software {
    template<typename mcu>
    requires mcu::family == MCUFamilies::AVR
    class SoftwareUart<mcu, SoftUartMethod::Assembler> {
    private:
        static constexpr auto preamble = 0x55;
        static constexpr auto RXBIT = 1;
        static constexpr auto RXPIN = 0x05;
        static auto receiveData() {
            volatile uint8_t receiveBuffer;
            asm volatile(R"(
                receiveByte:
                        sbic %1,%2
                        rjmp receiveByte
                        ldi r23, lo8(8)
                .CL9:
                        movw xl, r24
                        lsr xh
                        ror xl
                        ldi r23, 9
                rxb3:
                        rcall WaitBitcell
                        lsr r22
                        sbic %1, %2
                        ori r22, 128
                        dec r23
                        brne rxb3
                        ret
                WaitBitcell:
                        movw xl, r24
                wbc0:
                        sbiw xl, 4
                        brcc wbc0
                wbcx:
                        sts %0,r22
                        ret
            )"
            : "=m" (receiveBuffer)
            : "n" (RXPIN), "n" (RXBIT));
            return receiveBuffer;
        }

        static auto waitForSync() {
            asm volatile(R"(
                rjmp waitForSyncASM
                .skipHigh:
                    sbic %0,%1
                    rjmp .skipHigh
                .skipLow:
                        sbis %0,%1
                        rjmp .skipLow
                waitForSyncASM:
                        clr r25
                        clr r24
                .CL2:
                        sbic %0,%1
                        rjmp .CL2
                .CL3:
                        adiw r24,5
                        sbis %0,%1
                        rjmp .CL3
                .CL4:
                        sbic %0,%1
                        rjmp .CL4
                .CL5:
                        sbiw r24,5
                        sbis %0,%1
                        rjmp .CL5
                        brmi .skipHigh
                        ret
            )"
            :
            : "n" (RXPIN), "n" (RXBIT));
        }
    public:
        static auto syncAndReceiveBytes(unsigned char *input, uint8_t elements) {
            waitForSync();
            while(receiveData() != preamble) {}
            for(uint8_t i=0; i < elements; i++) {
                input[i] = receiveData();
            }
            return input;
        }

        template<auto pinNumber, auto minBaud, auto maxBaud>
        static constexpr void init() {
            pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
            pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();

            //waitForSync();
        }
    };
}
