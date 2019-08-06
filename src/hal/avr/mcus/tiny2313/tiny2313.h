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

# include <avr/io.h>

#include "../../utils/AvrUtils.h"
#include "../../Register.h"
#include "../../../../lib/type_traits.h"
#include "../../../MCUFamilies.h"

namespace lib::avr {
        struct ATTiny2313 final
        {
            static constexpr MCUFamilies family = MCUFamilies::AVR;
            static constexpr auto frequency = F_CPU;
            using mem_width = uint8_t;
            ATTiny2313() = delete;
            struct Port final
            {
                Port() = delete;
                DataRegister<Port, ReadOnly, std::byte> in;
                DataRegister<Port, ReadWrite, std::byte> ddr;
                DataRegister<Port, ReadWrite, std::byte> out;
                template<typename L> struct address;
            };

            struct Timer8Bit {
                static constexpr uint8_t count = 2;
                using value_type = uint8_t;
                enum class TCCRA : uint8_t {
                    coma0 = (1 << COM0A0),
                    coma1 = (1 << COM0A1),
                    comb0 = (1 << COM0B0),
                    comb1 = (1 << COM0B1),
                    wgm0 = (1 << WGM00),
                    wgm1 = (1 << WGM01)
                };
                ControlRegister<Timer8Bit, TCCRA> tccra;
                enum class TCCRB : uint8_t {
                    foca = (1 << FOC0A),
                    focb = (1 << FOC0B),
                    wgm2 = (1 << WGM02),
                    cs2 = (1 << CS02),
                    cs1 = (1 << CS01),
                    cs0 = (1 << CS00),
                };

                ControlRegister<Timer8Bit, TCCRB> tccrb;
                DataRegister<Timer8Bit, ReadWrite> tcnt;
                DataRegister<Timer8Bit, ReadWrite> ocra;
                DataRegister<Timer8Bit, ReadWrite> ocrb;
                template<int N> struct address;
            };

            struct Timer16Bit {
                static constexpr uint8_t count = 2;

                using value_type = uint8_t;
                enum class TCCRA : uint8_t {
                    coma0 = (1 << COM1A0),
                    coma1 = (1 << COM1A1),
                    comb0 = (1 << COM1B0),
                    comb1 = (1 << COM1B1),
                    wgm0 = (1 << WGM10),
                    wgm1 = (1 << WGM11)
                };
                ControlRegister<Timer16Bit, TCCRA> tccra;

                enum class TCCRB : uint8_t {
                    icnc = (1 << FOC0A),
                    ices = (1 << FOC0B),
                    wgm2 = (1 << WGM12),
                    wgm3 = (1 << WGM13),
                    cs2 = (1 << CS12),
                    cs1 = (1 << CS11),
                    cs0 = (1 << CS10),
                };
                ControlRegister<Timer16Bit, TCCRB> tccrb;

                enum class TCCRC : uint8_t {
                    foca = (1 << FOC1A),
                    focb = (1 << FOC1B),
                };

                ControlRegister<Timer16Bit, TCCRC> tccrc;

                DataRegister<Timer16Bit, UnUsed, mem_width> reserved;

                DataRegister<Timer16Bit, ReadWrite> tcntl;
                DataRegister<Timer16Bit, ReadWrite> tcnth;

                DataRegister<Timer16Bit, ReadOnly> icrl;
                DataRegister<Timer16Bit, ReadOnly> icrh;

                DataRegister<Timer16Bit, ReadWrite> orcal;
                DataRegister<Timer16Bit, ReadWrite> ocrah;

                DataRegister<Timer16Bit, ReadWrite> orcbl;
                DataRegister<Timer16Bit, ReadWrite> ocrbh;

                template<int N> struct address;
            };

        } __attribute__((packed));

        template<>
        struct ATTiny2313::Port::address<A>
        {
            inline static constexpr uintptr_t value = 0x1B;
        };

        template<>
        struct ATTiny2313::Port::address<B>
        {
            inline static constexpr uintptr_t value = 0x18;
        };

        template<>
        struct ATTiny2313::Port::address<D>
        {
            inline static constexpr uintptr_t value = 0x30;
        };

        template<>
        struct ATTiny2313::Timer8Bit::address<0> {
            static constexpr uint8_t value = 0x2D;
        };

        template<>
        struct ATTiny2313::Timer16Bit::address<0> {
            static constexpr uint8_t value = 0x80;
        };

        template<>
        struct ATTiny2313::Timer16Bit::address<1> {
            static constexpr uint8_t value = 0x90;
        };
    }