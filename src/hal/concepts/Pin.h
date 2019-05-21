//
// Created by Sebastian on 21.05.2019.
//

#pragma once

namespace pin {
    //------- [ PINS ] ------
    template<typename mcu, auto N>
    struct Pin;

    template<typename T>
    concept bool isPin() {
        return requires(T t) {
            typename T::port_type;
            T::on();
            T::off();
            T::flip();
            T::get();
            T::number;
        };
    }

    template <typename T>
    concept isAbstractPin = isPin<typename T::value>();
}


