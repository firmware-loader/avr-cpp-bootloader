#ifdef AVR
#undef AVR
#endif

#include <stdlib.h>
#include "src/hal/avr/mcus/mega328/Mega328.h"
#include "src/abstraction/uart/AbstractUart.h"
#include "src/software/uart/implementation/SoftwareUart.h"
#include "src/software/uart/implementation/TimerSoftwareUart.h"
#include "src/software/uart/AbstractSoftwareUart.h"
#include "src/hal/avr/utils/bootloader/mega/Boot.h"


using mcu = lib::avr::ATMega328;

int main() {
    using namespace lib::software::literals;
    //using uart = lib::software::Uart<mcu>;
    //using softUart = lib::software::SoftwareUart<mcu, 0, lib::software::SoftUartMethod::InlineAssembler>;
    using softUart = lib::software::AbstractSoftwareUart<mcu, 0, lib::software::SoftUartMethod::InlineAssembler>;
    using bootloader = lib::avr::boot::BootloaderHal<mcu>;

    //uart::init<38400_baud>();
    softUart::init<9600_baud, 9600_baud>();
    if((softUart::gotSignalBeforeTimout<uint16_t>())) {
        bootloader::writeToFlash(softUart::getWord, [] { return softUart::getBytes<16>(); });
    } else {
        bootloader::startUserProgram();
    }

    //while(true) {
    /*    auto meta1 = softUart::getWord();
    uart::sendChar(meta1 & 0xFF);
    uart::sendChar((meta1 >> 8u));
        auto meta2 = softUart::getWord();
    uart::sendChar(meta2 & 0xFF);
    uart::sendChar((meta2 >> 8u));*/

    /*const auto& word = softUart::getBytes<16>();
    uart::sendChar((*word)[0]);
    uart::sendChar((*word)[1]);
    uart::sendChar((*word)[2]);
    uart::sendChar((*word)[3]);
    uart::sendChar((*word)[4]);
    uart::sendChar((*word)[5]);
    uart::sendChar((*word)[6]);
    uart::sendChar((*word)[7]);
    uart::sendChar((*word)[8]);
    uart::sendChar((*word)[9]);
    uart::sendChar((*word)[10]);
    uart::sendChar((*word)[11]);
    uart::sendChar((*word)[12]);
    uart::sendChar((*word)[13]);
    uart::sendChar((*word)[14]);
    uart::sendChar((*word)[15]);*/
        //uart::sendChar(word & 0xFF);
        //uart::sendChar((word >> 8u));
        /*uart::sendChar((word >> 16u));
        uart::sendChar((word >> 24u));
        uart::sendChar((word >> 32u));
        uart::sendChar((word >> 40u));
        uart::sendChar((word >> 48u));
        uart::sendChar((word >> 56u));*/
    //}
}