#ifdef AVR
#undef AVR
#endif

#include <avr/io.h>
#include <util/delay.h>
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

    uart::init<19200_baud>();
    softUart::init<0>();

    /*do {
        softUart::waitForSync();
    } while(softUart::receiveData() != 0xCC);*/
    softUart::waitForSync();
    while(softUart::receiveData() != softUart::preamble) {}

    while(true) {
        auto data = softUart::receiveData();
        uart::sendChar(data);
    }
}