//
// Created by Sebastian on 19.06.2019.
//

#pragma once
#include "../../../utils/TypeTraits.h"
#include "../../../utils/TimingUtils.h"
#include "SoftwareUart.h"

namespace lib::software {
    template<typename mcu, auto pinNumber>
    requires pin::isAbstractPin<pin::Pin < mcu, pinNumber>>
    class SoftwareUart<mcu, pinNumber, SoftUartMethod::TimingBased> {
    private:
        static typename utils::max_type<typename mcu::mem_width, int16_t>::type mCounter;

        static constexpr auto isHigh() {
            using pin = pin::Pin<mcu, 0>::value;
            return (pin::get() != 0);
        }
    public:
        template<Baud minBaud, Baud maxBaud>
        static constexpr void init() {
        }

        static auto waitForSync() {
            auto tmp = 0;
            while (true) {
                tmp = 0;
                while (isHigh()) {}           //skip first high
START_MEASUREMENT(1)
                do {         //measure first low time
                    tmp += TIMING_CONSTANT(1);
                }  while (!isHigh());
STOP_MEASUREMENT(1)
                while (isHigh()) {}         //wait for 2nd low
START_MEASUREMENT(2)
                do {
                    tmp -= TIMING_CONSTANT(2);
                }  while (!isHigh());
STOP_MEASUREMENT(2)
                if (tmp < 0) {
                    while (isHigh()) {}
                    while (!isHigh()) {}      //repeat until in sync
                } else {
                    break;                  //sync
                }
            }
            mCounter = tmp;
        }

        static mcu::mem_width receiveData() {
            return 0;
        }
    };

    template<typename mcu, auto pinNumber>
    typename utils::max_type<typename mcu::mem_width, int16_t>::type SoftwareUart<mcu, pinNumber, SoftUartMethod::TimingBased>::mCounter = 0;
}



