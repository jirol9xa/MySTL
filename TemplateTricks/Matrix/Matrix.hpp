#pragma once

#include <cstdint>

template <typename T, int columns, int rows, int... Elems>
class Matrix
{
private:
    int columns_ = columns;
    int rows_ = rows;

    enum {  } ;
};
