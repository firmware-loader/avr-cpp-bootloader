//
// Created by Sebastian on 23.04.2019.
//

#pragma once

#include <avr/io.h>
#include "../../abstraction/pins/PinControl.h"

enum class Sync : uint8_t {
    Synced,
    Syncing
};

template<typename mcu>
class SoftwareUart {
private:
    static constexpr auto blockstart = 0xCC;

    static int16_t bitcellLength;
    static Sync sync;

    static constexpr auto isHigh() {
        //return pin::readPinState<pin::Pin<mcu, 0>>() == pin::State::ON;
        using pin = pin::Pin<mcu, 0>::value;
        return (pin::get() != 0);
    }

public:
    static constexpr auto preamble = 0x55;
    template<auto pinNumber>
    static constexpr void init() {
        pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
        pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();
        //waitForSync();
    }


    static constexpr auto waitForSync() {
        while(true) {
            while(isHigh()){}           //skip first high
            while (!isHigh()) {         //measure first low time
                bitcellLength++;        //add to counter
            }
            while (isHigh()) {}         //wait for 2nd low
            while (!isHigh()) {         //measure first low time
                bitcellLength--;        //subtract from counter
            }
            while(!isHigh()){}          //skip low
            if(bitcellLength < 0) {
                while(!isHigh()){}     //repeat until in sync
            } else {
                break;                  //sync
            }
        }
    }

    static auto ReceiveByte() {
        uint8_t buffer = 0;
        uint8_t totalSyncSwitches = (sync == Sync::Syncing) ? 4 : 10; //4 high low switches / 8-N-1
        uint16_t counter = bitcellLength;
        uint16_t tmpCounter = 0;
        uint16_t numElements = 0;

        while(isHigh()){}                   //startbit is always low

        while(totalSyncSwitches != 0) {
            if (isHigh()) {
                while (isHigh()) {
                    counter--;
                    if (counter == 0) {
                        counter = bitcellLength;
                        numElements++;
                        buffer *= 2;
                        buffer |= 1u;
                    }
                }
            } else {
                while (!isHigh()) {
                    counter--;
                    if (counter == 0) {
                        counter = bitcellLength;
                        numElements++;
                        buffer *= 2;
                        buffer |= 0u;
                    }
                }
            }

            if(tmpCounter > 0 && (buffer & 1u) == 0) {
                tmpCounter -= numElements;
            } else if((buffer & 1u) == 0){
                tmpCounter += numElements;
            }

            if(sync == Sync::Syncing) {
                totalSyncSwitches--;
            } else {
                totalSyncSwitches -= numElements;
            }

            if(tmpCounter < 0 && sync == Sync::Syncing) {
                while(isHigh()){}
                while(!isHigh()){}     //repeat until in sync
            } else if(sync == Sync::Syncing) {
                bitcellLength = tmpCounter;
                sync = Sync::Synced;
            } else {
                if(sizeof(typename mcu::mem_width) <= 1) {  // 8 bit mcu
                    asm("nop");
                    asm("nop");
                } else {                                    // 16 Bit or more
                    asm("nop");
                }
                asm("nop");                                 // sync = Sync::Synced
            }
        }
        return buffer;
    }
};

template<typename mcu>
int16_t SoftwareUart<mcu>::bitcellLength = 1;


template<typename mcu>
Sync SoftwareUart<mcu>::sync = Sync::Syncing;