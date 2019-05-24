//
// Created by Sebastian on 21.05.2019.
//

#pragma once

namespace lib::software::detail {
    namespace {
        //extern "C" volatile int16_t counter;
        extern "C" volatile uint8_t receiveBuffer;

        extern "C" void waitForSyncASM();
        extern "C" void receiveByte();
    }

    static auto sync() {
        waitForSyncASM();
    }

    static auto getByte() {
        receiveByte();

        return receiveBuffer;
    }
}