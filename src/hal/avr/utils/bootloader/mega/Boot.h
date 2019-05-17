//
// Created by Sebastian on 17.05.2019.
//

#pragma once

namespace lib::avr::boot {
    <template typename mcu>
    struct BootloaderHal {
    private:
        static constexpr void (*start)() = 0x0000;
    public:
        static constexpr writeToFlash(mcu::mem_width data) {

        }
        static constexpr flushData() {

        }
    };
}


