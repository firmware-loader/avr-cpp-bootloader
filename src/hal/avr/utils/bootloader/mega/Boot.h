//
// Created by Sebastian on 17.05.2019.
//

#pragma once
#include <avr/boot.h>

namespace lib::avr::boot {

    template<typename mcu>
    struct BootloaderHal {
    private:
        static constexpr void (*start)() = 0x0000;
    public:
        static constexpr void writeToFlash(mcu::mem_width data) {

        }
        static constexpr void flushData() {

        }
    };
}


