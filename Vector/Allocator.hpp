#pragma once

#include <cstddef>
#include <cstdlib>
#include <type_traits>
#include <new>
#include <iostream>

// TODO: Need to add chunk class,
// because all types of allocators neeed to know at leaast size of 
// allocated block

// We use strategy pattern for implementing allocators with different allocation
// strategies
template <size_t CHUNK_SIZE = 1024> class AllocStrategy
{
  public:
    size_t min_size_ = 32;

  public:
    int          a                                   = 10;
    virtual void *allocate(size_t obj_amnt)           = 0;
    virtual void deallocate(void *ptr, size_t obj_amnt) = 0;
};

template <typename T, typename AllocStrategy> class MyOwnAllocator
{
  private:
    static_assert(!std::is_same_v<T, void>,
                  "Type of allocator must not be void (since C++20)");

    using size_t = std::size_t;

    AllocStrategy *strategy_;

  public:
    using value_type = T;

    MyOwnAllocator();
    template <typename U> MyOwnAllocator(const U &other);

    T   *allocate(size_t obj_amnt) { return static_cast<T>(strategy_->allocate(obj_amnt)); }
    void deallocate(T *ptr, size_t obj_amnt) { strategy_->deallocate(ptr, obj_amnt); }
};

template <typename T, typename U, typename AllocStrategy>
bool operator==(const MyOwnAllocator<T, AllocStrategy> &lhs,
                const MyOwnAllocator<U, AllocStrategy> &rhs);
template <typename T, typename U, typename AllocStrategy>
bool operator!=(const MyOwnAllocator<T, AllocStrategy> &lhs,
                const MyOwnAllocator<U, AllocStrategy> &rhs)
{
    return !(lhs == rhs);
}

/// Simple implementation of stack allocation strategy
template <size_t CHUNK_SIZE = 1024> class StackStrategy : public AllocStrategy<CHUNK_SIZE>
{
  private:
    size_t size_;
    size_t capacity_ = AllocStrategy<CHUNK_SIZE>::min_size_;
    char     *data_ = nullptr;

  public:
    StackStrategy(size_t capacity) : size_(0), capacity_(capacity) {
        try {data_ = new char[capacity_];}
        catch (const std::bad_alloc &except) {
            std::cout << except.what();
            throw "StackCtor except";
        }
    }

    void   *allocate(size_t obj_amnt) override;
    void deallocate(void *ptr, size_t obj_amnt) override;

    ~StackStrategy() { delete [] data_; }
private:
    void resize(size_t new_capacity);
};

template <size_t CHUNK_SIZE>
void *StackStrategy<CHUNK_SIZE>::allocate(size_t obj_amnt) {
    if (size_ + obj_amnt > capacity_)
        resize();

    char *new_mem = data_[size_];
    size_ += obj_amnt;

    return new_mem;
}
