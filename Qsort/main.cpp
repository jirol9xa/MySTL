#include <algorithm>
#include <iostream>
#include <vector>
#include "qsort.hpp"

int main()
{
    std::vector<int> vec1(10000000);
    std::vector<int> vec2(10000000);

    do {
        for (int i = 0; i < 10000000; ++i) {
            vec1[i] = std::rand();
            vec2[i] = vec1[i];
        }

        MySTL::Qsort(vec1.begin(), vec1.end());
        std::sort(vec2.begin(), vec2.end());

    } while (MySTL::IsRangesEq(vec1.begin(), vec1.end(), vec2.begin()));

    return 0;
}
