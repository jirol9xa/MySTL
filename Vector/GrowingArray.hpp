#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <utility>

namespace MySTL {

/// Simple template class for implementing dinamically growing
/// array logic. That provides only few methods and has no any
/// control over the memory. Use only under your responsibility
///
/// We have two ways to use it:
/// 1) We have big mem buffer and want, that GrowingArray simulate interface of
/// simple array with resizing, but only inside that mem buff, so memory will
/// not be deallocated in destructur
/// 2) We delegate all actions with memory to GrowingArray, so memory will be
/// deallocated in destructor
template <typename T> class GrowingArray
{
    static constexpr size_t RESIZE_CONSTANT = 2;

    T     *data_;
    size_t size_     = 0;
    size_t capacity_ = 0;

    // Flag for indicating if memory, that are pointed by GrowingArray must be
    // deallocated in destructor
    bool is_owning_mem_ = false;

  public:
    GrowingArray(size_t capacity) : capacity_(capacity), is_owning_mem_(true)
    {
        if (capacity_)
            data_ = new T[capacity_];
        else
            data_ = nullptr;
    }

    GrowingArray(const GrowingArray &other)
        : size_(other.size_),
          capacity_(other.capacity_),
          is_owning_mem_(other.is_owning_mem_)
    {
        if (!is_owning_mem_)
        {
            data_ = other.data_;
            return;
        }

        data_ = new T[capacity_];
        std::copy(other.data_, other.data_ + other.size_, data_);
    }
    GrowingArray(GrowingArray &&other)
        : data_(other.data_),
          size_(other.size_),
          capacity_(other.capacity_),
          is_owning_mem_(other.is_owning_mem_)
    {
        other.data_ = nullptr;
    }

    GrowingArray &operator=(const GrowingArray &rhs)
    {
        if (this == &rhs)
            return *this;

        size_     = rhs.size_;
        capacity_ = rhs.capacity_;

        // If the original object was owning a memory, we should deallocate it
        if (is_owning_mem_)
            delete[] data_;

        is_owning_mem_ = rhs.is_owning_mem_;
        // If rhs and now our object are not owning a memory, we should not allocate
        // anything, just copy a pointer
        if (!is_owning_mem_)
        {
            data_ = rhs.data_;
            return *this;
        }

        data_ = new T[capacity_];
        std::copy(rhs.data_, rhs.data_ + rhs.size_, data_);

        return *this;
    }
    GrowingArray &operator=(GrowingArray &&rhs)
    {
        if (this == &rhs)
            return *this;

        size_     = rhs.size_;
        capacity_ = rhs.capacity_;

        // Please see Chunk destructor to understand what is happening
        std::swap(is_owning_mem_, rhs.is_owning_mem_);
        std::swap(data_, rhs.data_);

        return *this;
    }

    size_t getSize() const { return size_; }
    size_t getCapacity() const { return capacity_; }
    T     *getData() const { return data_; }

    /// Resizes array in "times" times up if (times > 0) &&
    /// resizes down if (times < 0)
    void resize(size_t times)
    {
        if (!is_owning_mem_)
            throw "Can't resize GrowingArray mem, because GrowingArray does not own it";

        T *new_data = new T[capacity_ * times];
        for (size_t i = 0; i < size_; ++i)
            new_data[i] = std::move_if_noexcept(data_[i]);

        delete[] data_;

        capacity_ *= times;
        data_ = new_data;

        if (size_ > capacity_)
            size_ = capacity_;
    }

    /// Cuts the array until new_size. !!! new_size should be less or equal current size_
    void cutArray(const size_t &new_size)
    {
        if (new_size > size_)
            throw "Can't cut array to new_size, that is greater, than actual";

        size_ = new_size;
        cutArrayUntilSize();
    }

    void cutArrayUntilSize()
    {
        if (!is_owning_mem_)
            throw "Can't cut GrowingArray, because GrowingArray does not own it";

        if (size_ == capacity_)
            return;

        T *new_data = new T[size_];
        for (size_t i = 0; i < size_; ++i)
            new_data[i] = std::move_if_noexcept(data_[i]);

        delete[] data_;

        capacity_ = size_;
        data_     = new_data;
    }

    /// Returns the pointer to the beggining memory inside GrowingArray where we can place
    /// N elems
    T *getSpaceForNElems(size_t N)
    {
        if (capacity_ - size_ < N)
        {
            if (is_owning_mem_)
                resize(RESIZE_CONSTANT);
            else
                throw "Not enough mem in chunk ownership";
        }

        size_t old_size = size_;
        size_ += N;

        return data_ + old_size;
    }

    T &operator[](size_t idx)
    {
        if (idx < 0 || idx >= size_)
            throw "Index out of range";

        return data_[idx];
    }

    ~GrowingArray()
    {
        if (is_owning_mem_)
            delete[] data_;
    }
};

}
