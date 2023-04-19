#pragma once

#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <new>
#include <type_traits>

#include "GrowingArray.hpp"

class Chunk
{
    const char *begin_;
    const char *end_;

    // Flag shows, if chunk can be deallocated
    bool   is_deallocated_ = false;
    size_t size_           = 0;

  public:
    Chunk() = default;

    Chunk(const char *begin, const char *end)
        : begin_(begin), end_(end), size_(end - begin)
    {
    }
    Chunk(const char *begin, const size_t &size)
        : begin_(begin), end_(begin + size), size_(size)
    {
    }

    template <typename T> bool is_inside(T *ptr, size_t obj_amnt = 1)
    {
        if (!size_)
            return false;
        return ptr > begin_ &&
               ((std::conditional_t<std::is_same_v<T, void>, char *, T *>)ptr +
                obj_amnt) < end_;
    }

    bool isDeallocated() { return is_deallocated_; }

    void deallocate() { is_deallocated_ = true; }
};

// We use strategy pattern for implementing allocators with different allocation
// strategies
class AllocStrategy
{
  public:
    static constexpr size_t min_size_ = 32;

  public:
    virtual void *allocate(size_t obj_amnt)              = 0;
    virtual void  deallocate(void *ptr, size_t obj_amnt) = 0;

    virtual ~AllocStrategy() = default;
};

template <typename T, template <typename> class AllocStrat> class MyOwnAllocator
{
  private:
    static_assert(!std::is_same_v<T, void>,
                  "Type of allocator must not be void (since C++20)");

    using size_t = std::size_t;

    AllocStrat<T> *strategy_;

  public:
    using value_type = T;

    MyOwnAllocator() : strategy_(new AllocStrat<T>()) {}
    template <typename U> MyOwnAllocator(const U &other);

    T *allocate(size_t obj_amnt)
    {
        return static_cast<T *>(strategy_->allocate(obj_amnt));
    }
    void deallocate(T *ptr, size_t obj_amnt = 1) { strategy_->deallocate(ptr, obj_amnt); }

    ~MyOwnAllocator() { delete strategy_; }
};

template <typename T, typename U, template <typename> class AllocStrategy>
bool operator==(const MyOwnAllocator<T, AllocStrategy> &lhs,
                const MyOwnAllocator<U, AllocStrategy> &rhs);
template <typename T, typename U, template <typename> class AllocStrategy>
bool operator!=(const MyOwnAllocator<T, AllocStrategy> &lhs,
                const MyOwnAllocator<U, AllocStrategy> &rhs)
{
    return !(lhs == rhs);
}

/// Simple implementation of stack allocation strategy
template <typename T> class StackStrategy : public AllocStrategy
{
  private:
    static constexpr size_t TYPE_SIZE = sizeof(T);

    // Capacity and size in objects
    size_t size_ = 0;
    size_t capacity_;
    // Real capacity and size in bytes
    size_t real_size_ = 0;
    size_t real_capacity;

    // Need to implement good storage for chuncks
    GrowingArray<char> data_;
    // Array of chunks inside the "main" data_ buffer
    GrowingArray<Chunk> chunks_;

  public:
    StackStrategy(size_t capacity = AllocStrategy::min_size_)
        : capacity_(capacity),
          real_capacity(capacity_ * TYPE_SIZE),
          data_(real_capacity),
          chunks_(capacity_)
    {
    }

    void *allocate(size_t obj_amnt) override;
    void  deallocate(void *ptr, size_t obj_amnt) override;

  private:
    /// As we have stack-like allocator, we must actually deallocate in LIFO model.
    /// So that method actually deallocates all deallocated chunks from the top of stack.
    void tryDeallocate()
    {
        size_t last_alive = chunks_.getSize();
        for (; last_alive >= 0; --last_alive)
            if (!chunks_[last_alive].isDeallocated())
                break;

        chunks_.cutArray(last_alive);
    }
};

template <typename T> void *StackStrategy<T>::allocate(size_t obj_amnt)
{
    auto mem   = data_.getSpaceForNElems(obj_amnt * TYPE_SIZE);
    auto chunk = chunks_.getSpaceForNElems(1);

    *chunk = {mem, obj_amnt * TYPE_SIZE};
    return mem;
}

template <typename T> void StackStrategy<T>::deallocate(void *ptr, size_t obj_amnt)
{
    size_t chunks_amnt = chunks_.getSize();
    for (size_t i = 0; i < chunks_amnt; ++i)
    {
        Chunk &chunk = chunks_[i];
        if (chunk.is_inside(ptr, obj_amnt))
            chunk.deallocate();
    }
}
