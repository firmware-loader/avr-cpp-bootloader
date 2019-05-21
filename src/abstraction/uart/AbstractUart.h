//
// Created by Sebastian on 21.04.2019.
//

#pragma once

#include "../../hal/MCUFamilies.h"
#include "../../hal/avr/utils/Uart.h"
#include "../../software/Literals.h"


namespace lib::software {

    template<typename MicroController>
    class Uart {
    public:
        template<Baud baudrate>
        constexpr static void init() {
            if constexpr (MicroController::family == MCUFamilies::AVR) {
                namespace uartNS = lib::avr::uart;
                using uart = uartNS::UartHal<MicroController, 0>;
                uart::template init<static_cast<unsigned long long>(baudrate), uartNS::Speed::Double, uartNS::TransmissionMode::SimplexTX, uartNS::StopBits::One, uartNS::DataBits::Eight>();
            }
        }

        constexpr static void sendData(const char* data) {
            if constexpr (MicroController::family == MCUFamilies::AVR) {
                namespace uartNS = lib::avr::uart;
                using uart = uartNS::UartHal<MicroController, 0>;
                uart::sendData(data);
            }
        }

        constexpr static void sendChar(const unsigned char data) {
            if constexpr (MicroController::family == MCUFamilies::AVR) {
                namespace uartNS = lib::avr::uart;
                using uart = uartNS::UartHal<MicroController, 0>;
                uart::sendChar(data);
            }
        }
    };
}




