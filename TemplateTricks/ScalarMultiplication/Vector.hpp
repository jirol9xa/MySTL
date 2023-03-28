#pragma once

#include <array>
#include <initializer_list>

template <typename T, int N = 2> struct Vector
{
    std::array<T, N> arr_;

    constexpr Vector(const std::array<T, N> &arr) noexcept : arr_(arr) {}

    constexpr Vector(std::initializer_list<T> init) noexcept : arr_(init) {}

    constexpr Vector operator*(const Vector rhs) const noexcept
    {
        std::array<T, N> res_arr;
        auto             rhs_arr = rhs.arr_;

        for (int i = 0; i < N; ++i)
            res_arr[i] = rhs_arr[i] * arr_[i];

        return {res_arr};
    }
};
