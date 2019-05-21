/*
 * ++C - C++ introduction
 * Copyright (C) 2013, 2014, 2015, 2016, 2017 Wilhelm Meier <wilhelm.meier@hs-kl.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <avr/io.h>
#include "../mcus/mega328/Mega328.h"
#include <stdint.h>


namespace lib::Hal {
        template<typename PortName, typename MicroController>
        class Port {
            static inline constexpr auto port = avr::getAddress<typename MicroController::Port, PortName>;
        public:
            Port() = delete;
            typedef PortName portname_type;

            static volatile uint8_t& ddr() {
                return *port()->ddr;
            }
            static volatile uint8_t& get() {
                return *port()->out;
            }
            static uint8_t read() {
                return *port()->in;
            }
        };

        template<typename Port, uint8_t Number>
        class Pin {
            static_assert(Number < 8);
        public:
            Pin() = delete;
            struct Output {};
            struct Input {};
            static inline constexpr uint8_t mask = 1 << Number;
            static inline constexpr uint8_t number = Number;
            typedef Port port_type;

            template<typename Dir>
            static void dir() {
                if constexpr(std::is_same<Dir, Output>::value) {
                    Port::ddr() |= mask;
                }
                else if constexpr(std::is_same<Dir, Input>::value) {
                    Port::ddr() &= static_cast<uint8_t>(~mask);
                }
                else {
                    static_assert(sizeof(Port) == 0, "Only Input or Output allowed");
                }
            }
            static void on() {
                Port::get() |= mask;
            }

            static void flip() {
                Port::get() ^= mask;
            }
            static void off() {
                Port::get() &= static_cast<uint8_t>(~mask);
            }
            static auto get() {
                return Port::read() & (mask);
            }
        };


}



