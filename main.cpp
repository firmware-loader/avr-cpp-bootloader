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
#include "src/software/uart/AssemblerSoftwareUart.h"
#include "src/software/uart/external/Sync.h"


using mcu = lib::avr::ATMega328;

int main() {
    using namespace lib::software::literals;
    using uart = lib::software::Uart<mcu>;
    using softUart = lib::software::SoftwareUart<mcu, lib::software::SoftUartMethod::Timer>;

    uart::init<19200_baud>();

    unsigned char buffer[10];
    while(true) {
        sync();
        buffer[0] = (unsigned char)getByte();
        buffer[1] = (unsigned char)getByte();
        buffer[2] = (unsigned char)getByte();
        buffer[3] = (unsigned char)getByte();
        buffer[4] = (unsigned char)getByte();
        buffer[5] = (unsigned char)getByte();
        buffer[6] = (unsigned char)getByte();
        buffer[7] = (unsigned char)getByte();
        buffer[8] = (unsigned char)getByte();
        buffer[9] = (unsigned char)getByte();
        buffer[10] = (unsigned char)getByte();
        buffer[11] = (unsigned char)getByte();
        buffer[12] = (unsigned char)getByte();
        buffer[13] = (unsigned char)getByte();
        buffer[14] = (unsigned char)getByte();
        buffer[15] = (unsigned char)getByte();
        buffer[16] = (unsigned char)getByte();
        buffer[17] = (unsigned char)getByte();

        uart::sendChar(buffer[0]);
        uart::sendChar(buffer[1]);
        uart::sendChar(buffer[2]);
        uart::sendChar(buffer[3]);
        uart::sendChar(buffer[4]);
        uart::sendChar(buffer[5]);
        uart::sendChar(buffer[6]);
        uart::sendChar(buffer[7]);
        uart::sendChar(buffer[8]);
        uart::sendChar(buffer[9]);
        uart::sendChar(buffer[10]);
        uart::sendChar(buffer[11]);
        uart::sendChar(buffer[12]);
        uart::sendChar(buffer[13]);
        uart::sendChar(buffer[14]);
        uart::sendChar(buffer[15]);
        uart::sendChar(buffer[16]);
        uart::sendChar(buffer[17]);
    }
    /*
    softUart::init<0, 9600_baud, 19200_baud>();
    while(softUart::receiveData() != softUart::preamble) {}

    while(true) {
        auto data = softUart::receiveData();
        uart::sendChar(data);
    }*/
}