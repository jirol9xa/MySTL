#include "Allocator.hpp"
#include "Vector.hpp"

#include <iostream>

using namespace MySTL;

template <typename T> using StackAlloc = MyOwnAllocator<T, StackStrategy>;

int main()
{
    Vector<int, StackAlloc> vec = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Vector<int, StackAlloc> vec1(vec);

    Vector<int, StackAlloc> vec2;
    for (int i = 0; i < 5; ++i)
    {
        vec2.push_back(10);
        std::cout << "addr = " << &vec2[i] << ", elem = " << vec2[i] << '\n';
    }

    return 0;
}
