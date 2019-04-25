//
// Created by Sebastian on 23.04.2019.
//

#pragma once

template<typename MicroController, typename MicroController::mem_width TimerChannel>
class Timer {
private:
    using tccra = typename MicroController::Timer16Bit::tccra;
    using tccrb = typename MicroController::Timer16Bit::tccrb;
    using tccrc = typename MicroController::Timer16Bit::tccrc;


};



