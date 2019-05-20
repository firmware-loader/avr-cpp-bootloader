//
// Created by Sebastian on 21.05.2019.
//

#pragma once

#include "SoftwareUart.h"

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
        while(true) {

            asm("clr %A0"
                "clr %B1"
                : "=r" (bitcellLength)
                : "r" (bitcellLength));
            while(isHigh()){}
            asm("psa01:"
                "adiw %A0, 5"
                : "=r" (bitcellLength));
        }
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


