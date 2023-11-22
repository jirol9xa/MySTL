#include "transform_if.hpp"
#include <iostream>
#include <iterator>

int main()
{
    int arr[5];
    std::iota(std::begin(arr), std::end(arr), 1);

    auto twice          = [](auto value) { return value * 2; };
    auto even           = [](auto value) { return value % 2 == 0; };
    int  transformed[5] = {};

    MySTL::transform_if(std::begin(arr), std::end(arr), std::begin(transformed), twice,
                        even);

    auto printer = [](auto begin, auto end) {
        copy(begin, end, std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;
    };

    printer(std::begin(arr), std::end(arr));
    printer(std::begin(transformed), std::end(transformed));

    return 0;
}
