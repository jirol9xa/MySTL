#include "Vector.hpp"

int main()
{
    Vector<int> vec(10, 0);
    for (int i = 0; i < 10; ++i)
        std::cout << vec[i] << ' ';

    std::cout << '\n';

    return 0;
}
