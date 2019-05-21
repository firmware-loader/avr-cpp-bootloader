//
// Created by Sebastian on 16.05.2019.
//

#pragma once

#include "../../hal/MCUFamilies.h"
#include "../../hal/avr/utils/Timer8.h"

namespace lib::software {

    struct Hertz {
        //    constexpr KiloHertz(unsigned long long kilohertz) : mKHz{kilohertz}{ }
        explicit constexpr operator unsigned long long() const { return static_cast<unsigned long long>(mKHz); }
        //    [[nodiscard]] constexpr auto khz() const -> unsigned long long { return mKHz; };
        //private:
        unsigned long long mKHz;
    };

    namespace {
        template<typename MicroController, MCUFamilies family>
        struct MCUDetail;

        template<typename MicroController>
        struct MCUDetail<MicroController, MCUFamilies::AVR> {
        private:
            template<unsigned long long speed>
            constexpr static auto getClockDiv() {
                namespace timerNS = lib::avr::timer8bit;
                constexpr auto div = F_CPU / speed;
                if constexpr(div < 4) {
                    return timerNS::ClockConfig::NoPrescaler;
                } else if(div >= 4 && div < 32) {
                    return timerNS::ClockConfig::Div8;
                } else if(div >= 32 && div < 128) {
                    return timerNS::ClockConfig::Div64;
                } else if(div >= 128 && div < 512) {
                    return timerNS::ClockConfig::Div256;
                } else {
                    return timerNS::ClockConfig::Div1024;
                }
            }
        public:
            template<auto config>
            static constexpr void avrInit() {
                namespace timerNS = lib::avr::timer8bit;
                using timer = timerNS::Timer8<MicroController, 0>;
                timer::template init<config, timerNS::TimerCompareMode::NormalOperation, timerNS::WaveformGeneratorModes::Normal, timerNS::TimerTop::Max>();
            }

            template<unsigned long long speed>
            constexpr static void init() {
                avrInit<getClockDiv<speed>()>();
            }

            template<unsigned long long speed>
            constexpr static auto getRealClockValue() {
                using clockConfig = lib::avr::timer8bit::ClockConfig;
                constexpr clockConfig div = getClockDiv<speed>();

                static_assert(div != clockConfig::STOP ||
                              div != clockConfig::ExternalSourceOnRisingEdge ||
                              div != clockConfig::ExternalSourceOnFallingEdge,
                              "Invalid Clock Div Value!");

                switch (div) {
                    case clockConfig::NoPrescaler:
                        return Hertz{F_CPU};
                    case clockConfig::Div8:
                        return Hertz{F_CPU / 8};
                    case clockConfig::Div64:
                        return Hertz{F_CPU / 64};
                    case clockConfig::Div256:
                        return Hertz{F_CPU / 256};
                    case clockConfig::Div1024:
                        return Hertz{F_CPU / 1024};
                    default:
                        return Hertz{F_CPU};
                }

            }

            [[nodiscard]] static constexpr auto readValue() {
                namespace timerNS = lib::avr::timer8bit;
                using timer = timerNS::Timer8<MicroController, 0>;

                return timer::readValue();
            }

            [[nodiscard]] static constexpr auto timerBitCount() {
                namespace timerNS = lib::avr::timer8bit;
                using timer = timerNS::Timer8<MicroController, 0>;

                return timer::bitCount();
            }
        };
    }

    namespace literals {
        constexpr Hertz operator"" _hz ( unsigned long long hz )
        {
            return Hertz{hz};
        }
    }

    template<typename MicroController>
    class AbstractTimer {
    private:
        using mcuDetail = MCUDetail<MicroController, MicroController::family>;
    public:
        template<Hertz speed>
        constexpr static auto init() {
            return mcuDetail::template init<static_cast<unsigned long long>(speed)>();
        }

        template<Hertz speed>
        [[nodiscard]] static constexpr auto getRealClockValue() {
            return mcuDetail::template getRealClockValue<static_cast<unsigned long long>(speed)>();
        }

        [[nodiscard]] static constexpr auto readValue() {
                return mcuDetail::readValue();
        }

        [[nodiscard]] static constexpr auto timerBitCount() {
            return mcuDetail::timerBitCount();
        }


    };
}



