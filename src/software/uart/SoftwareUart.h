//
// Created by Sebastian on 23.04.2019.
//

#pragma once

#include <avr/io.h>
#include "../../abstraction/pins/PinControl.h"
#include "../../abstraction/uart/AbstractTimer.h"
#include "../../utils/custom_limits.h"

enum class Sync : uint8_t {
    Synced,
    Syncing
};

template<typename mcu>
class SoftwareUart {
private:
    using timer = lib::software::AbstractTimer<mcu>;
    static constexpr auto blockstart = 0xCC;

    static Sync sync;

    static constexpr auto isHigh() {
        //return pin::readPinState<pin::Pin<mcu, 0>>() == pin::State::ON;
        using pin = pin::Pin<mcu, 0>::value;
        return (pin::get() != 0);
    }

public:
    static int16_t bitcellLength;
    static constexpr auto preamble = 0x55;
    template<auto pinNumber>
    static constexpr void init() {
        using namespace lib::software::literals;

        timer::template init<250000_khz>();
        pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
        pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();
        //waitForSync();
    }


    static inline auto calculateTime(auto startValue, auto endValue) {
        return (startValue > endValue) ?
        (utils::numeric_limits<decltype(startValue)>::max() - startValue) + endValue
                                : endValue - startValue;
    }

    static unsigned long long waitForSync() {
        while(true) {
            while(isHigh()){}           //skip first high

            auto startValue = timer::readValue();
            while (!isHigh()) {         //measure first low time
            }
            auto endValue = timer::readValue();

            //overflow protection
            bitcellLength = calculateTime(startValue, endValue);

            while (isHigh()) {}         //wait for 2nd low

            startValue = timer::readValue();
            while (!isHigh()) {
            }
            endValue = timer::readValue();
            bitcellLength -= calculateTime(startValue, endValue);


            if(bitcellLength < 0) {
                while(isHigh()){}
                while(!isHigh()){}      //repeat until in sync
            } else {
                break;                  //sync
            }
        }
        return bitcellLength;    //where do these 10 instructions come from?
    }

    static auto receiveData() {
        uint8_t buffer = 0;
        uint8_t i = 0;
        while(isHigh()){}                   // skip everything before start
        for(; i < 9; i++) {                  // 8-N-1 (will overwrite start bit)
            auto startValue = timer::readValue();
            volatile uint8_t tmp = bitcellLength / 2 ;
            buffer *= 2;                    // lshift
            while(calculateTime(startValue, timer::readValue()) < tmp ) {  }
            if(!isHigh()) {
                buffer |= 1u;                // keep same op count
            }
            while(calculateTime(startValue, timer::readValue()) < bitcellLength ) {  }
        }
        while(!isHigh()){}                  // skip last low (stop bit)
        return buffer;
    }

    static auto ReceiveByte() {
        uint8_t buffer = 0;
        uint8_t totalSyncSwitches = (sync == Sync::Syncing) ? 4 : 10; //4 high low switches / 8-N-1
        uint16_t counter = bitcellLength;
        uint16_t tmpCounter = 0;
        uint8_t num_low = 0;

        while (totalSyncSwitches != 0) {
            uint16_t numElements = 0;

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
            }
            else {
                while (!isHigh()) {
                    counter--;
                    if (counter == 0) {
                        counter = bitcellLength;
                        numElements++;
                        buffer *= 2;
                        buffer |= 0u;
                    }
                }
                num_low++;
            }

            if (tmpCounter > 0 && (buffer & 1u) == 0) {
                tmpCounter -= numElements;
            }
            else if ((buffer & 1u) == 0) {
                tmpCounter += numElements;
            }

            if (sync == Sync::Syncing) {
                totalSyncSwitches--;
            }
            else {
                totalSyncSwitches -= numElements;
            }

            if (tmpCounter < 0  && sync == Sync::Syncing) {
                buffer = 0;
                num_low = 0;
                tmpCounter = 0;
                totalSyncSwitches = 4;
                while (!isHigh()) {}     //repeat until in sync
            }
            else if (sync == Sync::Syncing && num_low >= 2) {
                bitcellLength = tmpCounter;
                sync = Sync::Synced;
                break;                                      //we are in sync now and can skip the last high
            }
            else {
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
int16_t SoftwareUart<mcu>::bitcellLength = 0;


template<typename mcu>
Sync SoftwareUart<mcu>::sync = Sync::Syncing;