//
// Created by Sebastian on 21.05.2019.
//

#pragma once

namespace {
    extern "C" volatile int16_t counter;
    extern "C" volatile uint8_t receiveBuffer;

    extern "C" void waitForSyncASM();
    extern "C" void receiveByte();
}

static auto syncAndReciveByte() {
    waitForSyncASM();
    receiveByte();

    return receiveBuffer;
}