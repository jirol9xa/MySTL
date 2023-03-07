#pragma once

#include<cstdint>

template <typename T>
class Vector
{
private:
    T *data_;
    uint_fast32_t size_;
    uint_fast32_t capacity_;

public:
    Vector();
    Vector(Vector &&)
};
