//
// Created by Sebastian on 23.04.2019.
//

#pragma once

#include <avr/io.h>
#include "../../abstraction/pins/PinControl.h"

enum class Sync {
    SYNCING,
    SYNCED,
    OFFSYNC
};

template<typename mcu>
class SoftwareUart {
private:
    static constexpr auto preamble = 0x55;
    static constexpr auto blockstart = 0xCC;
    static constexpr auto measurements = 2;

    static int16_t timer[2];
    static uint8_t measurement;
    static bool started;
    static bool waitForLow;
    static bool ignoreNextLow;

    static constexpr auto isHigh() {
        return pin::readPinState<pin::Pin<mcu, 0>>() == pin::State::ON;
    }
public:
    template<auto pinNumber>
    static constexpr void init() {

        pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
        pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();
    }

    static constexpr auto checkSlot() {
        auto high = isHigh();

        if(high && !started) {
            started = true;
            waitForLow = true;
        }

        if(!high && started && !ignoreNextLow) {
            waitForLow = false;
            timer[measurement]++;
        }

        if(!high && ignoreNextLow) {
            started = false;
            ignoreNextLow = false;
        }

        if(high && started && !waitForLow) {
            started = false;
            if(measurement + 1 < measurements) {
                measurement++;
            } else {
                waitForLow = false;
                measurement = 0;

                if(timer[0] - timer[1] < 0) {
                    ignoreNextLow = true;
                    return Sync::OFFSYNC;
                } else {
                    return Sync::SYNCED;
                }
            }
        }
        return Sync::SYNCING;
    }
};

template<typename mcu>
int16_t SoftwareUart<mcu>::timer[2] = {0, 0};

template<typename mcu>
uint8_t SoftwareUart<mcu>::measurement = 0;

template<typename mcu>
bool SoftwareUart<mcu>::started = false;

template<typename mcu>
bool SoftwareUart<mcu>::waitForLow = false;

template<typename mcu>
bool SoftwareUart<mcu>::ignoreNextLow = false;

