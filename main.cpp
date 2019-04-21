#include <avr/io.h>
#include <util/delay.h>
#include "hal/avr/mcus/mega328/Mega328.h"
#include "hal/avr/Register.h"
#include "hal/avr/pin/Control.h"

using mcu = lib::avr::ATMega328;

int main() {
    using PortB = lib::Hal::Port<lib::avr::B, mcu>;
    using PinB0 = lib::Hal::Pin<PortB, 0>;
    PinB0::dir<PinB0::Output>();

    while(true) {
        PinB0::flip();
        _delay_ms(500);
    }
}