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
        struct ATMega328 final
        {
            static constexpr MCUFamilies family = MCUFamilies::AVR;
            static constexpr auto frequency = F_CPU;
            using mem_width = uint8_t;
            ATMega328() = delete;
            struct Port final
            {
                Port() = delete;
                DataRegister<Port, ReadOnly, std::byte> in;
                DataRegister<Port, ReadWrite, std::byte> ddr;
                DataRegister<Port, ReadWrite, std::byte> out;
                template<typename L> struct address;
            };

            struct SPI
            {
                SPI() = delete;
                enum class spcr : mem_width {
                    SPIE0 = (1 << SPIE),
                    SPE0 = (1 << SPE),
                    DORD0 = (1 << DORD),
                    MSTR0 = (1 << MSTR),
                    CPOL0 = (1 << CPOL),
                    CPHA0 = (1 << CPHA),
                    SPR01 = (1 << SPR1),
                    SPR00 = (1 << SPR0)
                };

                enum class spsr : mem_width {
                    SPIF0  = (1 << SPIF),
                    WCOL0  = (1 << WCOL),
                    SPI2X0 = (1 << SPI2X)
                };

                enum class Pins : mem_width {
                    SS = 2,
                    Mosi = 3,
                    Miso = 4,
                    SCK = 5
                };

                template<uint8_t number>
                struct SPI_Port;

                ControlRegister<SPI, spcr, mem_width> Spcr;
                ControlRegister<SPI, spsr, mem_width> Spsr;
                DataRegister<SPI, ReadWrite, mem_width> Spdr;
                template<mem_width N> struct address;
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

            struct ADConverter {
                static constexpr const mem_width count = 1;
                using value_type = mem_width ;

                DataRegister<ADConverter, ReadOnly, mem_width> adcl;
                DataRegister<ADConverter, ReadOnly, mem_width> adch;

                enum class ADCsra : mem_width {
                    aden = (1 << ADEN),
                    adsc = (1 << ADSC),
                    adfr = (1 << ADATE),
                    adif = (1 << ADIF),
                    adie = (1 << ADIE),
                    adps2 = (1 << ADPS2),
                    adps1 = (1 << ADPS1),
                    adps0 = (1 << ADPS0),
                };

                ControlRegister<ADConverter, ADCsra> adcsra;

                enum class ADCsrb : mem_width {
                    acme = (1 << ACME),
                    adts2 = (1 << ADTS2),
                    adts1 = (1 << ADTS1),
                    adts0 = (1 << ADTS0),
                };

                ControlRegister<ADConverter, ADCsrb> adcsrb;

                enum class ADMux : mem_width {
                    refs1 = (1 << REFS1),
                    refs0 = (1 << REFS0),
                    adlar = (1 << ADLAR),
                    mux3 = (1 << MUX3),
                    mux2 = (1 << MUX2),
                    mux1 = (1 << MUX1),
                    mux0 = (1 << MUX0),
                };
                ControlRegister<ADConverter, ADMux> admux;

                DataRegister<ADConverter, UnUsed, mem_width> reserved;

                enum class DigitalInputDisable : mem_width {
                    adc7d = (1 << 7),
                    adc6d = (1 << 6),
                    adc5d = (1 << 5),
                    adc4d = (1 << 4),
                    adc3d = (1 << 3),
                    adc2d = (1 << 2),
                    adc1d = (1 << 1),
                    adc0d = (1 << 0),
                };

                ControlRegister<ADConverter, ADMux> didr0;

                template<int N> struct address;
            };

            struct Uart {
                enum class UCSRA : mem_width {
                    mpcmo = (1 << MPCM0),
                    u2x   = (1 << U2X0),
                    UPE   = (1 << UPE0),
                    DOR   = (1 << DOR0),
                    FE    = (1 << FE0),
                    udre  = (1 << UDRE0),
                    txc   = (1 << TXC0),
                    rxc   = (1 << RXC0),
                };

                ControlRegister<Uart, UCSRA> ucsra;

                enum class UCSRB : mem_width {
                    txb8  = (1 << TXB80),
                    rxb8  = (1 << RXB80),
                    ucsz  = (1 << UCSZ02),
                    txen  = (1 << TXEN0),
                    rxen  = (1 << RXEN0),
                    udrie = (1 << UDRIE0),
                    txcie = (1 << TXCIE0),
                    rxcie = (1 << RXCIE0),
                };

                ControlRegister<Uart, UCSRB> ucsrb;

                enum class UCSRC : mem_width {
                    ucpol  = (1 << UCPOL0),
                    ucsz0  = (1 << UCSZ00),
                    ucsz1  = (1 << UCSZ01),
                    ucsz2  = (1 << UCSZ02),
                    usbs    = (1 << USBS0),
                    upm0   = (1 << UPM00),
                    upm1   = (1 << UPM01),
                    umsel0  = (1 << UMSEL00),
                    umsel1  = (1 << UMSEL01),
                };

                ControlRegister<Uart, UCSRC> ucsrc;

                DataRegister<Uart, UnUsed, mem_width> reserved;

                DataRegister<ADConverter, ReadWrite, mem_width> ubrrl;
                DataRegister<ADConverter, ReadWrite, mem_width> ubrrh;
                DataRegister<ADConverter, ReadWrite, mem_width> udr;

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

        template<>
        struct ATMega328::Timer8Bit::address<0> {
            static constexpr uint8_t value = 0x44;
        };

        template<>
        struct ATMega328::Timer16Bit::address<0> {
            static constexpr uint8_t value = 0x80;
        };

        template<>
        struct ATMega328::Timer16Bit::address<1> {
            static constexpr uint8_t value = 0x90;
        };

        template<>
        struct ATMega328::ADConverter::address<0> {
            static constexpr uintptr_t value = 0x78;
        };

        template<>
        struct ATMega328::Uart::address<0> {
            static constexpr uintptr_t value = 0xC0;
        };
    }