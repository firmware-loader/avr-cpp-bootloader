//
// Created by Sebastian on 16.05.2019.
//

#pragma once

#include "../../hal/MCUFamilies.h"
#include "../../hal/avr/utils/Timer8.h"

namespace lib::software {

    template<typename MicroController>
    class AbstractTimer {
    public:
        template<unsigned long speed>
        constexpr static void init() {
            if constexpr (MicroController::family == MCUFamilies::AVR) {
                namespace timerNS = lib::avr::timer8bit;
                using timer = timerNS::Timer8<MicroController, 0>;
                //TODO: Actually calculate this
                constexpr auto clockDiv = timerNS::ClockConfig::Div64;

                timer::template init<clockDiv, timerNS::TimerCompareMode::NormalOperation, timerNS::WaveformGeneratorModes::Normal, timerNS::TimerTop::Max>();
            }
        }

        [[nodiscard]] static constexpr auto readValue() {
            if constexpr (MicroController::family == MCUFamilies::AVR) {
                namespace timerNS = lib::avr::timer8bit;
                using timer = timerNS::Timer8<MicroController, 0>;

                return timer::readValue();
            }
        }
    };
}



