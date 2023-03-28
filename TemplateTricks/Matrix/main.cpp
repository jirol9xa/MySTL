#include "Matrix.hpp"
#include <array>
#include <iostream>

int main()
{
    constexpr std::array<int, 2>                line  = {1, 2};
    constexpr std::array<int, 2>                line2 = {3, 4};
    constexpr std::array<std::array<int, 2>, 2> table{line, line};
    constexpr std::array<std::array<int, 2>, 2> table2{line2, line2};
    constexpr Matrix<int, 2, 2>                 m1(table);
    constexpr Matrix<int, 2, 2>                 m2(table2);

    constexpr Matrix<int, 2, 2> res = m1 * m2;
    std::cout << res.getElem(0, 0) << '\n';

    return 0;
}
