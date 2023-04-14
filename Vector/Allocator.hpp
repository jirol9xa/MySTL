#pragma once

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <new>
#include <type_traits>

// TODO: Need to add chunk class,
// because all types of allocators neeed to know at leaast size of
// allocated block

template <size_t CHUNK_SIZE> class Chunk
{
    static_assert(CHUNK_SIZE < sizeof(Chunk),
                  "CHUNK_SIZE must be greater than sizeof(Chunk)");

    // Flag shows, if chunk can be deallocated
    bool   is_free_ = false;
    size_t size_    = 0;

  public:
    Chunk(size_t size) : size_(size) {}

    template <typename T> bool is_inside(T *ptr)
    {
        if (!size_)
            return false;

        char *ch_this         = reinterpret_cast<char *>(this);
        char *latest_possible = ch_this + CHUNK_SIZE - sizeof(T);

        return ptr > ch_this && ptr <= latest_possible;
    }

    void *allocate()
    {
        if (!is_free_)
            return nullptr;

        is_free_ = false;
        return this + sizeof(Chunk);
    }

    void deallocate() { is_free_ = true; }
};

// We use strategy pattern for implementing allocators with different allocation
// strategies
template <size_t CHUNK_SIZE = 1024> class AllocStrategy
{
  public:
    size_t min_size_ = 32;

  public:
    int           a                                      = 10;
    virtual void *allocate(size_t obj_amnt)              = 0;
    virtual void  deallocate(void *ptr, size_t obj_amnt) = 0;
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

    T *allocate(size_t obj_amnt) { return static_cast<T>(strategy_->allocate(obj_amnt)); }
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
    // Capacity and size in objects
    size_t size_     = 0;
    size_t capacity_ = AllocStrategy<CHUNK_SIZE>::min_size_;

    // Real capacity and size in bytes
    size_t real_size_    = 0;
    size_t real_capacity = AllocStrategy<CHUNK_SIZE>::min_size_ * CHUNK_SIZE;

    // Need to implement good storage for chuncks
    char *data_ = nullptr;
    // Array of chunks inside the "main" data_ buffer
    Chunk<CHUNK_SIZE> *chunks = nullptr;

  public:
    StackStrategy(size_t capacity)
        : capacity_(capacity), real_capacity(capacity_ * CHUNK_SIZE)
    {
        try
        {
            data_ = new char[real_capacity];
        }
        catch (const std::bad_alloc &except)
        {
            std::cout << except.what();
            throw "StackCtor in StackStrategy except";
        }
    }

    void *allocate(size_t obj_amnt) override;
    void  deallocate(void *ptr, size_t obj_amnt) override;

    ~StackStrategy() { delete[] data_; }

  private:
    /// Function, that separate row memory to chunks
    void separateChunks();

    void resize(size_t new_capacity);
};

template <size_t CHUNK_SIZE> void *StackStrategy<CHUNK_SIZE>::allocate(size_t obj_amnt)
{
    if (size_ + obj_amnt > capacity_)
        resize();

    char *new_mem = data_[size_];
    size_ += obj_amnt;

    return new_mem;
}

template <size_t CHUNK_SIZE> void StackStrategy<CHUNK_SIZE>::separateChunks() {}

template <size_t CHUNK_SIZE> void StackStrategy<CHUNK_SIZE>::resize(size_t new_capacity)
{
}
