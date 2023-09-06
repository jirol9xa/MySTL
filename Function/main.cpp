#include "functionANAL.hpp"
#include <iostream>

int sum10(int a, int b) { return (a + b) * 10; }
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

    std::function<int(int, int)> a = sum10;
    std::cout << a(10, 10) << '\n';

    a = sum;
    std::cout << a(10, 10) << '\n';

    std::function<int(int, int)> b = sum10;
    a                              = b;

    std::cout << a(10, 10) << '\n';

    sum = sub;

    std::cout << sum(25, 5) << '\n';

    sum = [](int a, int b) { return a * b; };

    std::cout << sum(25, 5) << '\n';

    return 0;
}
