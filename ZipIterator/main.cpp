#include "zip_iterator.hpp"
#include <array>
#include <iostream>
#include <numeric>

int main()
{
    using namespace MySTL;

    std::array<int, 10> arr1, arr2;
    std::iota(std::begin(arr1), std::end(arr1), 1);
    std::iota(std::begin(arr2), std::end(arr2), 10);

    std::ostream_iterator<int> os{std::cout, " "};
    std::puts("Arr1:");
    std::copy(std::begin(arr1), std::end(arr1), os);
    std::puts("\nArr2:");
    std::copy(std::begin(arr2), std::end(arr2), os);

    // Now let's calculate scalar multiplication of arr1 and arr2
    Zipper zipper(arr1, arr2);
    std::cout << "\nres = "
              << std::accumulate(std::begin(zipper), std::end(zipper), 0,
                                 [](auto sum, const auto &pair) {
                                     return sum + pair.first * pair.second;
                                 })
              << '\n';

    return 0;
}
