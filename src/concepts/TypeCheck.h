//
// Created by Sebastian on 16.05.2019.
//

#pragma once

#include "../utils/TypeTraits.h"

namespace concepts {
    template<class T>
    concept UnsignedType = utils::is_unsigned<T>::value;
}


