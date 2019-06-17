//
// Created by Sebastian on 16.05.2019.
//

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace utils {
    using size_t = size_t;

    enum class Endian {
        Little,
        Big
    };

    template<class T, T v>
    struct integral_constant {
        static constexpr T value = v;
        typedef T value_type;
        typedef integral_constant type; // using injected-class-name
        constexpr explicit operator value_type() const noexcept { return value; }

        constexpr value_type operator()() const noexcept { return value; } //since c++14
    };

    using true_type    = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;


    template<class T, class U>
    struct is_same : false_type {
    };

    template<class T>
    struct is_same<T, T> : true_type {
    };

    template<class T>
    struct remove_const {
        typedef T type;
    };
    template<class T>
    struct remove_const<const T> {
        typedef T type;
    };

    template<class T>
    struct remove_volatile {
        typedef T type;
    };
    template<class T>
    struct remove_volatile<volatile T> {
        typedef T type;
    };


    template<class T>
    struct remove_cv {
        typedef typename remove_volatile<typename remove_const<T>::type>::type type;
    };

    template<typename T>
    struct remove_ptr {
        using type = T;
    };

    template<typename T>
    struct remove_ptr<T*> {
        using type = T;
    };

    template<typename T>
    struct remove_ptr<const T*> {
        using type = T;
    };

    template<typename T>
    struct is_pointer : false_type {};

    template<typename T>
    struct is_pointer<T*>  : true_type {};

    template<typename>
    struct is_integral_base : false_type {
    };

    template<>
    struct is_integral_base<bool> : true_type {
    };
    template<>
    struct is_integral_base<char> : true_type {
    };
    template<>
    struct is_integral_base<unsigned char> : true_type {
    };
    template<>
    struct is_integral_base<signed char> : true_type {
    };
    template<>
    struct is_integral_base<int> : true_type {
    };
    template<>
    struct is_integral_base<unsigned int> : true_type {
    };
    template<>
    struct is_integral_base<short> : true_type {
    };
    template<>
    struct is_integral_base<long> : true_type {
    };
    template<>
    struct is_integral_base<unsigned long> : true_type {
    };
    template<>
    struct is_integral_base<long long> : true_type {
    };
    template<>
    struct is_integral_base<unsigned long long> : true_type {
    };


    template<typename T>
    struct is_integral : is_integral_base<T> {
    };

    template<class T>
    struct is_floating_point
            : integral_constant<
                    bool,
                    is_same<float, typename remove_cv<T>::type>::value ||
                    is_same<double, typename remove_cv<T>::type>::value ||
                    is_same<long double, typename remove_cv<T>::type>::value
            > {
    };

    template<class T>
    struct is_arithmetic : integral_constant<bool,
            is_integral<T>::value ||
            is_floating_point<T>::value> {
    };

    namespace detail {
        template<typename T, bool = is_arithmetic<T>::value>
        struct is_unsigned : integral_constant<bool, T(0) < T(-1)> {
        };

        template<typename T>
        struct is_unsigned<T, false> : false_type {
        };
    } // namespace detail

    template<typename T>
    struct is_unsigned : detail::is_unsigned<T>::type {
    };

    template<auto N> requires is_arithmetic<decltype(N)>::value
    struct byte_type;

    template<class T>
    struct is_array : false_type {};

    template<class T>
    struct is_array<T[]> : true_type {};

    template<class T, utils::size_t N>
    struct is_array<T[N]> : true_type {};


    template<class T>
    struct is_bounded_array: false_type {};

    template<class T, utils::size_t N>
    struct is_bounded_array<T[N]> : true_type {};

    template<class T>
    struct bounded_array_size;

    template<class T, utils::size_t N>
    struct bounded_array_size<T[N]> {
        static constexpr auto value = N;
    };

    template<>
    struct byte_type<1>{
        using value_type = uint8_t;
    };

    template<>
    struct byte_type<2> {
        using value_type = uint16_t;
    };

    template<>
    struct byte_type<4> {
        using value_type = uint32_t;
    };

    template<>
    struct byte_type<8> {
        using value_type = uint64_t;
    };

    template<typename T>
    struct bit_count {};

    template<>
    struct bit_count<uint8_t> {
        static constexpr auto value = 8;
    };

    template<>
    struct bit_count<uint16_t> {
        static constexpr auto value = 16;
    };

    template<>
    struct bit_count<uint32_t > {
        static constexpr auto value = 32;
    };

    template<>
    struct bit_count<uint64_t > {
        static constexpr auto value = 64;
    };
}

