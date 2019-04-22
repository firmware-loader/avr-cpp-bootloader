//
// Created by Sebastian on 21.04.2019.
//

#pragma once
#include <avr/io.h>

namespace lib::avr {
    struct A {};
    struct B {};
    struct C {};
    struct D {};
    struct E {};

    template<typename Component, uint8_t N>
    constexpr Component* getAddress()
    {
        return reinterpret_cast<Component*>(Component::template address<N>::value);
    }
    template<typename Component, typename Letter>
    constexpr Component* getAddress()
    {
        return reinterpret_cast<Component*>(Component::template address<Letter>::value);
    }
    template<typename Component>
    constexpr Component* getAddress()
    {
        return reinterpret_cast<Component*>(Component::address);
    }
}


