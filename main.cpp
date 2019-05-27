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
#include "src/software/uart/vendorspecific/InlineAssemblerSoftwareUart.h"
#include "src/software/uart/vendorspecific/AssemblerSoftwareUart.h"
#include "src/software/uart/vendorspecific/external/Sync.h"
#include "src/hal/avr/utils/bootloader/mega/Boot.h"


using mcu = lib::avr::ATMega328;

int main() {
    using namespace lib::software::literals;
    using uart = lib::software::Uart<mcu>;
    using softUart = lib::software::SoftwareUart<mcu, 0, lib::software::SoftUartMethod::InlineAssembler>;
    using bootloader = lib::avr::boot::BootloaderHal<mcu>;

    uart::init<57600_baud>();
    softUart::init<600_baud, 57600_baud>();
    //bootloader::writeToFlash(0x00, softUart::getWord);

    while(true) {
        auto word = softUart::getBytes<2>();
        uart::sendChar(word & 0xFF);
        uart::sendChar((word >> 8));
    }
}