//
// Created by Sebastian on 21.05.2019.
//

#pragma once

#include "../SoftwareUart.h"
#include "external/Sync.h"

namespace lib::software {
    template<typename mcu, auto pinNumber>
        requires mcu::family == MCUFamilies::AVR && pin::isAbstractPin<pin::Pin<mcu, pinNumber>>
    class SoftwareUart<mcu, pinNumber, SoftUartMethod::Assembler> {
    private:
    public:
        static inline auto receiveData() {
            return detail::getByte();
        }

        static auto waitForSync() {
            detail::sync();
        }

        template<auto minBaud, auto maxBaud>
        static constexpr void init() {
            pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
            pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();

            //waitForSync();
        }
    };
}
