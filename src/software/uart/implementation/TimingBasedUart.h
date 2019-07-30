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
        static constexpr auto praeamble = 0x55;
        static typename utils::max_type<typename mcu::mem_width, uint16_t>::type mCounter;

        static constexpr auto isHigh() {
            using pin = pin::Pin<mcu, pinNumber>::value;
            return (pin::get() != 0);
        }
    public:
        template<Baud minBaud, Baud maxBaud>
        static constexpr void init() {
        }

        static auto waitForSync() {
            decltype(mCounter) tmp = 0;
            while (true) {
                while (isHigh()) {}           //skip first high
START_MEASUREMENT
                do {         //measure first low time
                    tmp += TIMING_CONSTANT_1;
                }  while (!isHigh());
STOP_MEASUREMENT
                while (isHigh()) {}         //wait for 2nd low
START_MEASUREMENT
                do {
                    tmp -= TIMING_CONSTANT_1;
                }  while (!isHigh());
STOP_MEASUREMENT
                if (tmp < 0) {
                    while (isHigh()) {}
                    while (!isHigh()) {}      //repeat until in sync
                } else {
                    break;                  //sync
                }
            }
            mCounter = tmp;
            while (receiveData() != praeamble) {}
        }

        static mcu::mem_width receiveData() {
            uint8_t buffer = 0;
            uint16_t j = 0;
            uint16_t mCounterTmp = mCounter;
            while (isHigh()) {}                   // skip everything before start (this will keep the sync)
            START_MEASUREMENT
            for(j =  0; j < mCounterTmp / 2u; j+=TIMING_CONSTANT_2) { asm volatile(""); }
            STOP_MEASUREMENT
            for (uint8_t i = 9; i != 0; i--) {                  // 8-N-1 (will overwrite start bit)
                buffer /= 2;                    // lshift
                if (isHigh()) {
                    buffer |= (1u << 7u);
                }
                START_MEASUREMENT
                j = mCounterTmp;
                while((j-=TIMING_CONSTANT_1) > 0) {asm volatile("");}
                //for(j = mCounterTmp; j > mCounter; j-=TIMING_CONSTANT_2) { asm volatile(""); }
                STOP_MEASUREMENT
            }
            while (!isHigh()) {}                  // skip last low (stop bit)
            return buffer;
        }
    };



    template<typename mcu, auto pinNumber>
    typename utils::max_type<typename mcu::mem_width, uint16_t>::type SoftwareUart<mcu, pinNumber, SoftUartMethod::TimingBased>::mCounter = 0;
}



