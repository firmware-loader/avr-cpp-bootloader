//
// Created by Sebastian on 21.05.2019.
//

#pragma once

namespace lib::software::detail {
    namespace {
        //extern "C" volatile int16_t counter;
        extern "C" volatile uint8_t asm_uart_receiveBuffer;

        extern "C" void asm_uart_waitForSyncASM();
        extern "C" void asm_uart_receiveByte();
    }

    auto sync() -> void {
        asm_uart_waitForSyncASM();
    }

    auto getByte() ->  uint8_t {
        asm_uart_receiveByte();

        return asm_uart_receiveBuffer;
    }
}