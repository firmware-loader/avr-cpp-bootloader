//
// Created by Sebastian on 23.04.2019.
//

#pragma once

#include <avr/io.h>
#include "../../../abstraction/pins/PinControl.h"
#include "../../../abstraction/timer/AbstractTimer.h"
#include "../../../utils/custom_limits.h"
#include "../../../utils/TypeTraits.h"
#include "../../../concepts/TypeCheck.h"
#include "../../../utils/Utility.h"

namespace lib::software {
    enum class SoftUPDIMethod : uint8_t {
        UPDITimingBased
    };

    template<typename mcu, auto pinNumber, SoftUPDIMethod method>
    class SoftwareUPDI;
}

