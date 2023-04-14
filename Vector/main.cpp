#include "Vector.hpp"
#include <algorithm>

int main()
{
    Vector<int> vec = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> vec1(vec);

    std::sort(vec1.begin(), vec1.end());

    for (auto &&iter : vec1)
        std::cout << iter << ' ';

    std::cout << '\n';

    return 0;
}
