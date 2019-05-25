//
// Created by Sebastian on 21.05.2019.
//

#pragma once

#include "SoftwareUart.h"
#include "external/Sync.h"

namespace lib::software {
    template<typename mcu, auto pinNumber>
        requires mcu::family == MCUFamilies::AVR && pin::isAbstractPin<pin::Pin<mcu, pinNumber>>
    class SoftwareUart<mcu, pinNumber, SoftUartMethod::Assembler> {
    private:
        static constexpr auto preamble = 0x55;
        static inline auto receiveData() {
            return detail::getByte();
        }

        static auto waitForSync() {
            detail::sync();
        }
    public:
        static auto syncAndReceiveBytes(uint8_t* input, uint8_t elements) {
            waitForSync();
            for(uint8_t i=0; i < elements; i++) {
                input[i] = receiveData();
            }
        }

        static auto getWord() {
            uint16_t word = 0;
            waitForSync();

            word = receiveData();
            word |= (uint16_t)receiveData() << 8u;

            return word;
        }

        template<auto minBaud, auto maxBaud>
        static constexpr void init() {
            pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
            pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();

            //waitForSync();
        }
    };
}
