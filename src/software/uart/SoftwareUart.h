//
// Created by Sebastian on 23.04.2019.
//

#pragma once

#include <avr/io.h>
#include "../../abstraction/pins/PinControl.h"
#include "../../abstraction/uart/AbstractTimer.h"
#include "../../utils/custom_limits.h"
#include "../../utils/TypeTraits.h"
#include "../../concepts/TypeCheck.h"

enum class Sync : uint8_t {
    Synced,
    Syncing
};

template<typename mcu>
class SoftwareUart {
private:
    using timer = lib::software::AbstractTimer<mcu>;
    static constexpr auto blockstart = 0xCC;

    static Sync sync;

    static constexpr auto isHigh() {
        //return pin::readPinState<pin::Pin<mcu, 0>>() == pin::State::ON;
        using pin = pin::Pin<mcu, 0>::value;
        return (pin::get() != 0);
    }

public:
    static int16_t bitcellLength;
    static constexpr auto preamble = 0x55;
    template<auto pinNumber>
    static constexpr void init() {
        using namespace lib::software::literals;

        timer::template init<250000_khz>();
        pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
        pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();
        //waitForSync();
    }

    template<typename T> requires concepts::UnsignedType<T>
    static constexpr auto calculateTime(T startValue, T endValue) {
        return (startValue > endValue) ?
        (utils::numeric_limits<decltype(startValue)>::max() - startValue) + endValue
                                : endValue - startValue;
    }

    static auto waitForSync() {
        while(true) {
            while(isHigh()){}           //skip first high

            auto startValue = timer::readValue();
            while (!isHigh()) {         //measure first low time
            }
            auto endValue = timer::readValue();

            //overflow protection
            bitcellLength = calculateTime(startValue, endValue);

            while (isHigh()) {}         //wait for 2nd low

            startValue = timer::readValue();
            while (!isHigh()) {
            }
            endValue = timer::readValue();
            bitcellLength -= calculateTime(startValue, endValue);


            if(bitcellLength < 0) {
                while(isHigh()){}
                while(!isHigh()){}      //repeat until in sync
            } else {
                break;                  //sync
            }
        }
        //return bitcellLength;    //where do these 10 instructions come from?
    }

    static auto receiveData() {
        uint8_t buffer = 0;
        uint8_t i = 0;                      // this is mandatory
        while(isHigh()){}                   // skip everything before start (this will keep the sync)
        for(; i < 9; i++) {                  // 8-N-1 (will overwrite start bit)
            auto startValue = timer::readValue();
            //for(uint8_t j=0; j < 10; j++) { asm(""); }
            _delay_us(10);                  // todo: replace this
            buffer /= 2;                    // lshift
            if(isHigh()) {
                buffer |= (1u << 7);
            }
            while(calculateTime(startValue, timer::readValue()) < bitcellLength ) {  }
        }
        while(!isHigh()){}                  // skip last low (stop bit)
        return buffer;
    }
};

template<typename mcu>
int16_t SoftwareUart<mcu>::bitcellLength = 0;


template<typename mcu>
Sync SoftwareUart<mcu>::sync = Sync::Syncing;