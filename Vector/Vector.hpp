#pragma once

#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <new>
#include <string>
#include <utility>
#include <vector>

template <typename T> class Vector
{
  private:
    T            *data_;
    uint_fast32_t size_     = 0;
    uint_fast32_t capacity_ = 8;

  public:
    Vector();
    Vector(const int capacity);
    /// Ctor, that takes a number the number of elements and their value
    Vector(const int size, const T &elems_value);
    // Vector(std::initializer_list<T>);

    Vector(Vector &&); // Make version based on noexcept attribute of T(&&)
    Vector<T> &operator=(const Vector &);
    Vector<T> &operator=(Vector &&);

    void resize(uint_fast32_t cnt);

    T       &operator[](int_fast32_t idx) noexcept;
    const T &operator[](int_fast32_t idx) const noexcept;
    T       &at(int_fast32_t idx);
    const T &at(int_fast32_t idx) const;

    T       &front();
    const T &front() const;
    T       &back();
    const T &back() const;
};

template <typename T> Vector<T>::Vector()
{
    try
    {
        data_ = new T[capacity_];
    }
    catch (const std::bad_alloc &except)
    {
        capacity_ = 0;
        data_     = nullptr;
        // throw
    }
}

template <typename T> Vector<T>::Vector(const int capacity) : capacity_(capacity)
{
    try
    {
        data_ = new T[capacity_];
    }
    catch (const std::bad_alloc &except)
    {
        capacity_ = 0;
        data_     = nullptr;

        std::string str = "Not enough memory for creating a vector with capacity_ = " +
                          std::to_string(capacity);
        throw str;
    }
}

template <typename T>
Vector<T>::Vector(const int size, const T &elem_value) : Vector<T>(size)
{
    size_ = size;

    for (int i = 0; i < size; ++i)
        data_[i] = elem_value;

    // TODO: Implement with iterators
    //for (auto &&elem : data_)
        //elem = elem_value;
}

/*template <typename T>
Vector<T>::Vector(std::initializer_list<T> init)
{
}*/

template <typename T> Vector<T>::Vector(Vector &&that)
{
    data_     = that.data_;
    size_     = that.size_;
    capacity_ = that.capacity_;

    that.data = nullptr;
}

template <typename T> Vector<T> &Vector<T>::operator=(const Vector &that)
{
    if (&that == this)
        return *this;

    size_     = that.size_;
    capacity_ = that.capacity_;
    delete[] data_;

    try
    {
        data_ = new T[capacity_];
    }
    catch (const std::bad_alloc &except)
    {
        std::string str = "Not enough memory for copying a vector with capacity_ = " +
                          std::to_string(capacity_);
        throw str;
    }
    for (int i = 0; i < size_; ++i)
        data_[i] = that.data_[i];

    // TODO: Implement it with iterators
    // std::copy()
}

template <typename T> Vector<T> &Vector<T>::operator=(Vector &&that)
{
    if (&that == this)
        return *this;

    data_     = that.data_;
    size_     = that.size_;
    capacity_ = that.capacity_;

    that.data = nullptr;
}

template <typename T> void Vector<T>::resize(uint_fast32_t count)
{
    if (count < size_)
        throw "New capacity too small to contain all existing elems";

    capacity_  = count;
    T *new_arr = nullptr;

    try
    {
        new_arr = new T[count];
    }
    catch (std::bad_alloc)
    {
        throw "Not enough mem for resizing";
    }

    for (uint_fast32_t i = 0; i < size_; ++i)
        new_arr[i] = std::move_if_noexcept(data_[i]);

    delete[] data_;
    data_ = new_arr;
}

template <typename T> T &Vector<T>::operator[](int_fast32_t idx) noexcept
{
    return data_[idx];
}

template <typename T> const T &Vector<T>::operator[](int_fast32_t idx) const noexcept
{
    return data_[idx];
}

template <typename T> T &Vector<T>::at(int_fast32_t idx)
{
    if (idx >= size_ || idx < 0)
        throw "Idx = " + std::to_string(idx) + " out of range";

    return data_[idx];
}

template <typename T> const T &Vector<T>::at(int_fast32_t idx) const
{
    if (idx >= size_ || idx < 0)
        throw "Idx = " + std::to_string(idx) + " out of range";

    return data_[idx];
}

template <typename T> T &Vector<T>::front() {}

template <typename T> const T &Vector<T>::front() const {}

template <typename T> T &Vector<T>::back() {}

template <typename T> const T &Vector<T>::back() const {}
