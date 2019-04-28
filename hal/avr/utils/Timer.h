//
// Created by Sebastian on 23.04.2019.
//

#pragma once

#include "AvrUtils.h"
namespace lib::avr::timer {
    enum class TimerResolution : uint8_t {
        EightBit,
        NineBit,
        TenBit,
        SixteenBit
    };

    enum class WaveformGeneratorModes : uint8_t {
        Normal,
        PWMPhaseCorrect,
        ClearCounterOnCompareMatch,
        FastPWM
    };

    enum class OCRUpdateMode {
        TopReached,
        BottomReached,
        Immediate
    };

    enum class TimerOverflowFlagOn {
        Max,
        Top,
        Bottom
    };

    enum class ClockConfig : uint8_t  {
        STOP,
        NoPrescaler,
        Div8,
        Div64,
        Div256,
        Div1024,
        ExternalSourceOnFallingEdge,
        ExternalSourceOnRisingEdge,
    };

    enum class TimerTop : uint8_t  {
        OCRA,
        ICR,
        Max
    };

    template<WaveformGeneratorModes mode, OCRUpdateMode updateMode, TimerOverflowFlagOn flagOn, TimerResolution resolution, TimerTop top>
    struct WaveformGenerationMode;

    template<typename MicroController, typename MicroController::mem_width TimerChannel>
    class Timer {
    private:
        static constexpr auto timer = lib::avr::getAddress<typename MicroController::Timer16Bit, TimerChannel>;
        using tccra = typename MicroController::Timer16Bit::tccra;
        using tccrb = typename MicroController::Timer16Bit::tccrb;
        using tccrc = typename MicroController::Timer16Bit::tccrc;


        template<WaveformGeneratorModes mode, OCRUpdateMode updateMode, TimerOverflowFlagOn flagOn, TimerResolution resolution, TimerTop top = TimerTop::Max>
        static constexpr void setWaveformGeneratorMode() {
            WaveformGenerationMode<mode, updateMode, flagOn, resolution, top> mode;
        }
            /*if(mode == WaveformGeneratorModes::Normal && updateMode == OCRUpdateMode::Immediate &&
                flagOn == TimerOverflowFlagOn::Max && resolution == TimerResolution::SixteenBit) {
                //Nothing (0, 0, 0)
            } else if(mode == WaveformGeneratorModes::Normal && updateMode == OCRUpdateMode::TopReached &&
                flagOn == TimerOverflowFlagOn::Bottom) {
                if(resolution == TimerResolution::EightBit) {
                    *timer()->tccra.add(tccra::wgm0);
                } else if(resolution == TimerResolution::NineBit) {
                    *timer()->tccra.add(tccra::wgm1);
                } else if(resolution == TimerResolution::TenBit) {
                    *timer()->tccra.add(tccra::wgm0, tccra::wgm1);
                }
            } else if(mode == WaveformGeneratorModes::ClearCounterOnCompareMatch && updateMode == OCRUpdateMode::TopReached &&
                       flagOn == TimerOverflowFlagOn::Bottom && resolution == TimerResolution::SixteenBit) {
                *timer()->tccrb.add(tccrb::wgm2);
            } else if(mode == WaveformGeneratorModes::FastPWM && updateMode == OCRUpdateMode::BottomReached &&
                     flagOn == TimerOverflowFlagOn::Top) {
                *timer()->tccrb.add(tccrb::wgm2);
                if(resolution == TimerResolution::EightBit) {
                    *timer()->tccra.add(tccra::wgm0);
                } else if(resolution == TimerResolution::NineBit) {
                    *timer()->tccra.add(tccra::wgm1);
                } else if(resolution == TimerResolution::TenBit) {
                    *timer()->tccra.add(tccra::wgm0, tccra::wgm1);
                }
            }*/


        template<ClockConfig config>
        static constexpr void setClockConfig() {
            switch(config) {
                case ClockConfig::STOP:
                    break;
                case ClockConfig::NoPrescaler:
                    *timer()->tccrb.add(tccrb::cs0);
                    break;
                case ClockConfig::Div8:
                    *timer()->tccrb.add(tccrb::cs1);
                    break;
                case ClockConfig::Div64:
                    *timer()->tccrb.add(tccrb::cs0, tccrb::cs1);
                    break;
                case ClockConfig::Div256:
                    *timer()->tccrb.add(tccrb::cs2);
                    break;
                case ClockConfig::Div1024:
                    *timer()->tccrb.add(tccrb::cs0, tccrb::cs2);
                    break;
                case ClockConfig::ExternalSourceOnFallingEdge:
                    *timer()->tccrb.add(tccrb::cs1, tccrb::cs2);
                    break;
                case ClockConfig::ExternalSourceOnRisingEdge:
                    *timer()->tccrb.add(tccrb::cs0, tccrb::cs1, tccrb::cs2);
                    break;
            }
        }

    };

    template<>
    struct WaveformGenerationMode<WaveformGeneratorModes::Normal, OCRUpdateMode::Immediate, TimerOverflowFlagOn::Max, TimerResolution::SixteenBit, TimerTop::Max> {
        static constexpr auto wgm0 = false;
        static constexpr auto wgm1 = false;
        static constexpr auto wgm2 = false;
        static constexpr auto wgm3 = false;
    };

    template<>
    struct WaveformGenerationMode<WaveformGeneratorModes::PWMPhaseCorrect, OCRUpdateMode::TopReached, TimerOverflowFlagOn::Bottom, TimerResolution::EightBit, TimerTop::Max> {
        static constexpr auto wgm0 = true;
        static constexpr auto wgm1 = false;
        static constexpr auto wgm2 = false;
        static constexpr auto wgm3 = false;
    };

    template<>
    struct WaveformGenerationMode<WaveformGeneratorModes::PWMPhaseCorrect, OCRUpdateMode::TopReached, TimerOverflowFlagOn::Bottom, TimerResolution::NineBit, TimerTop::Max> {
        static constexpr auto wgm0 = false;
        static constexpr auto wgm1 = true;
        static constexpr auto wgm2 = false;
        static constexpr auto wgm3 = false;
    };

    template<>
    struct WaveformGenerationMode<WaveformGeneratorModes::PWMPhaseCorrect, OCRUpdateMode::TopReached, TimerOverflowFlagOn::Bottom, TimerResolution::TenBit, TimerTop::Max> {
        static constexpr auto wgm0 = true;
        static constexpr auto wgm1 = true;
        static constexpr auto wgm2 = false;
        static constexpr auto wgm3 = false;
    };
}


