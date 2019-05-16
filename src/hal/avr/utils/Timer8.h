//
// Created by Sebastian on 16.05.2019.
//

#pragma once
#include "AvrUtils.h"

namespace lib::avr::timer8bit {

    enum class ClockConfig : uint8_t {
        STOP,
        NoPrescaler,
        Div8,
        Div64,
        Div256,
        Div1024,
        ExternalSourceOnFallingEdge,
        ExternalSourceOnRisingEdge,
    };

    enum class WaveformGeneratorModes : uint8_t {
        Normal,
        PWMPhaseCorrect,
        ClearCounterOnCompareMatch,
        FastPWM
    };

    enum class TimerTop : uint8_t {
        OCRA,
        Max
    };

    enum class TimerCompareMode : uint8_t {
        NormalOperation,
        ToggleOCBOnMatch,
        ClearOCBOnMatch,
        SetOCBOnMatch
    };

    template<WaveformGeneratorModes mode, TimerTop top>
    struct WaveformGenerationMode;

    template<typename MicroController, MicroController::mem_width TimerChannel>
    class Timer8 {
    private:
        static constexpr auto timer = lib::avr::getAddress<typename MicroController::Timer8Bit, TimerChannel>;
        using tccra = typename MicroController::Timer8Bit::TCCRA;
        using tccrb = typename MicroController::Timer8Bit::TCCRB;

        template<WaveformGeneratorModes mode, TimerTop top = TimerTop::Max>
        static constexpr void setWaveformGeneratorMode() {
            using wgm = WaveformGenerationMode<mode, top>;
            wgm::template generate<timer, tccra, tccrb>();
        }

        template<ClockConfig config>
        static constexpr void setClockConfig() {
            switch (config) {
                case ClockConfig::STOP:
                    break;
                case ClockConfig::NoPrescaler:
                    timer()->tccrb.add(tccrb::cs0);
                    break;
                case ClockConfig::Div8:
                    timer()->tccrb.add(tccrb::cs1);
                    break;
                case ClockConfig::Div64:
                    timer()->tccrb.add(tccrb::cs0, tccrb::cs1);
                    break;
                case ClockConfig::Div256:
                    timer()->tccrb.add(tccrb::cs2);
                    break;
                case ClockConfig::Div1024:
                    timer()->tccrb.add(tccrb::cs0, tccrb::cs2);
                    break;
                case ClockConfig::ExternalSourceOnFallingEdge:
                    timer()->tccrb.add(tccrb::cs1, tccrb::cs2);
                    break;
                case ClockConfig::ExternalSourceOnRisingEdge:
                    timer()->tccrb.add(tccrb::cs0, tccrb::cs1, tccrb::cs2);
                    break;
            }
        }

        template<TimerCompareMode mode>
        static constexpr void setCompareMode() {
            switch (mode) {
                case TimerCompareMode::NormalOperation:
                    break;
                case TimerCompareMode::ToggleOCBOnMatch:
                    timer()->tccra.add(tccra::coma0);
                    break;
                case TimerCompareMode::ClearOCBOnMatch:
                    timer()->tccra.add(tccra::coma1);
                    break;
                case TimerCompareMode::SetOCBOnMatch:
                    timer()->tccra.add(tccra::coma0, tccra::coma0);
                    break;
            }
        }
    public:
        template<ClockConfig config, TimerCompareMode compareMode, WaveformGeneratorModes mode, TimerTop top = TimerTop::Max>
        static constexpr void init() {
            //static_assert(compareMode == TimerCompareMode::ToggleOCBOnMatch && mode != WaveformGeneratorModes::FastPWM, "ToggleOCBOnMatch is not allowed in Fast PWM mode!");
            //static_assert(compareMode == TimerCompareMode::ToggleOCBOnMatch && mode != WaveformGeneratorModes::PWMPhaseCorrect, "ToggleOCBOnMatch is not allowed in PWMPhaseCorrect mode");

            setClockConfig<config>();
            setWaveformGeneratorMode<mode, top>();
            setCompareMode<compareMode>();
        }

        [[nodiscard]] static constexpr auto readValue() -> uint8_t {
            return *timer()->tcnt;
        }
    };

    template<>
    struct WaveformGenerationMode<WaveformGeneratorModes::Normal, TimerTop::Max> {
        template<auto timer, typename tccra, typename tccrb>
        static constexpr void generate() {
        }
    };

    template<>
    struct WaveformGenerationMode<WaveformGeneratorModes::PWMPhaseCorrect, TimerTop::Max> {
        template<auto timer, typename tccra, typename tccrb>
        static constexpr void generate() {
            timer()->tccra.add(tccra::wgm0);
        }
    };

    template<>
    struct WaveformGenerationMode<WaveformGeneratorModes::ClearCounterOnCompareMatch, TimerTop::OCRA> {
        template<auto timer, typename tccra, typename tccrb>
        static constexpr void generate() {
            timer()->tccra.add(tccra::wgm1);
        }
    };

    template<>
    struct WaveformGenerationMode<WaveformGeneratorModes::FastPWM, TimerTop::Max> {
        template<auto timer, typename tccra, typename tccrb>
        static constexpr void generate() {
            timer()->tccra.add(tccra::wgm0, tccra::wgm1);
        }
    };

    template<>
    struct WaveformGenerationMode<WaveformGeneratorModes::PWMPhaseCorrect, TimerTop::OCRA> {
        template<auto timer, typename tccra, typename tccrb>
        static constexpr void generate() {
            timer()->tccrb.add(tccra::wgm2);
            timer()->tccra.add(tccra::wgm0);
        }
    };

    template<>
    struct WaveformGenerationMode<WaveformGeneratorModes::FastPWM, TimerTop::OCRA> {
        template<auto timer, typename tccra, typename tccrb>
        static constexpr void generate() {
            timer()->tccrb.add(tccra::wgm2);
            timer()->tccra.add(tccra::wgm0, tccra::wgm1);
        }
    };
}
