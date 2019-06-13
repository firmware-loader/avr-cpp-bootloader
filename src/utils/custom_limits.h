//
// Created by Sebastian on 16.05.2019.
//

#pragma once


#include <limits.h>
#include <float.h>

namespace utils {

    template<typename T>
    struct numeric_limits {
        static constexpr auto max() { return T(); }
    };

    template<>
    constexpr auto numeric_limits<bool>::max() {
        return true;
    }

    template<>
    constexpr auto numeric_limits<char>::max() {
        return CHAR_MAX;
    }

    template<>
    constexpr auto numeric_limits<signed char>::max() {
        return SCHAR_MAX;
    }

    template<>
    constexpr auto numeric_limits<unsigned char>::max() {
        return UCHAR_MAX;
    }

    template<>
    constexpr auto numeric_limits<short>::max() {
        return SHRT_MAX;
    }

    template<>
    constexpr auto numeric_limits<unsigned short>::max() {
        return USHRT_MAX;
    }

    template<>
    constexpr auto numeric_limits<int>::max() {
        return INT_MAX;
    }

    template<>
    constexpr auto numeric_limits<unsigned int>::max() {
        return UINT_MAX;
    }

    template<>
    constexpr auto numeric_limits<long>::max() {
        return LONG_MAX;
    }

    template<>
    constexpr auto numeric_limits<unsigned long>::max() {
        return ULONG_MAX;
    }

    /*template<>
    constexpr auto numeric_limits<long long>::max() {
        return LONG_LONG_MAX;
    }

    template<>
    constexpr auto numeric_limits<unsigned long long>::max() {
        return ULONG_LONG_MAX;
    }*/

    template<>
    constexpr auto numeric_limits<float>::max() {
        return FLT_MAX;
    }

    template<>
    constexpr auto numeric_limits<double>::max() {
        return DBL_MAX;
    }

    template<>
    constexpr auto numeric_limits<long double>::max() {
        return LDBL_MAX;
    }
}



