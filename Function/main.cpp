#include "functionANAL.hpp"
#include <iostream>

int sum(int a, int b) { return a + b; }

int main()
{
    using MySTL::function;

    function<int(int, int)> sum{::sum};
    function<int(int, int)> sub{[](int a, int b) { return a - b; }};

    std::cout << sum(4, 9) << '\n';
    std::cout << sub(25, 5) << '\n';

    return 0;
}
