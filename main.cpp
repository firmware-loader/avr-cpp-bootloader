#ifdef AVR
#undef AVR
#endif

#ifdef __AVR_ATtiny2313__
#include "src/hal/avr/mcus/tiny2313/tiny2313.h"
#include "src/hal/avr/mcus/tiny2313/PinMap.h"
#endif

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
#include "src/hal/avr/mcus/mega328/Mega328.h"
#include "src/abstraction/uart/AbstractUart.h"
#include "src/hal/avr/utils/bootloader/mega/Boot.h"
#endif

#include "src/software/uart/implementation/SoftwareUart.h"
#include "src/software/uart/implementation/TimerSoftwareUart.h"
#include "src/software/uart/AbstractSoftwareUart.h"
#include "src/software/updi/AbstractSoftwareUPDI.h"
#include "src/software/updi/UpdiProtocol.h"

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
using mcu = lib::avr::ATMega328;
#endif
#ifdef __AVR_ATtiny2313__
using mcu = lib::avr::ATTiny2313;
#endif

int main() {
    using namespace lib::software::literals;

#if (PROTOCOL == FLAG_DEBUG) || (PROTOCOL == FLAG_UNIDIRECTIONAL)
    using softUart = lib::software::AbstractSoftwareUart<mcu, 0, lib::software::SoftUartMethod::Timer>;
    using bootloader = lib::avr::boot::BootloaderHal<mcu>;
#endif

#if PROTOCOL == FLAG_DEBUG
    using uart = lib::software::Uart<mcu>;
#endif

#if PROTOCOL == FLAG_UPDI
    using updiProtocol = lib::software::UPDIProtocol<mcu, 0>;
    updiProtocol::start();
#elif PROTOCOL == FLAG_UNIDIRECTIONAL
    softUart::init<9600_baud, 9600_baud>();
    if((softUart::gotSignalBeforeTimout<uint16_t>())) {
        bootloader::writeToFlash(softUart::getWord, [] { return softUart::getBytes<16>(); });
    } else {
        bootloader::startUserProgram();
    }
#elif PROTOCOL == FLAG_DEBUG
     while(true) {
        const auto& word = uart::getBytes<16>();
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
        uart::sendChar((*word)[15]);
    }
#endif
    return 0;
}