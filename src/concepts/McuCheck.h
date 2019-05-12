//
// Created by Sebastian on 21.04.2019.
//

#pragma once
template<typename T>
concept Microcontroller = requires(T a) {
    { std::hash<T>{}(a) } -> std::size_t;
};

template<typename T>
concept ControlRegisterable = requires(T a) {
    { std::hash<T>{}(a) } -> std::size_t;
};



