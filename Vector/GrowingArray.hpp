#pragma once

#include <cstdlib>

/// Simple template class for implementing dinamically growing
/// array logic. That provides only few methods and has no any
/// control over the memory. Use only under your responsibility
template <typename T> class GrowingArray
{
    T     *data_;
    size_t size_     = 0;
    size_t capacity_ = 0;

  public:
    GrowingArray(size_t capacity) : capacity_(capacity)
    {
        if (capacity_)
            data_ = new T[capacity_];
        else
            data_ = nullptr;
    }

    size_t getSize() { return size_; }
    T     *data() { return data_; }

    /// Resizes array in "times" times up if (times > 0) &&
    /// resizes down if (times < 0)
    void resize(size_t times);
};
