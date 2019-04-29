#ifdef AVR
#undef AVR
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "hal/avr/mcus/mega328/Mega328.h"
#include "hal/avr/pin/Control.h"
#include "abstraction/uart/AbstractUart.h"
#include "hal/avr/utils/Timer.h"


using mcu = lib::avr::ATMega328;

int main() {
    using namespace lib::software::literals;
    //using PortB = lib::Hal::Port<lib::avr::B, mcu>;
    //using PinB0 = lib::Hal::Pin<PortB, 0>;
    using uart = lib::software::Uart<mcu>;

    //PinB0::dir<PinB0::Output>();
    uart::init<9600_baud>();

    while(true) {
        //PinB0::flip();
        uart::sendData("Hallo Welt!\n\r");
        _delay_ms(500);
    }
}