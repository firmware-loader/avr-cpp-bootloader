#pragma once

namespace std {
    using byte = uint8_t;

    template<typename T1, typename T2>
    struct is_same {
        static inline constexpr bool value = false;
    };
    template<typename T>
    struct is_same<T, T> {
        static inline constexpr bool value = true;
    };

}