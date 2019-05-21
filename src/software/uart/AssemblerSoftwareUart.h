//
// Created by Sebastian on 21.05.2019.
//

#pragma once

#include "SoftwareUart.h"

namespace lib::software {
    template<typename mcu>
    class SoftwareUart<mcu, SoftUartMethod::Assembler> {
    private:
        static int16_t bitcellLength;

        static constexpr auto isHigh() {
            //return pin::readPinState<pin::Pin<mcu, 0>>() == pin::State::ON;
            using pin = pin::Pin<mcu, 0>::value;
            return (pin::get() != 0);
        }

    public:
        static auto receiveData();

        static auto waitForSync() {
            //uint8_t *bh asm ("r24");
            //uint8_t *bl asm ("r25");
            int16_t data;

            //while (!isHigh()) {}
            asm("sync:");
            asm("clr %A0"
            : "=r" (data));
            asm("clr %B0"
            : "=r" (data));
            asm("psa01:");
            asm("adiw %A0, 5"
            : "=r" (data) );
            asm("sbis %0, 0"
            : "=r" (PINB));
            asm("rjmp psa01");
            asm("psa02:");
            asm("sbiw %A0, 5"
            : "=r" (data) );
            asm("sbis %0, 0"
            : "=r" (PINB));
            asm("rjmp psa02");
            asm("brmi sync");
            return data;
        }

        template<auto pinNumber, auto minBaud, auto maxBaud>
        static constexpr void init() {
            pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
            pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();

            waitForSync();
        }
    };


    template<typename mcu>
    int16_t SoftwareUart<mcu, SoftUartMethod::Assembler>::bitcellLength = 0;

}
