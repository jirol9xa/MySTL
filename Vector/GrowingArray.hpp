#pragma once

#include <algorithm>
#include <cstdlib>
#include <utility>

/// Simple template class for implementing dinamically growing
/// array logic. That provides only few methods and has no any
/// control over the memory. Use only under your responsibility
template <typename T> class GrowingArray
{
    static constexpr size_t RESIZE_CONSTANT = 2;
    T                      *data_;
    size_t                  size_     = 0;
    size_t                  capacity_ = 0;
    // Flag for indicating if memory, that are pointed by chunk must be deallocated
    // in chunk destructor
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

        // If the original Chunk was owning a memory, we should deallocate it
        if (is_owning_mem_)
            delete[] data_;

        is_owning_mem_ = rhs.is_owning_mem_;
        // If rhs and now our chunk are not owning a memory, we should not allocate
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
    T     *data() const { return data_; }

    /// Resizes array in "times" times up if (times > 0) &&
    /// resizes down if (times < 0)
    void resize(size_t times)
    {
        T *new_data = new T[capacity_ * times];
        for (size_t i = 0; i < size_; ++i)
            new_data[i] = std::move_if_noexcept(data_[i]);

        capacity_ *= times;
        data_ = new_data;

        if (size_ > capacity_)
            size_ = capacity_;
    }

    // Returns the pointer to the beggining memory inside GrowingArray where we can place
    // N elems
    T *getSpaceForNElems(size_t N)
    {
        if (capacity_ - size_ < N)
            resize(RESIZE_CONSTANT);

        size_t old_size = size_;
        size_ += N;

        return data_[old_size];
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
