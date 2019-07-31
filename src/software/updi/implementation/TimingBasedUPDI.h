//
// Created by Sebastian on 19.06.2019.
//

#pragma once
#include "../../../utils/TypeTraits.h"
#include "../../../utils/TimingUtils.h"
#include "SoftwareUPDI.h"

namespace lib::software {

    template<typename mcu, auto pinNumber>
    requires pin::isAbstractPin<pin::Pin < mcu, pinNumber>>
    class SoftwareUPDI<mcu, pinNumber, SoftUPDIMethod::UPDITimingBased> {
    private:
        using Pin = pin::Pin<mcu, pinNumber>::value;
        using output = Pin::Output;
        using input = Pin::Input;
        static typename utils::max_type<typename mcu::mem_width, uint16_t>::type mCounter;

        static constexpr auto isHigh() {
            return (Pin::get() != 0);
        }
    public:
        template<Baud minBaud, Baud maxBaud>
        static constexpr void init() {
            Pin::template dir<input>();
            //Pin::on();
        }

        static auto waitForSync() {
            Pin::template dir<input>();
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
            Pin::template dir<input>();
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
            while (!isHigh()) {}                  // skip last low (parity bit)
            return buffer;
        }

        static inline void WaitBitcell(uint16_t &j, const uint16_t &mCounterTmp) {
            START_MEASUREMENT
            j = mCounterTmp;
            while ((j -= TIMING_CONSTANT_1) > 0) { asm volatile(""); }
            STOP_MEASUREMENT
        }

        static void sendData(uint8_t data) {
            Pin::template dir<output>();
            Pin::on();
            uint16_t j = 0;
            uint16_t mCounterTmp = mCounter;
            bool parity = getPairty(data);

            WaitBitcell(j, mCounterTmp);

            Pin::off();
            WaitBitcell(j, mCounterTmp);

            for (uint8_t i = 0; i < 8; i++) {
                if(data & 1)
                    Pin::on();
                else
                    Pin::off();
                data /= 2;
                WaitBitcell(j, mCounterTmp);
            }
            if(parity) {
                Pin::on();
            } else {
                Pin::off();
            };
            WaitBitcell(j, mCounterTmp);
            Pin::on();
            WaitBitcell(j, mCounterTmp);
        }

        static bool getPairty(uint8_t data) {
            data ^= data >> 4;
            data ^= data >> 2;
            data ^= data >> 1;
            return (data) & 1;
        }
    };



    template<typename mcu, auto pinNumber>
    typename utils::max_type<typename mcu::mem_width, uint16_t>::type SoftwareUPDI<mcu, pinNumber, SoftUPDIMethod::UPDITimingBased>::mCounter = 0;
}



