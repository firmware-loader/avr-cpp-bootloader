//
// Created by Sebastian on 19.06.2019.
//

#pragma once
#include "../../../utils/TypeTraits.h"
#include "../../../utils/TimingUtils.h"
#include "../../uart/implementation/SoftwareUart.h"

namespace lib::software {
    template<typename mcu, auto pinNumber>
    requires pin::isAbstractPin<pin::Pin < mcu, pinNumber>>
    class SoftwareUart<mcu, pinNumber, SoftUartMethod::UPDITimingBased> {
    private:
        static typename utils::max_type<typename mcu::mem_width, uint16_t>::type mCounter;

        static constexpr auto isHigh() {
            using pin = pin::Pin<mcu, pinNumber>::value;
            return (pin::get() != 0);
        }
    public:
        template<Baud minBaud, Baud maxBaud>
        static constexpr void init() {
            DDRD |= (1 << PD2);
            using pin = pin::Pin<mcu, 2>::value;
            pin::on();
        }

        static auto waitForSync() {
            decltype(mCounter) tmp = 0;
            while (isHigh()) {}           //skip first high
            START_MEASUREMENT
            do {         //measure first low time
                tmp += TIMING_CONSTANT_1;
            }  while (!isHigh());
            mCounter = tmp;
            STOP_MEASUREMENT
            for (uint8_t i = 0; i < 4; i++) {
                while (isHigh()) {}           //skip first high
                while (!isHigh()) {}
            }

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
                //PORTD |= (1 << PD2);
                buffer /= 2;                    // lshift
                if (isHigh()) {
                    buffer |= (1u << 7u);
                }
                //
                START_MEASUREMENT
                j = mCounterTmp;
                //PORTD &= ~(1 << PD2);
                while((j-=TIMING_CONSTANT_1) > 0) {asm volatile("");}
                STOP_MEASUREMENT
            }
            while (!isHigh()) {}                  // skip last low (stop bit)
            return buffer;
        }

        static void sendData(uint8_t data) {
            uint16_t j = 0;
            uint16_t mCounterTmp = mCounter;
            using pin = pin::Pin<mcu, 2>::value;

            pin::off();
            START_MEASUREMENT
            j = mCounterTmp;
            while((j-=TIMING_CONSTANT_1) > 0) {asm volatile("");}
            STOP_MEASUREMENT

            for (uint8_t i = 8; i != 0; i--) {
                if(data & 1)
                    pin::on();
                else
                    pin::off();
                data /= 2;
                START_MEASUREMENT
                j = mCounterTmp;
                while((j-=TIMING_CONSTANT_1) > 0) {asm volatile("");}
                STOP_MEASUREMENT
            }
            pin::on();
            START_MEASUREMENT
            j = mCounterTmp;
            while((j-=TIMING_CONSTANT_1) > 0) {asm volatile("");}
            STOP_MEASUREMENT
        }
    };



    template<typename mcu, auto pinNumber>
    typename utils::max_type<typename mcu::mem_width, uint16_t>::type SoftwareUart<mcu, pinNumber, SoftUartMethod::UPDITimingBased>::mCounter = 0;
}


