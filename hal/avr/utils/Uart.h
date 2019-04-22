//
// Created by Sebastian on 22.04.2019.
//

#pragma once

#include "AvrUtils.h"

namespace lib::avr::uart {
    enum class TransmissionMode {
        SimplexRX,
        SimplexTX,
        FullDuplex
    };

    enum class StopBits {
        One,
        Two
    };

    enum class DataBits {
        Five,
        Six,
        Seven,
        Eight,
        Nine
    };

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

    template<typename MicroController, typename MicroController::mem_width UartChannel>
    class UartHal {
    private:
        static constexpr auto uart = lib::avr::getAddress<typename MicroController::Uart, UartChannel>;
        using ucsrb = typename MicroController::Uart::UCSRB;
        using ucsra = typename MicroController::Uart::UCSRA;
        using ucsrc = typename MicroController::Uart::UCSRC;

        template<unsigned long baudrate>
        [[nodiscard]] static constexpr auto calculateBaudValue() {
            return ((F_CPU+baudrate*8)/(baudrate*16)-1);
        }

        template<auto baudrate>
        static constexpr void setBaudrate() {
            auto baudValue = calculateBaudValue<baudrate>();
            *uart()->ubrrh = baudValue >> 8;
            *uart()->ubrrl = baudValue & 0xFF;
        }

        template<TransmissionMode mode>
        static constexpr void setTransmissionMode() {
            switch(mode) {
                case TransmissionMode::FullDuplex:
                    uart()->ucsrb.add(ucsrb::txen, ucsrb::rxen);
                    break;
                case TransmissionMode::SimplexRX:
                    uart()->ucsrb.add(ucsrb::rxen);
                    break;
                case TransmissionMode::SimplexTX:
                    uart()->ucsrb.add(ucsrb::txen);
                    break;
            }
        }

        template<bool doubleSpeed>
        static constexpr void setDoubleSpeed() {
            if constexpr(doubleSpeed) {
                uart()->ucsra.add(ucsra::u2x);
            } else {
                uart()->ucsra.clear(ucsra::u2x);
            }
        }

        template<StopBits stopBits>
        static constexpr void setStopBits() {
            switch(stopBits) {
                case StopBits::One:
                //    uart()->ucsrc.clear(ucsrc::usbs);
                    break;
                case StopBits::Two:
                    uart()->ucsrc.add(ucsrc::usbs);
                    break;
            }
        }

        template<DataBits dataBits>
        static constexpr void setDataBits() {
            switch(dataBits) {
                case DataBits::Five:
                    uart()->ucsrc.clear(ucsrc::ucsz0, ucsrc::ucsz1, ucsrc::ucsz2);
                    break;
                case DataBits::Six:
                    uart()->ucsrc.add(ucsrc::ucsz0);
                    //uart()->ucsrc.clear(ucsrc::ucsz1, ucsrc::ucsz2);
                    break;
                case DataBits::Seven:
                    uart()->ucsrc.add(ucsrc::ucsz1);
                    //uart()->ucsrc.clear(ucsrc::ucsz0, ucsrc::ucsz2);
                    break;
                case DataBits::Eight:
                    uart()->ucsrc.add(ucsrc::ucsz0, ucsrc::ucsz1);
                    //uart()->ucsrc.clear(ucsrc::ucsz2);
                    break;
                case DataBits::Nine:
                    uart()->ucsrc.add(ucsrc::ucsz0, ucsrc::ucsz1, ucsrc::ucsz2);
                    break;
            }
        }
    public:
        template<unsigned long baudrate, bool doubleSpeed, TransmissionMode mode, StopBits stopBits, DataBits dataBits>
        static constexpr void initUart() {
            setBaudrate<baudrate>();
            setDoubleSpeed<doubleSpeed>();
            setTransmissionMode<mode>();
            setStopBits<stopBits>();
            setDataBits<dataBits>();
        }

        static constexpr void sendData(const char* data) {
            for(uint8_t i = 0; i < sizeof(data); i++) {
                sendChar(data[i]);
            }
        }

        static constexpr void sendChar(uint8_t character) {
            while(!uart()->ucsra.isSet(ucsra::udre)) { }
            *uart()->udr |= character;
        }
    };
}



