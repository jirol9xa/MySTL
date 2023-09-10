#include "bind.hpp"
#include <functional>
#include <iostream>

int sum(int a, int b) { return a + b; }

int main()
{
    using namespace MySTL;
    auto sum = bind<int(int, int), int, int>(::sum, 1, 2);
    std::cout << sum() << '\n';

    // auto f = bind([](int a, int b) { return a + b; }, 1, 2);

    auto f1 = std::bind([](int a, int b) { return a - b; }, 1, 2);

    // std::cout << f() << '\n';

    std::cout << f1() << '\n';

    auto f2{std::move(f1)};

    return 0;
}
