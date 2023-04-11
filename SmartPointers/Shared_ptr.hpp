#pragma once

#include <cstdlib>

/// Struct, that implement control block
struct ControlBlock
{
    size_t reference_cnt;
    // For Weak_ptr
    size_t shadow_cnt;

    void *data_;
};

/// Class for implementing shared pointer to single object logic
/// arrays are supported since C++17, but I still live in 2007
template <typename T> class Shared_ptr
{
  private:
    // Flag, that pointer was created with Make_shared
    // if it is true, we can simply calculate address of pointer data
    // ActualDataAddr = &ControlBlock + sizeof(ControlBlock)
    bool mk_shr_ctred = false;

    ControlBlock *ctrl_block_ = nullptr;

    /// Method, that can use fact of locality, if we created an original object
    /// with Make_shared func. Using it is dangerous, because of null-defined objects
    T *fast_get()
    {
        if (mk_shr_ctred)
            return reinterpret_cast<u_int8_t *>(ctrl_block_) + sizeof(ControlBlock);

        return ctrl_block_->data_;
    }

  public:
    constexpr Shared_ptr() = default;
    constexpr Shared_ptr(std::nullptr_t) : Shared_ptr() {}
    Shared_ptr(T *ptr);

    Shared_ptr(const Shared_ptr &rhs);
    Shared_ptr &operator=(const Shared_ptr &rhs);

    operator bool()
    {
        if (!ctrl_block_)
            return false;

        return ctrl_block_->data_;
    }

    T *get() const
    {
        if (!ctrl_block_)
            throw "Can't get value of null-init Shared_ptr";

        return fast_get();
    }

    T &operator*()
    {
        if (!operator bool())
            throw "Can't deref a nullptr";

        return *fast_get();
    }

    void                       reset();
    template <typename U> void reset(U *ptr);
};
