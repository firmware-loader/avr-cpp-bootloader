//
// Created by Sebastian on 12.05.2019.
//

#include "../../hal/avr/mcus/mega328/PinMap.h"
#pragma once

namespace pin {
    enum class Direction {
        INPUT,
        OUTPUT
    };

    enum class State {
        ON,
        OFF,
        FLIP
    };

    template<typename Pin, Direction direction>
    static constexpr auto setDirection() {
        using pin = Pin::value;
        if constexpr (direction == Direction::INPUT) {
            pin::template dir<typename pin::Input>();
        } else if (direction == Direction::OUTPUT) {
            pin::template dir<typename pin::Output>();
        }
    }

    template<typename Pin, State direction>
    static constexpr auto setPinState() {
        using pin = Pin::value;
        if constexpr (direction == State::ON) {
            pin::on();
        } else if (direction == State::OFF) {
            pin::off();
        } else if (direction == State::FLIP) {
            pin::flip();
        }
    }
}



