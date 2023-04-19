#pragma once

#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <new>
#include <string>
#include <utility>

template <typename T, template <typename> class Allocator>
class Vector : private Allocator<T>
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
    Vector(std::initializer_list<T>);

    Vector(const Vector &that);
    Vector(Vector &&); // Make version based on noexcept attribute of T(&&)
    Vector &operator=(const Vector &);
    Vector &operator=(Vector &&);

    /// Resizes vector up to capacity = cnt
    void resize(uint_fast32_t cnt);

    void push_back(const T &elem)
    {
        if (size_ >= capacity_)
            resize(size_ * 2);

        data_[size_++] = elem;
    }

    T       &operator[](int_fast32_t idx) noexcept;
    const T &operator[](int_fast32_t idx) const noexcept;
    T       &at(int_fast32_t idx);
    const T &at(int_fast32_t idx) const;

    T       &front();
    const T &front() const;
    T       &back();
    const T &back() const;

    ~Vector()
    {
        try
        {
            Allocator<T>::deallocate(data_);
        }
        catch (...)
        { /* need to process exception*/
        }
        // delete[] data_;
    }

    template <typename U>
    struct VecIterator : public std::iterator<std::random_access_iterator_tag, U>
    {
        U *val_;

        VecIterator(T *val) : val_(val) {}

        VecIterator(const VecIterator &that) { val_ = that.val_; }

        // clang-format off
        bool operator==(const VecIterator &that) { return val_ == that.val_; }
        bool operator!=(const VecIterator &that) { return val_ != that.val_; }
        bool operator> (const VecIterator &that) { return val_ > that.val_;  }
        bool operator< (const VecIterator &that) { return val_ < that.val_;  }
        bool operator>=(const VecIterator &that) { return val_ >= that.val_; }
        bool operator<=(const VecIterator &that) { return val_ <= that.val_; }

        typename VecIterator::reference operator*() { return *val_; }

        VecIterator operator++()    { ++val_; return *this; }
        VecIterator operator++(int) { return VecIterator(val_++); }
        VecIterator operator--()    { --val_; return *this; }
        VecIterator operator--(int) { return VecIterator(val_--); }
       
        using diff_t = typename VecIterator::difference_type;

        VecIterator &operator+=(const diff_t n);
        VecIterator &operator-=(const diff_t n);
        VecIterator  operator+ (const diff_t n);
        VecIterator  operator- (const diff_t n);

        diff_t operator- (const VecIterator &that);

        T &operator[](const int_fast32_t n);
        // clang-format on
    };

    using iterator       = VecIterator<T>;
    using const_iterator = VecIterator<const T>;

    // clang-format off
    iterator       begin()       { return       iterator(data_); }
    const_iterator begin() const { return const_iterator(data_); };
    iterator       end()         { return       iterator(data_ + size_); }
    const_iterator end()   const { return const_iterator(data_ + size_); }
    // clang-format on
};

template <typename T, template <typename> class Allocator> Vector<T, Allocator>::Vector()
{
    try
    {
        data_ = Allocator<T>::allocate(capacity_); // new T[capacity_];
    }
    catch (const std::bad_alloc &except)
    {
        capacity_ = 0;
        data_     = nullptr;
        // throw
    }
}

