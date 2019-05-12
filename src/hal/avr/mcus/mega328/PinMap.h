//
// Created by Sebastian on 12.05.2019.
//

#pragma once

#include "../../utils/AvrUtils.h"
#include "Mega328.h"
#include "../../pin/Control.h"

//Arduino Pinout
namespace lib::pins {
    using mcu = lib::avr::ATMega328;

    using PortB = lib::Hal::Port<lib::avr::B, mcu>;
    using PortC = lib::Hal::Port<lib::avr::C, mcu>;
    using PortD = lib::Hal::Port<lib::avr::D, mcu>;
//using PortE = lib::Hal::Port<lib::avr::E, mcu>;

    using PinB0 = lib::Hal::Pin<PortB, 0>;

    using PinC0 = lib::Hal::Pin<PortC, 0>;

    using PinD0 = lib::Hal::Pin<PortD, 0>;

//------- [ PINS ] ------

    template<typename mcu, auto N>
    struct Pin;

//------- [ D ] ------

    template<>
    struct Pin<mcu, 0> {
        using value = lib::Hal::Pin<PortD, 0>;
    };

    template<>
    struct Pin<mcu, 1> {
        using value = lib::Hal::Pin<PortD, 1>;
    };

    template<>
    struct Pin<mcu, 2> {
        using value = lib::Hal::Pin<PortD, 2>;
    };

    template<>
    struct Pin<mcu, 3> {
        using value = lib::Hal::Pin<PortD, 3>;
    };

    template<>
    struct Pin<mcu, 4> {
        using value = lib::Hal::Pin<PortD, 4>;
    };

    template<>
    struct Pin<mcu, 5> {
        using value = lib::Hal::Pin<PortD, 5>;
    };

    template<>
    struct Pin<mcu, 6> {
        using value = lib::Hal::Pin<PortD, 6>;
    };

    template<>
    struct Pin<mcu, 7> {
        using value = lib::Hal::Pin<PortD, 7>;
    };

//------- [ B ] ------

    template<>
    struct Pin<mcu, 8> {
        using value = lib::Hal::Pin<PortB, 0>;
    };

    template<>
    struct Pin<mcu, 9> {
        using value = lib::Hal::Pin<PortB, 1>;
    };

    template<>
    struct Pin<mcu, 10> {
        using value = lib::Hal::Pin<PortB, 2>;
    };

    template<>
    struct Pin<mcu, 11> {
        using value = lib::Hal::Pin<PortB, 3>;
    };

    template<>
    struct Pin<mcu, 12> {
        using value = lib::Hal::Pin<PortB, 4>;
    };

    template<>
    struct Pin<mcu, 20> {
        using value = lib::Hal::Pin<PortB, 6>;
    };

    template<>
    struct Pin<mcu, 21> {
        using value = lib::Hal::Pin<PortB, 7>;
    };

//------- [ C ] ------

    template<>
    struct Pin<mcu, 14> {
        using value = lib::Hal::Pin<PortC, 0>;
    };

    template<>
    struct Pin<mcu, 15> {
        using value = lib::Hal::Pin<PortC, 1>;
    };

    template<>
    struct Pin<mcu, 16> {
        using value = lib::Hal::Pin<PortC, 2>;
    };

    template<>
    struct Pin<mcu, 17> {
        using value = lib::Hal::Pin<PortC, 3>;
    };

    template<>
    struct Pin<mcu, 18> {
        using value = lib::Hal::Pin<PortC, 4>;
    };

    template<>
    struct Pin<mcu, 19> {
        using value = lib::Hal::Pin<PortC, 5>;
    };

    template<>
    struct Pin<mcu, 22> {
        using value = lib::Hal::Pin<PortC, 6>;
    };
}