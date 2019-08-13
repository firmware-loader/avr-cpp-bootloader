//
// Created by Sebastian on 05.08.2019.
//

#pragma once
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>

namespace lib::avr::boot {
    enum class ResetReason {
        NOT_EXT_RESET,
        EXT_RESET,
        OTHER
    };

    template<typename mcu>  requires mcu::family == MCUFamilies::AVR
    class UPDIBootloader {
    public:
        static ResetReason resetReason() {
            uint8_t mcusr = MCUSR;
            MCUSR = 0;

            if(mcusr & (1<<PORF) || mcusr & (1 << WDRF) || mcusr & (1 << BORF)) {
                return ResetReason::NOT_EXT_RESET;
            } else if(mcusr & (1 << EXTRF)) {
                return ResetReason::EXT_RESET;
            } else {
                return ResetReason::OTHER;
            }
        }

        static uint8_t readFlash(uint16_t addr) {
            return pgm_read_byte(addr);
        }

        static bool flashBusy() {
            return boot_spm_busy();
        }

        static void writeFlashPage(uint16_t page) {
            boot_page_write(page);
        }

        static void clearFlashPage(uint16_t page) {
            boot_page_erase(page);
            boot_spm_busy_wait();
        }

        static void fillFlashPage(uint16_t addr, uint16_t content) {
            boot_page_fill(addr, content);
        }

        static void enableFlash() {
            boot_rww_enable();
        }
    };
}



