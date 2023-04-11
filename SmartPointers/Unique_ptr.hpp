#pragma once

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

// For supporting user-defined destructors
#include <functional>

/// Class for implementing unique-pointer logic
template <typename T> class Unique_ptr
{
  private:
    T *ptr_ = nullptr;

  public:
    Unique_ptr(T *ptr) : ptr_(ptr) {}

    ~Unique_ptr() { delete ptr_; }
};

/// Specialisation for arrays of run-time length
template <typename T> class Unique_ptr<T[]>
{
  protected:
    T *arr_ = nullptr;

  public:
    Unique_ptr(T *arr) : arr_(arr){};

    T &operator[](size_t idx)
    {
        if (idx < 0)
            throw std::out_of_range("Index = " + std::to_string(idx) +
                                    " is lower, than zero.");

        return arr_[idx];
    }

    ~Unique_ptr() { delete[] arr_; }
};

/// Specialisations for arrays of compile-time length
template <typename T, size_t N> class Unique_ptr<T[N]> : public Unique_ptr<T[]>
{
  public:
    Unique_ptr(T *arr) : Unique_ptr<T[]>(arr) {}

    T &operator[](size_t idx)
    {
        if (idx >= N || idx < 0)
            throw std::out_of_range("Index = " + std::to_string(idx) + " out of range.");

        return this->arr_[idx];
    }
};

/// make_unique implementation for single object
template <typename T, typename... Args, typename = std::enable_if_t<!std::is_array_v<T>>>
Unique_ptr<T> Make_unique(Args &&...args)
{
    return new T(std::forward<Args>(args)...);
}

/// make_unique implementation for run-time lenght arrays
template <typename T, typename = std::enable_if_t<!std::is_bounded_array_v<T>>>
Unique_ptr<std::remove_extent_t<T>[]> Make_unique(size_t size)
{ return new std::remove_extent_t<T>[size]; }

/// make-unique implementation for compile-time lenght arrays
/// function is explicitly deleted, cause we can't control life-time of stack-located
/// objects
template <typename T, typename = std::enable_if_t<std::is_bounded_array_v<T>>>
Unique_ptr<T[]> Make_unique() = delete;
