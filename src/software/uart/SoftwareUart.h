//
// Created by Sebastian on 23.04.2019.
//

#pragma once

#include <avr/io.h>
#include "../../abstraction/pins/PinControl.h"
#include "../../abstraction/timer/AbstractTimer.h"
#include "../../utils/custom_limits.h"
#include "../../utils/TypeTraits.h"
#include "../../concepts/TypeCheck.h"
#include "../../utils/Utility.h"

enum class Sync : uint8_t {
    Synced,
    Syncing
};

enum class SoftUartMethod : uint8_t {
    Timer,
    Assembler
};

template<typename mcu, SoftUartMethod method>
class SoftwareUart;

