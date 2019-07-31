#ifdef AVR
#undef AVR
#endif

#include "src/hal/avr/mcus/mega328/Mega328.h"
#include "src/abstraction/uart/AbstractUart.h"
#include "src/software/uart/implementation/SoftwareUart.h"
#include "src/software/uart/implementation/TimerSoftwareUart.h"
#include "src/software/uart/AbstractSoftwareUart.h"
#include "src/hal/avr/utils/bootloader/mega/Boot.h"
#include "src/software/updi/AbstractSoftwareUPDI.h"
#include "src/software/updi/UpdiProtocol.h"


using mcu = lib::avr::ATMega328;

int main() {
    using namespace lib::software::literals;
    //using uart = lib::software::Uart<mcu>;
    //using softUart = lib::software::AbstractSoftwareUart<mcu, 0, lib::software::SoftUartMethod::Timer>;
    //using softUPDI = lib::software::AbstractSoftwareUPDI<mcu, 0, lib::software::SoftUPDIMethod::UPDITimingBased>;
    //using bootloader = lib::avr::boot::BootloaderHal<mcu>;
    using updiProtocol = lib::software::UPDIProtocol<mcu, 0>;


    updiProtocol::init();
    //uart::init<9600_baud>();

    //softUPDI::init<9600_baud, 9600_baud>();

    DDRD |= (1 << PD2);
    /*softUart::init<9600_baud, 9600_baud>();
    if((softUart::gotSignalBeforeTimout<uint16_t>())) {
        bootloader::writeToFlash(softUart::getWord, [] { return softUart::getBytes<16>(); });
    } else {
        bootloader::startUserProgram();
    }*/

    while(true) {
    /*   auto meta1 = softUart::getWord();
    uart::sendChar(meta1 & 0xFF);
    uart::sendChar((meta1 >> 8u));
        auto meta2 = softUart::getWord();
    uart::sendChar(meta2 & 0xFF);
    uart::sendChar((meta2 >> 8u));*/
    /*const auto& word = softUPDI::getBytes<16>();
        softUPDI::sendChar((*word)[0]);
        softUPDI::sendChar((*word)[1]);
        softUPDI::sendChar((*word)[2]);
        softUPDI::sendChar((*word)[3]);
        softUPDI::sendChar((*word)[4]);
        softUPDI::sendChar((*word)[5]);
        softUPDI::sendChar((*word)[6]);
        softUPDI::sendChar((*word)[7]);
        softUPDI::sendChar((*word)[8]);
        softUPDI::sendChar((*word)[9]);
        softUPDI::sendChar((*word)[10]);
        softUPDI::sendChar((*word)[11]);
        softUPDI::sendChar((*word)[12]);
        softUPDI::sendChar((*word)[13]);
        softUPDI::sendChar((*word)[14]);
        softUPDI::sendChar((*word)[15]);
        //uart::sendChar(word & 0xFF);
        //uart::sendChar((word >> 8u));*/
        /*uart::sendChar((word >> 16u));
        uart::sendChar((word >> 24u));
        uart::sendChar((word >> 32u));
        uart::sendChar((word >> 40u));
        uart::sendChar((word >> 48u));
        uart::sendChar((word >> 56u));*/
   }
}