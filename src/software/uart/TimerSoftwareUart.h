//
// Created by Sebastian on 21.05.2019.
//

#pragma once

#include "SoftwareUart.h"

template<typename mcu>
class SoftwareUart<mcu, SoftUartMethod::Timer> {
private:
    static int16_t bitcellLength;
    using timer = lib::software::AbstractTimer<mcu>;
    static constexpr auto blockstart = 0xCC;
    static constexpr auto isHigh() {
        //return pin::readPinState<pin::Pin<mcu, 0>>() == pin::State::ON;
        using pin = pin::Pin<mcu, 0>::value;
        return (pin::get() != 0);
    }

    template<typename T> requires concepts::UnsignedType<T>
    static constexpr T calculateTime(T startValue, T endValue) {
        if constexpr(timer::timerBitCount() == sizeof(T) * 8) {
            // value needs to be unsigned to also work for wrapped around differences
            return endValue - startValue;
        } else {
            return  (endValue - startValue) & (utils::getMaxValueOfBitcount<timer::timerBitCount()>());
        }
    }

public:
    static constexpr auto preamble = 0x55;

    template<auto pinNumber, auto minBaud, auto maxBaud>
    static constexpr void init() {
        using namespace lib::software::literals;
        constexpr auto timerClockSpeed = 250000_khz;

        timer::template init<timerClockSpeed>();
        constexpr auto realTimerValue = timer::template getRealClockValue<timerClockSpeed>();
        constexpr long double timerOffset = (timerClockSpeed / (long double) realTimerValue);

        static_assert(timerOffset <= 1.1 && timerOffset >= 0.9, "Timer Offset not within acceptable error margin!");
        static_assert(realTimerValue / minBaud <= utils::getMaxValueOfBitcount<timer::timerBitCount()>(),
                      "Timer will overflow at minimum baud rate!");
        static_assert(realTimerValue / minBaud > 0, "Timer too slow for maximum baud rate!");
        static_assert((realTimerValue / (long double)minBaud) - utils::math::floor(realTimerValue / (long double)minBaud) <= 0.1,
                      "Minimum Baud not within acceptable error margin!");
        static_assert((realTimerValue / (long double)maxBaud) - utils::math::floor(realTimerValue / (long double)maxBaud) <= 0.1,
                      "Maximum Baud not within acceptable error margin!");
        static_assert(minBaud <= maxBaud, "Minimum Baud has to be below or equal to maximum baud!");


        pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
        pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();

        waitForSync();
    }

    static auto waitForSync() {
        while(true) {
            while(isHigh()){}           //skip first high

            auto startValue = timer::readValue();
            while (!isHigh()) {         //measure first low time
            }
            auto endValue = timer::readValue();
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
    }

    static auto receiveData() {
        uint8_t buffer = 0;
        uint8_t i = 0;                      // this position is mandatory
        while(isHigh()){}                   // skip everything before start (this will keep the sync)
        for(; i < 9; i++) {                  // 8-N-1 (will overwrite start bit)
            auto startValue = timer::readValue();
            _delay_us(10);                  // todo: replace this
            buffer /= 2;                    // lshift
            if(isHigh()) {
                buffer |= (1u << 7u);
            }
            while(calculateTime(startValue, timer::readValue()) < bitcellLength ) {  }
        }
        while(!isHigh()){}                  // skip last low (stop bit)
        return buffer;
    }
};

template<typename mcu>
int16_t SoftwareUart<mcu, SoftUartMethod::Timer>::bitcellLength = 0;


