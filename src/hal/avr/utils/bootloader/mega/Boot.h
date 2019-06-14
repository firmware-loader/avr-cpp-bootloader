//
// Created by Sebastian on 17.05.2019.
//

#pragma once

#include <stdint-gcc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/boot.h>
#include "../../../../../utils/TypeTraits.h"

namespace lib::avr::boot {
    template<typename mcu>
    struct BootloaderHal {
    private:
        static constexpr void (*start)() = 0x0000;
    public:
        template<typename T>
        requires static_cast<bool>(utils::is_same<decltype(T()()), uint16_t>()) // is this a bug in gcc?
        static auto writeToFlash(T readMethod) {
            DDRB |= (1 << PB0);
            PORTB |= (1 << PB0);
            //static_assert(utils::is_same<decltype(T()()), uint16_t>());
            uint16_t startAddress = readMethod();
            uint16_t dbg_size = readMethod();

            uint8_t sreg = SREG;
            cli();

            do {
                eeprom_busy_wait();

                boot_page_erase(startAddress);
                boot_spm_busy_wait();
                for (uint16_t i = 0; (i < SPM_PAGESIZE && dbg_size > 0); i += 2) {
                    uint16_t w = readMethod();

                    boot_page_fill(startAddress + i, w);
                    dbg_size -= 2;
                }

                boot_page_write(startAddress);
                boot_spm_busy_wait();


                boot_rww_enable();
                startAddress += SPM_PAGESIZE;
            } while(dbg_size > 0);
            PORTB &= ~(1 << PB0);

            SREG = sreg;
            startUserProgram();
        }

        template<typename T>
        requires utils::bit_count<decltype(T()())>::value > 16 && utils::bit_count<decltype(T()())>::value <= 64
        static auto writeToFlash(T readMethod) {
            using bitCounter = utils::bit_count<decltype(T()())>;
            constexpr auto byteCount = bitCounter::value / 8;
            DDRB |= (1 << PB0);
            PORTB |= (1 << PB0);
            auto startAddress = readMethod();
            auto dbg_size = readMethod();

            uint8_t sreg = SREG;
            cli();

            do {
                eeprom_busy_wait();

                boot_page_erase(startAddress);
                boot_spm_busy_wait();
                for (uint16_t i = 0; (i < SPM_PAGESIZE && dbg_size > 0); i += byteCount) {
                    auto w = readMethod();
                    for(uint8_t j = 0; j < byteCount; j+= 2) {
                        boot_page_fill(startAddress + i + j, (w >> (j*8)));
                    }
                    dbg_size -= byteCount;
                }

                boot_page_write(startAddress);
                boot_spm_busy_wait();


                boot_rww_enable();
                startAddress += SPM_PAGESIZE;
            } while(dbg_size > 0);
            PORTB &= ~(1 << PB0);

            SREG = sreg;
            startUserProgram();
        }

        static auto flushData() {

        }

        static constexpr auto startUserProgram() {
            start();
        }
    };
}


