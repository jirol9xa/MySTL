#pragma once

#include<cstdint>
#include <initializer_list>
#include <new>
#include <utility>

template <typename T>
class Vector
{
private:
    T *data_;
    uint_fast32_t size_;
    uint_fast32_t capacity_;

public:
    Vector();
    Vector(Vector &&); // Make version based on noexcept attribute of T(&&)
    Vector(const int &size);
    /// Ctor, that takes a number the number of elements and their value
    Vector(const int &size, const T &elems_value);
    Vector(std::initializer_list<T> init);

    void resize(uint_fast32_t cnt);
};

template <typename T>
void Vector<T>::resize(uint_fast32_t count)
{
    if (count < size_)
        throw "New capacity too small to contain all existing elems";

    capacity_ = count;
    T *new_arr = nullptr;

    try{new_arr = new T[count];}
    catch(std::bad_alloc) { throw "Not enough mem for resizing"; }

    for (uint_fast32_t i = 0; i < size_; ++i)
        new_arr[i] = std::move_if_noexcept(data_[i]);
   

    delete [] data_;
    data_ = new_arr;
}
