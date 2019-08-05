//
// Created by Sebastian on 18.06.2019.
//

#pragma once

#include "implementation/TimingBasedUPDI.h"
#include "../../utils/custom_limits.h"

namespace lib::software {
    template<typename mcu, auto pinNumber, SoftUPDIMethod method>
    class AbstractSoftwareUPDI {
    private:
        using softUart = SoftwareUPDI<mcu, pinNumber, method>;
        using selectedPin = pin::Pin<mcu, pinNumber>::value;
    public:
        static uint16_t getWord() {
            softUart::waitForSync();
            return softUart::receiveData() | (static_cast<uint16_t>(softUart::receiveData()) << 8u);
        }

        template<auto N>
        [[nodiscard]] static auto getBytes()
        requires utils::is_arithmetic<decltype(N)>::value
        && N <= 2 {
            using type = utils::byte_type<N>::value_type;
            type value = 0;
            softUart::waitForSync();


            for (typename mcu::mem_width i = 0; i < N; i++) {
                value |= static_cast<type>(softUart::receiveData()) << (8u * i);
            }
            return value;
        }

        template<auto N>
        [[nodiscard]] static const utils::array<typename mcu::mem_width, N> *getBytes()
        requires utils::is_arithmetic<decltype(N)>::value
        && N > 2 && N <= 255 {
            static utils::array<typename mcu::mem_width, N> value;
            softUart::waitForSync();
            for (typename mcu::mem_width i = 0; i < N; i++) {
                value[i] = softUart::receiveData();
            }
            return &value;
        }

        static void sendChar(uint8_t byte) {
            softUart::sendData(byte);
        }

        static void sendString(const char* str) {
            while (*str) {
                softUart::sendData(static_cast<uint8_t>(*str));
                str++;
            }
        }

        static uint8_t getByteWithoutSync() {
            return softUart::receiveData();
        }

        static uint8_t getByte() {
            softUart::waitForSync();
            return softUart::receiveData();
        }

        template<auto minBaud, auto maxBaud>
        static constexpr void init() {
             softUart::template init<minBaud, maxBaud>();
        }

        template<typename T> requires utils::is_arithmetic<T>::value
        static bool gotSignalBeforeTimout() {
            using pin = pin::Pin<mcu, pinNumber>::value;
            for(T i = 0; i < utils::numeric_limits<T>::max(); i++) { // utils::numeric_limits<T>::max()
                asm volatile("");
                if(pin::get() == 0) {
                    while(pin::get() == 0) { asm volatile(""); }
                    return true;
                }
            }
            return false;
        }
    };
}



