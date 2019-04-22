#ifdef AVR
#undef AVR
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "hal/avr/mcus/mega328/Mega328.h"
#include "hal/avr/Register.h"
#include "hal/avr/pin/Control.h"
#include "hal/avr/mcus/utils/Uart.h"


using mcu = lib::avr::ATMega328;

int main() {
    using PortB = lib::Hal::Port<lib::avr::B, mcu>;
    using PinB0 = lib::Hal::Pin<PortB, 0>;
    PinB0::dir<PinB0::Output>();
    lib::avr::uart::UartHal<mcu, 0>::initUart<9600, true, lib::avr::uart::TransmissionMode::FullDuplex, lib::avr::uart::StopBits::One, lib::avr::uart::DataBits::Eight>();

    while(true) {
        PinB0::flip();
        lib::avr::uart::UartHal<mcu, 0>::sendChar('a');
        _delay_ms(500);
    }
}