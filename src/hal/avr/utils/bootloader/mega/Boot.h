//
// Created by Sebastian on 17.05.2019.
//

#pragma once

namespace lib::avr::boot {
    struct BootloaderHal {
    private:
        static constexpr void (*start)() = 0x0000;
    };
}


