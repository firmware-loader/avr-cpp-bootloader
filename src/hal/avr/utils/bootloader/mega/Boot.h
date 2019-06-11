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
        //requires is_same<decltype(T()()), uint16_t>::value_type
        static auto writeToFlash(uint32_t page, T readMethod) {
            static_assert(utils::is_same<decltype(T()()), uint16_t>());
            uint16_t dbg_size = readMethod();

            uint8_t sreg = SREG;
            cli();

            do {
                eeprom_busy_wait();

                boot_page_erase(page);
                boot_spm_busy_wait();
                for (uint16_t i = 0; (i < SPM_PAGESIZE && dbg_size > 0); i += 2) {
                    uint16_t w = readMethod();

                    boot_page_fill(page + i, w);
                    dbg_size -= 2;
                }

                boot_page_write(page);
                boot_spm_busy_wait();


                boot_rww_enable();
                page += SPM_PAGESIZE;
            } while(dbg_size > 0);

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


