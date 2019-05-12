#ifdef AVR
#undef AVR
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "src/hal/avr/mcus/mega328/Mega328.h"
#include "src/hal/avr/pin/Control.h"
#include "src/abstraction/uart/AbstractUart.h"
#include "src/hal/avr/utils/Timer.h"
#include "src/abstraction/pins/PinControl.h"


using mcu = lib::avr::ATMega328;

int main() {
    using namespace lib::software::literals;
    using uart = lib::software::Uart<mcu>;

    pin::setDirection<pin::Pin<mcu, 8>, pin::Direction::OUTPUT>();

    pin::setDirection<pin::Pin<mcu, 6>, pin::Direction::INPUT>();
    pin::setInputState<pin::Pin<mcu, 6>, pin::InputState::PULLUP>();
    uart::init<9600_baud>();
    uart::sendData("GO!\n\r");

    while(true) {
        pin::flipOutputState<pin::Pin<mcu, 8>>();
        if(pin::readPinState<pin::Pin<mcu, 6>>() == pin::State::ON) {
            uart::sendData("ON!\n\r");
        } else {
            uart::sendData("OFF!\n\r");
        }
        _delay_ms(500);
    }
}