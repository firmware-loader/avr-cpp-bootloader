//
// Created by Sebastian on 21.05.2019.
//

#pragma once

#include "SoftwareUart.h"
#include "external/Sync.h"

namespace lib::software {
    template<typename mcu>
    class SoftwareUart<mcu, SoftUartMethod::Assembler> {
    private:
        static constexpr auto preamble = 0x55;
        static auto receiveData() {
            return detail::getByte();
        }

        static auto waitForSync() {
            detail::sync();
        }
    public:
        static auto syncAndRecieveBytes(unsigned char* input, uint8_t elements) {
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
