//
// Created by Sebastian on 20.05.2019.
//

#pragma once


namespace utils {
    template<auto bitcount>
    static constexpr unsigned long long getMaxValueOfBitcount() {
        constexpr unsigned long long value = (1 << (bitcount + 1));
        return value - 1;
    }

    namespace math {
        template<typename T>
        static constexpr long double floor(T value) {
            return (long double) value;
        }
    }
}



