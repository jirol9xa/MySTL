#include "Vector.hpp"
#include <initializer_list>
#include <iostream>

int main()
{
    constexpr std::array<int, 2> arr1 = {2, 2};
    constexpr std::array<int, 2> arr2 = {3, 3};

    constexpr Vector<int, 2> v1 = arr1;
    constexpr Vector<int, 2> v2 = arr2;
    constexpr Vector<int, 2> v3 = v1 * v2;

    std::cout << v3.arr_[0] << '\n';

    return 0;
}