template <typename T, template <typename> class Allocator>
Vector<T, Allocator>::Vector(const int capacity) : capacity_(capacity)
{
    try
    {
        data_ = Allocator<T>::allocate(capacity_); // new T[capacity_];
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

template <typename T, template <typename> class Allocator>
Vector<T, Allocator>::Vector(const int size, const T &elem_value) : Vector(size)
{
    size_ = size;

    for (auto &&elem : data_)
        elem = elem_value;
}

template <typename T, template <typename> class Allocator>
Vector<T, Allocator>::Vector(std::initializer_list<T> init) : Vector(init.size())
{
    size_ = capacity_;
    std::copy(init.begin(), init.end(), data_);
}

template <typename T, template <typename> class Allocator>
Vector<T, Allocator>::Vector(const Vector &that) : Vector(that.capacity_)
{
    size_ = that.size_;
    std::copy(that.begin(), that.end(), begin());
}

template <typename T, template <typename> class Allocator>
Vector<T, Allocator>::Vector(Vector &&that)
{
    data_     = that.data_;
    size_     = that.size_;
    capacity_ = that.capacity_;

    that.data = nullptr;
}

template <typename T, template <typename> class Allocator>
Vector<T, Allocator> &Vector<T, Allocator>::operator=(const Vector &that)
{
    if (&that == this)
        return *this;

    Allocator<T>::deallocate(data_);

    size_     = that.size_;
    capacity_ = that.capacity_;
    // delete[] data_;

    try
    {
        data_ = Allocator<T>::allocate(capacity_); // new T[capacity_];
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

template <typename T, template <typename> class Allocator>
Vector<T, Allocator> &Vector<T, Allocator>::operator=(Vector &&that)
{
    if (&that == this)
        return *this;

    data_     = that.data_;
    size_     = that.size_;
    capacity_ = that.capacity_;

    that.data = nullptr;
}

template <typename T, template <typename> class Allocator>
void Vector<T, Allocator>::resize(uint_fast32_t count)
{
    if (count < size_)
        throw "New capacity too small to contain all existing elems";

    capacity_  = count;
    T *new_arr = nullptr;

    try
    {
        new_arr = Allocator<T>::allocate(count); // new T[count];
    }
    catch (std::bad_alloc)
    {
        throw "Not enough mem for resizing";
    }

    for (uint_fast32_t i = 0; i < size_; ++i)
        new_arr[i] = std::move_if_noexcept(data_[i]);

    Allocator<T>::deallocate(data_); // delete[] data_;
    data_ = new_arr;
}

template <typename T, template <typename> class Allocator>
T &Vector<T, Allocator>::operator[](int_fast32_t idx) noexcept
{
    return data_[idx];
}

template <typename T, template <typename> class Allocator>
const T &Vector<T, Allocator>::operator[](int_fast32_t idx) const noexcept
{
    return data_[idx];
}

template <typename T, template <typename> class Allocator>
T &Vector<T, Allocator>::at(int_fast32_t idx)
{
    if (idx >= size_ || idx < 0)
        throw "Idx = " + std::to_string(idx) + " out of range";

    return data_[idx];
}

template <typename T, template <typename> class Allocator>
const T &Vector<T, Allocator>::at(int_fast32_t idx) const
{
    if (idx >= size_ || idx < 0)
        throw "Idx = " + std::to_string(idx) + " out of range";

    return data_[idx];
}

template <typename T, template <typename> class Allocator>
T &Vector<T, Allocator>::front()
{
    if (size_ == 0)
        throw "Can't take front elem from empty vector";

    return *data_;
}

template <typename T, template <typename> class Allocator>
const T &Vector<T, Allocator>::front() const
{
}

template <typename T, template <typename> class Allocator> T &Vector<T, Allocator>::back()
{
}

template <typename T, template <typename> class Allocator>
const T &Vector<T, Allocator>::back() const
{
}

template <typename T, template <typename> class Allocator>
template <typename U>
auto Vector<T, Allocator>::VecIterator<U>::operator+=(const diff_t n) -> VecIterator<U> &
{
    data_ += n;
    return *this;
}

template <typename T, template <typename> class Allocator>
template <typename U>
auto Vector<T, Allocator>::VecIterator<U>::operator-=(const diff_t n) -> VecIterator<U> &
{
    data_ -= n;
    return *this;
}

template <typename T, template <typename> class Allocator>
template <typename U>
auto Vector<T, Allocator>::VecIterator<U>::operator+(const diff_t n) -> VecIterator<U>
{
    return VecIterator(val_ + n);
}

template <typename T, template <typename> class Allocator>
template <typename U>
auto Vector<T, Allocator>::VecIterator<U>::operator-(const diff_t n) -> VecIterator<U>
{
    return VecIterator(val_ - n);
}

template <typename T, template <typename> class Allocator>
template <typename U>
auto Vector<T, Allocator>::VecIterator<U>::operator-(const VecIterator &that) -> diff_t
{
    return val_ - that.val_;
}
