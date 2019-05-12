//
// Created by Sebastian on 23.04.2019.
//

#pragma once

template<typename MicroController>
class SoftwareUart {
public:
    constexpr auto preamble = 0x55;
    constexpr auto blockstart = 0xCC;
};



