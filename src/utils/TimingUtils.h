//
// Created by Sebastian on 19.06.2019.
//

#pragma once

#define START_MEASUREMENT(x) asm(";START_MEASUREMENT_"#x);
#define STOP_MEASUREMENT(x) asm(";STOP_MEASUREMENT_"#x);
#define TIMING_CONSTANT(x) 42+x; asm(";TIMING_CONSTANT_"#x);



