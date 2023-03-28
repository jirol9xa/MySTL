#pragma once

#include <array>
#include <cstdint>
#include <initializer_list>

template <typename T, int columns, int rows> class Matrix
{
  private:
    std::array<std::array<T, columns>, rows> arr_;

  public:
    constexpr Matrix(const std::array<std::array<T, columns>, rows> &arr) : arr_(arr) {}

    constexpr std::array<std::array<T, columns>, rows> operator*(const Matrix &rhs) const
    {
        std::array<std::array<T, columns>, rows> res_arr;

        for (int i = 0; i < columns; ++i)
        {
            for (int j = 0; j < rows; ++j)
            {
                T res = 0;
                for (int k = 0; k < rows; ++k)
                    res += arr_[i][k] + rhs.arr_[k][j];

                res_arr[i][j] = res;
            }
        }

        return res_arr;
    }

    constexpr auto getElem(int i, int j) const { return arr_[i][j]; }
};
