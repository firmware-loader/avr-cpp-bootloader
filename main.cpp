#ifdef AVR
#undef AVR
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "src/hal/avr/mcus/mega328/Mega328.h"
#include "src/abstraction/uart/AbstractUart.h"
#include "src/software/uart/SoftwareUart.h"
#include "src/software/uart/TimerSoftwareUart.h"
#include "src/software/uart/InlineAssemblerSoftwareUart.h"
#include "src/software/uart/external/Sync.h"


using mcu = lib::avr::ATMega328;

int main() {
    using namespace lib::software::literals;
    using uart = lib::software::Uart<mcu>;
    using softUart = lib::software::SoftwareUart<mcu, lib::software::SoftUartMethod::Assembler>;

    uart::init<19200_baud>();
    softUart::init<0, 9600_baud, 19200_baud>();

    while(true) {
        constexpr auto elements = 10;
        unsigned char buffer[elements];
        softUart::syncAndReceiveBytes(buffer, elements);
        uart::sendChar(buffer[elements - 1]);
    }
}