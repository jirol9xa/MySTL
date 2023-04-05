#pragma once

#include <cstdlib>

template <typename T, typename AllocStrategy> class MyOwnAllocator
{
    using value_type = T;

    MyOwnAllocator();
    template <typename U> MyOwnAllocator(const U &other);

    T   *allocate(std::size_t obj_amnt);
    void deallocate(T *ptr, std::size_t obj_amnt);
};

template <typename T, typename U, typename AllocStrategy>
bool operator==(const MyOwnAllocator<T, AllocStrategy> &lhs,
                const MyOwnAllocator<U, AllocStrategy> &rhs);
template <typename T, typename U, typename AllocStrategy>
bool operator!=(const MyOwnAllocator<T, AllocStrategy> &lhs,
                const MyOwnAllocator<U, AllocStrategy> &rhs)
{
    return !(lhs == rhs);
}
