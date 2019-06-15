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
    template<typename mcu>  requires mcu::family == MCUFamilies::AVR
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
        requires decltype(T()())::static_size() % 2 == 0 && decltype(T()())::static_size() <= 255
        static auto writeToFlash(T readMethod) {
            DDRB |= (1 << PB0);
            PORTB |= (1 << PB0);
            constexpr auto arraySize = decltype(T()())::static_size();
            uint8_t startAddress = readMethod()[0];
            uint8_t dbg_size = readMethod()[0];

            uint8_t sreg = SREG;
            cli();

            do {
                eeprom_busy_wait();

                boot_page_erase(startAddress);
                boot_spm_busy_wait();
                for (uint16_t i = 0; (i < SPM_PAGESIZE && dbg_size > 0); i += arraySize) {
                    auto v = readMethod();
                    for(uint8_t j = 0; j < arraySize && dbg_size > 0; j+= 2) {
                        boot_page_fill(startAddress + i + j, (v[j + 1] << 8) | v[j]);
                        dbg_size -= 2;
                    }
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


