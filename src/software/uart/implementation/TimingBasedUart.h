//
// Created by Sebastian on 19.06.2019.
//

#pragma once
#include "../../../utils/TypeTraits.h"
#include "../../../utils/TimingUtils.h"
#include "SoftwareUart.h"

namespace lib::software {
    extern "C" {
        volatile uint8_t receiveBuffer2;
    }

    template<typename mcu, auto pinNumber>
    requires pin::isAbstractPin<pin::Pin < mcu, pinNumber>>
    class SoftwareUart<mcu, pinNumber, SoftUartMethod::TimingBased> {
    private:
        static constexpr auto praeamble = 0x55;
        static typename utils::max_type<typename mcu::mem_width, uint16_t>::type mCounter;

        static constexpr auto isHigh() {
            using pin = pin::Pin<mcu, 0>::value;
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
                    tmp += 5;
                }  while (!isHigh());
STOP_MEASUREMENT
                while (isHigh()) {}         //wait for 2nd low
START_MEASUREMENT
                do {
                    tmp -= 5;
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
            while (isHigh()) {}                   // skip everything before start (this will keep the sync)
            START_MEASUREMENT
            for(uint16_t j =  0; j < mCounter / 2u; j+=7) { asm volatile(""); }
            STOP_MEASUREMENT
            for (uint8_t i = 9; i != 0; i--) {                  // 8-N-1 (will overwrite start bit)
                buffer /= 2;                    // lshift
                if (isHigh()) {
                    buffer |= (1u << 7u);
                }
                START_MEASUREMENT
                for(uint16_t j = 0; j < mCounter; j+=7) { asm volatile(""); }
                STOP_MEASUREMENT
            }
            while (!isHigh()) {}                  // skip last low (stop bit)
            return buffer;
        }
        /*static void asmReceiveData() {
            asm volatile(R"(
                rjmp receiveByte
                WaitBitcell:
                        movw xl, %A[cb]
                wbc0:
                        sbiw xl, 4
                        brcc wbc0
                wbcx:
                        sts %[rb], r20
                        ret
                receiveByte:
                        sbic %[pin],%[bit]
                        rjmp receiveByte
                        ldi r21, lo8(8)
                CL9:
                        movw xl, %A[cb]
                        lsr xh
                        ror xl
                        ldi r21, 9
                rxb3:
                        rcall WaitBitcell
                        lsr r20
                        sbic %[pin],%[bit]
                        ori r20, 128
                        dec r21
                        brne rxb3
            )"
            : [rb] "=m" (receiveBuffer2)
            : [pin] "I" (_SFR_IO_ADDR(PIND)), [bit] "n" (0), [cb] "w" (mCounter)
            : "r20", "r21");
        }

        [[nodiscard]] static auto receiveData() {
            asmReceiveData();
            return receiveBuffer2;
        }*/
    };



    template<typename mcu, auto pinNumber>
    typename utils::max_type<typename mcu::mem_width, uint16_t>::type SoftwareUart<mcu, pinNumber, SoftUartMethod::TimingBased>::mCounter = 0;
}



