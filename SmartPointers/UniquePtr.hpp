#pragma once

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace MySTL {

/// Class for implementing unique-pointer to single object logic
template <typename T> class Unique_ptr {
  private:
    T *ptr_ = nullptr;

  public:
    // TODO: Mb use std::optional for well supporting cases with nullptr as a ptr
    constexpr Unique_ptr() = default;
    constexpr Unique_ptr(std::nullptr_t) : Unique_ptr() {}
    Unique_ptr(T *ptr) : ptr_(ptr) {}

    void reset()
    {
        delete ptr_;
        ptr_ = nullptr;
    }
    template <typename U> void reset(U *ptr)
    {
        if (ptr == ptr_)
            return;

        delete ptr_;
        ptr_ = ptr;
    }

    Unique_ptr &operator=(const Unique_ptr &&rhs) noexcept
    {
        ptr_     = rhs.ptr_;
        rhs.ptr_ = nullptr;
    }

    T *get() const { return ptr_; }
       operator bool() const { return ptr_; }

    auto operator*() const
    {
        if (!operator bool())
            throw "Can't deref a nullptr";

        return std::add_lvalue_reference_t<T>(*get());
    }
    T *operator->() const { return get(); }

    ~Unique_ptr() { delete ptr_; }
};

/// Specialisation for arrays of run-time length
template <typename T> class Unique_ptr<T[]> {
  protected:
    T *arr_ = nullptr;

  public:
    Unique_ptr() = default;
    Unique_ptr(T *arr) : arr_(arr){};

    T *get() { return arr_; }
       operator bool() { return arr_; }

    void reset()
    {
        delete[] arr_;
        arr_ = nullptr;
    }
    template <typename U> void reset(U *ptr)
    {
        delete[] arr_;
        arr_ = ptr;
    }

    T &operator[](size_t idx)
    {
        if (idx < 0)
            throw std::out_of_range("Index = " + std::to_string(idx) +
                                    " is lower, than zero.");
        if (!operator bool())
            throw "Can't deref a nullptr";

        return arr_[idx];
    }

    ~Unique_ptr() { delete[] arr_; }
};

/// make_unique implementation for single object
template <typename T, typename... Args, typename = std::enable_if_t<!std::is_array_v<T>>>
Unique_ptr<T> Make_unique(Args &&...args)
{
    return new T(std::forward<Args>(args)...);
}

/// make_unique implementation for run-time lenght arrays
template <typename T, typename = std::enable_if_t<!std::is_bounded_array_v<T>>>
Unique_ptr<T> Make_unique(size_t size)
{
    return new std::remove_extent_t<T>[size];
}

/// make-unique implementation for compile-time lenght arrays
/// function is explicitly deleted, cause we can't control life-time of stack-located
/// objects
template <typename T, typename = std::enable_if_t<std::is_bounded_array_v<T>>>
Unique_ptr<T[]> Make_unique() = delete;

} // namespace MySTL
