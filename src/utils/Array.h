//
// Created by Sebastian on 14.06.2019.
//

#pragma once
#include "TypeTraits.h"

namespace utils {
    template<typename T, utils::size_t N> requires sizeof(T) == 1
    class array {
    public:
        [[nodiscard]] constexpr T& operator[](auto position) {
            return mElement[position];
        }

        [[nodiscard]] constexpr T& operator[](auto position) const {
            return mElement[position];
        }

        [[nodiscard]] constexpr utils::size_t max_size() const {
            return N;
        }

        [[nodiscard]] static constexpr utils::size_t static_size() {
            return N;
        }

        [[nodiscard]] constexpr T *begin() {
            return mElement[0];
        }

        [[nodiscard]] constexpr T *begin() const {
            return mElement[0];
        }

        [[nodiscard]] constexpr T *end() {
            return mElement[N];
        }

        [[nodiscard]] constexpr T *end() const {
            return mElement[N];
        }

        template<Endian endian>
        [[nodiscard]] constexpr utils::byte_type<N> makeNumber() {
            utils::byte_type<N> buffer;
            for(uint8_t i=0; i < N; i++) {
                if constexpr(endian == Endian::Little)
                    buffer |= (mElement[i] << i*8);
                else
                    buffer |= (mElement[i] << (N-i-1)*8);
            }
        }
    private:
        T mElement[N];
    };
}



