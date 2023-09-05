#include "functionANAL.hpp"
#include <iostream>

int sum(int a, int b) { return a + b; }

struct MyStruct {
    void printBebra(void) { std::cout << "Bebra\n"; }
};

int main()
{
    using MySTL::function;

    function<int(int, int)> sum{::sum};
    function<int(int, int)> sub{[](int a, int b) { return a - b; }};

    std::cout << sum(4, 9) << '\n';
    std::cout << sub(25, 5) << '\n';

    function<int(int, int)> sum_copy = sum;
    std::cout << sum_copy(4, 9) << '\n';

    MyStruct             str;
    function<void(void)> print{[&]() { str.printBebra(); }};
    print();

    return 0;
}
