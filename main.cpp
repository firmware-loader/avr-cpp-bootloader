#ifdef AVR
#undef AVR
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "hal/avr/mcus/mega328/Mega328.h"
#include "hal/avr/Register.h"
#include "hal/avr/pin/Control.h"
#include "hal/avr/utils/Uart.h"


using mcu = lib::avr::ATMega328;

int main() {
    using namespace  lib::avr::uart::literals;
    namespace uartNS = lib::avr::uart;
    using uart = uartNS::UartHal<mcu, 0>;

    using PortB = lib::Hal::Port<lib::avr::B, mcu>;
    using PinB0 = lib::Hal::Pin<PortB, 0>;

    PinB0::dir<PinB0::Output>();
    uart::init<9600_baud, uartNS::Speed::Double, uartNS::TransmissionMode::FullDuplex, uartNS::StopBits::One, uartNS::DataBits::Eight>();

    while(true) {
        PinB0::flip();
        uart::sendData("Hallo Welt!\n\r");
        _delay_ms(500);
    }
}