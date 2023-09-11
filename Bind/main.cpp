#include "bind.hpp"
#include <functional>
#include <iostream>

void sum(int a, int b) { std::cout << a + b << '\n'; }

struct Test {
    void sum(int a, int b) { std::cout << a + b << '\n'; }
};

int main()
{
    using namespace MySTL;
    auto sum = bind(&::sum, 1, 2);
    sum();

    Test test;
    auto sum2 = MySTL::bind(&Test::sum, test, std::placeholders::_1, 2);
    sum2(1);

    // auto f = bind([](int a, int b) { return a + b; }, 1, 2);

    auto f1 = std::bind([](int a, int b) { return a - b; }, 1, 2);

    // std::cout << f() << '\n';

    std::cout << f1() << '\n';

    auto f2{std::move(f1)};

    return 0;
}
