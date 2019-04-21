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

#include "../utils/AvrUtils.h"
#include "../../Register.h"
#include "../../../../lib/type_traits.h"

namespace lib::avr {
        struct ATMega328 final
        {

            ATMega328() = delete;
            struct Port final
            {
                Port() = delete;
                DataRegister<Port, ReadOnly, std::byte> in;
                DataRegister<Port, ReadWrite, std::byte> ddr;
                DataRegister<Port, ReadWrite, std::byte> out;
                template<typename L> struct address;
            };

            struct Timer8Bit {
                static constexpr const uint8_t count = 2;
                typedef uint8_t value_type;
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


        } __attribute__((packed));

        template<>
        struct ATMega328::Port::address<B>
        {
            inline static constexpr uintptr_t value = 0x23;
        };
        template<>
        struct ATMega328::Port::address<C>
        {
            inline static constexpr uintptr_t value = 0x26;
        };
        template<>
        struct ATMega328::Port::address<D>
        {
            inline static constexpr uintptr_t value = 0x29;
        };

        //Timer0
        template<>
        struct ATMega328::Timer8Bit::address<0> {
            static constexpr uint8_t value = 0x44;
        };
    }