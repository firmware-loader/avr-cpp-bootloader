//
// Created by Sebastian on 21.04.2019.
//

#pragma once

#include "../../hal/MCUFamilies.h"
#include "../../hal/avr/utils/Uart.h"

namespace lib::software {
    class Baud {
    public:
        constexpr Baud(unsigned long long baudrate) : mBaudrate{baudrate}{ }
        constexpr operator unsigned long() const { return static_cast<unsigned long>(mBaudrate); }
    private:
        unsigned long long mBaudrate;
    };

    namespace literals {
        constexpr Baud operator"" _baud ( unsigned long long baud )
        {
            return Baud{baud};
        }
    }

    template<typename MicroController>
    class Uart {
    public:
        template<unsigned long baudrate>
        constexpr static void init() {
            if constexpr (MicroController::family == MCUFamilies::AVR) {
                namespace uartNS = lib::avr::uart;
                using uart = uartNS::UartHal<MicroController, 0>;
                uart::template init<baudrate, uartNS::Speed::Double, uartNS::TransmissionMode::SimplexTX, uartNS::StopBits::One, uartNS::DataBits::Eight>();
            }
        }

        constexpr static void sendData(const char* data) {
            if constexpr (MicroController::family == MCUFamilies::AVR) {
                namespace uartNS = lib::avr::uart;
                using uart = uartNS::UartHal<MicroController, 0>;
                uart::sendData(data);
            }
        }
    };
}




