//
// Created by Sebastian on 16.05.2019.
//

#pragma once

#include "../../hal/MCUFamilies.h"
#include "../../hal/avr/utils/Timer8.h"

namespace lib::software {

    class KiloHertz {
    public:
        constexpr KiloHertz(unsigned long long kilohertz) : mKHz{kilohertz}{ }
        constexpr operator unsigned long() const { return static_cast<unsigned long>(mKHz); }
        [[nodiscard]] constexpr auto khz() const -> unsigned long long { return mKHz; };
    private:
        unsigned long long mKHz;
    };

    namespace literals {
        constexpr KiloHertz operator"" _khz ( unsigned long long khz )
        {
            return KiloHertz{khz};
        }
    }

    template<typename MicroController>
    class AbstractTimer {
    private:
        template<auto config>
        static constexpr void avrInit() {
            namespace timerNS = lib::avr::timer8bit;
            using timer = timerNS::Timer8<MicroController, 0>;
            timer::template init<config, timerNS::TimerCompareMode::NormalOperation, timerNS::WaveformGeneratorModes::Normal, timerNS::TimerTop::Max>();
        }
    public:
        template<unsigned long long speed>
        constexpr static void init() {
            if constexpr (MicroController::family == MCUFamilies::AVR) {
                namespace timerNS = lib::avr::timer8bit;
                using timer = timerNS::Timer8<MicroController, 0>;
                constexpr auto div = F_CPU / speed;
                if constexpr(div < 4) {
                    avrInit<timerNS::ClockConfig::NoPrescaler>();
                } else if(div >= 4 && div < 32) {
                    avrInit<timerNS::ClockConfig::Div8>();
                } else if(div >= 32 && div < 128) {
                    avrInit<timerNS::ClockConfig::Div64>();
                } else if(div >= 128 && div < 512) {
                    avrInit<timerNS::ClockConfig::Div256>();
                } else {
                    avrInit<timerNS::ClockConfig::Div1024>();
                }

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



