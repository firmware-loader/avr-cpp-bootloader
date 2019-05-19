//
// Created by Sebastian on 18.05.2019.
//

#pragma once


#include "../../../MCUFamilies.h"
#include "../../../Register.h"

namespace lib {
    namespace msp {
        using namespace lib::mcu;

        struct MSP432P401 final {
            static constexpr MCUFamilies family = MCUFamilies::MSP;
            using mem_width = uint32_t;

            MSP432P401() = delete;

            struct Port final {
                Port() = delete;

                DataRegister <Port, ReadOnly, std::byte> in;
                DataRegister <Port, ReadWrite, std::byte> ddr;
                DataRegister <Port, ReadWrite, std::byte> out;
                template<typename L>
                struct address;
            };
        };
    }
}


