//
// Created by Sebastian on 16.05.2019.
//

#pragma once

namespace utils {
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

}

