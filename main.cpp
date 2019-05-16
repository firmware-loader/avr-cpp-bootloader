#ifdef AVR
#undef AVR
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "src/hal/avr/mcus/mega328/Mega328.h"
#include "src/hal/avr/pin/Control.h"
#include "src/abstraction/uart/AbstractUart.h"
#include "src/abstraction/pins/PinControl.h"
#include "src/software/uart/SoftwareUart.h"
#include "src/abstraction/uart/AbstractTimer.h"


using mcu = lib::avr::ATMega328;

int main() {
    using namespace lib::software::literals;
    using uart = lib::software::Uart<mcu>;
    using softUart = SoftwareUart<mcu>;
    using timer = lib::software::AbstractTimer<mcu>;

    uart::init<38400_baud>();
    uart::sendData("start!");

    softUart::init<0>();
    timer::init<123>();

    /*while(true) {
        auto baudrate = softUart::waitForSync();
        char buffer [33];
        itoa(baudrate, buffer, 10);
        uart::sendData(buffer);
        uart::sendChar('\n');
    }*/
    while(true) {
        auto value = timer::readValue();
        char buffer[33];
        itoa(value, buffer, 10);
        uart::sendData(buffer);
        uart::sendData("\n\r");
        _delay_ms(100);
    }

    //while(true) {
    //    uart::sendData("synced!");
    //}
}