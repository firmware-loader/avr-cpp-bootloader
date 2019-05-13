//
// Created by Sebastian on 23.04.2019.
//

#pragma once

#include <avr/io.h>
#include "../../abstraction/pins/PinControl.h"

template<typename mcu>
class SoftwareUart {
private:
    static constexpr auto preamble = 0x55;
    static constexpr auto blockstart = 0xCC;

    static int16_t bitcellLength;

    static constexpr auto isHigh() {
        return pin::readPinState<pin::Pin<mcu, 0>>() == pin::State::ON;
    }

    static constexpr auto waitbitcell() {
        auto tmpCell = bitcellLength;

    }

public:
    template<auto pinNumber>
    static constexpr void init() {
        pin::setDirection<pin::Pin<mcu, pinNumber>, pin::Direction::INPUT>();
        pin::setInputState<pin::Pin<mcu, pinNumber>, pin::InputState::PULLUP>();
        waitForSync();
    }


    static constexpr auto waitForSync() {
        int16_t counter = 0;            //add on first low, subtract on 2nd low

        do {
            while(!isHigh()){}          //skip first low
            while (isHigh()) {}         //and first high

            while (!isHigh()) {         //measure first low time
                counter++;              //add to counter
            }
            while (isHigh()) {}         //wait for 2nd low
            while (!isHigh()) {         //measure first low time
                counter--;              //subtract from counter
            }
        } while(counter < 0);           //repeat until in sync
        bitcellLength =  counter;       //return 1 bitcell
    }

    static constexpr auto ReceiveByte() {
        uint8_t buffer = 0;
        //skip first high
        //while(!isHigh()){}

        //8-N-1 Bitcounter
        for(uint8_t i = 0; i < 9; i++) {
            //wait for 1/2 Bitcell
            // ???
            buffer /= 2;                // right shift 0 to 7th bit
            if(isHigh()) {              // if high
                buffer |= (1 << 7);     // set 7th bit to 1
            }
        }
    }
};

template<typename mcu>
int16_t SoftwareUart<mcu>::bitcellLength = 0;