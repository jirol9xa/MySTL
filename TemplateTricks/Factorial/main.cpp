#include "Factorial.hpp"
#include <iostream>

int main()
{
    constexpr int res = Factorial<5>::res;
    std::cout << res << '\n';

    return 0;
}
