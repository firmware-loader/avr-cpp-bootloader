//
// Created by Sebastian on 21.05.2019.
//

#pragma once


namespace lib::software {
    class Baud {
    public:
        explicit constexpr operator unsigned long long() const { return static_cast<unsigned long long>(mBaudrate); }
        unsigned long long mBaudrate;
    };

    namespace literals {
        constexpr Baud operator"" _baud ( unsigned long long baud )
        {
            return Baud{baud};
        }
    }
}


