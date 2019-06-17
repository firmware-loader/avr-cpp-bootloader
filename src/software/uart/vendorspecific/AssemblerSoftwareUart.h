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

        template<auto N> requires utils::is_arithmetic<decltype(N)>::value && N <= 2
        static auto getBytes() {
            using type = utils::byte_type<N>::value_type;
            type value = 0;
            waitForSync();

            for(typename mcu::mem_width i=0; i < N; i++) {
                value |= static_cast<type>(receiveData()) << (8u * i);
            }

            return value;
        }

        template<auto N> requires utils::is_arithmetic<decltype(N)>::value && N > 2 && N <= 255
        static utils::array<unsigned char, N>* getBytes() {
            static utils::array<unsigned char, N> value;
            waitForSync();
            for(typename mcu::mem_width i=0; i < N; i++) {
                value[i] = receiveData();;
            }
            return &value;
        }

        template<auto minBaud, auto maxBaud>
        static constexpr void init() {
            pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
            pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();

            //waitForSync();
        }
    };
}
