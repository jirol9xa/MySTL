#include "Vector.hpp"

int main()
{
    Vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int> vec1(vec);    

    for (auto &&iter : vec1)
        std::cout << iter << ' ';

    std::cout << '\n';

    return 0;
}
