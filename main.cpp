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

    //uart::init<38400_baud>();
    softUart::init<384000_baud, 38400_baud>();
    bootloader::writeToFlash([]{return softUart::getBytes<16>(); });

    while(true) {
        /*auto word = softUart::getBytes<8>();
        uart::sendChar(word[0]);
        uart::sendChar(word[1]);
        uart::sendChar(word[2]);
        uart::sendChar(word[3]);
        uart::sendChar(word[4]);
        uart::sendChar(word[5]);
        uart::sendChar(word[6]);
        uart::sendChar(word[7]);*/
        //uart::sendChar(word & 0xFF);
        //uart::sendChar((word >> 8u));
        /*uart::sendChar((word >> 16u));
        uart::sendChar((word >> 24u));
        uart::sendChar((word >> 32u));
        uart::sendChar((word >> 40u));
        uart::sendChar((word >> 48u));
        uart::sendChar((word >> 56u));*/
    }
}