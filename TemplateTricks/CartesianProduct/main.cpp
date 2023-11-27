#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main()
{
    auto cartesian([](auto func, auto first, auto... rest) constexpr {
        (void)std::initializer_list<int>{
            ((first < rest) ? ((void)func(first, rest), 0) : 0)...};
    });
    auto call_cart([=](auto... params) constexpr {
        return [=](auto func) constexpr
        {
            (void)std::initializer_list<int>{
                ((void)cartesian(func, params, params...), 0)...};
        };
    });

    auto print([](auto first, auto second) {
        std::cout << '(' << first << ", " << second << ") ";
    });
    // call_cart(1, 2, 3, 4, 5, 6)(print);
    // std::cout << std::endl;

    constinit thread_local std::vector<int> vec = {};
    // clang-format off
    auto transform_and_push([&](auto func) constexpr {
        return [=](auto first, auto second) constexpr 
        {
            vec.push_back(func(first, second));
        };
    });
    // clang-format on
    auto twise([](auto first, auto second) constexpr { return first * 2 + second * 2; });

    call_cart(1, 2, 3, 4, 5, 6)(transform_and_push(twise));
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}
