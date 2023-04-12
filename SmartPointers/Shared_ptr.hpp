#pragma once

#include <atomic>
#include <cstdlib>

/// Class for implementing shared pointer to single object logic
/// arrays are supported since C++17, but I still live in 2007
template <typename T> class Shared_ptr
{
    /// Struct, that implement control block
    struct ControlBlock
    {
        std::atomic<size_t> reference_cnt = 0;
        // For Weak_ptr
        std::atomic<size_t> shadow_cnt = 0;

        void *data_ = nullptr;
    };

    template <typename U, typename... Args>
    friend Shared_ptr<U> Make_shared(Args &&...args);

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

    void inc_ref_cnt()
    {
        if (ctrl_block_)
            ctrl_block_->reference_cnt++;
    }

    void dec_ref_cnt()
    {
        if (ctrl_block_)
            ctrl_block_->reference_cnt--;
    }

    // Only Make_shared can use that ctor
    explicit Shared_ptr(ControlBlock *ctrl_block)
        : mk_shr_ctred(true), ctrl_block_(ctrl_block)
    {
    }

  public:
    constexpr Shared_ptr() = default;
    constexpr Shared_ptr(std::nullptr_t) : Shared_ptr() {}
    Shared_ptr(T *ptr);

    Shared_ptr(const Shared_ptr &rhs)
        : mk_shr_ctred(rhs.mk_shr_ctred), ctrl_block_(rhs.ctrl_block_)
    {
        inc_ref_cnt();
    }
    Shared_ptr &operator=(const Shared_ptr &rhs)
    {
        dec_ref_cnt();

        ctrl_block_ = rhs.ctrl_block_;
        inc_ref_cnt();
        mk_shr_ctred = rhs.mk_shr_ctred;
    }

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

    ~Shared_ptr()
    {
        if (!ctrl_block_)
            return;

        ctrl_block_->reference_cnt--;
        if (ctrl_block_->reference_cnt)
            return;

        // If we made original object with Make_share() we have
        // control block and data block in one, so we must not delete
        // ControlBlock if we have Weak_ptr, that refers to that block
        if (mk_shr_ctred)
        {
            if (!ctrl_block_->shadow_cnt)
            {
                // We use delete [], because ctrl_block_ is pointer to the array,
                // that was created in Make_share
                // Explicit destructor for object called, because placement syntax
                // doesn't provide "placement delete"
                ~(*static_cast<T *>(ctrl_block_ + sizeof(ControlBlock)))();
                delete[] ctrl_block_;
                return;
            }
        }

        delete static_cast<T *>(ctrl_block_->data_);

        // If we have non-zero counter for weak_ptr, we must not delete control block
        if (!ctrl_block_->shadow_cnt)
            delete ctrl_block_;
    }
};

template <typename T, typename... Args> Shared_ptr<T> Make_shared(Args &&...args)
{
    using ControlBlock = typename Shared_ptr<T>::ControlBlock;

    // We allocate single block, that consists of ControlBlock and shared object

    uint8_t *mem_block = new uint8_t[sizeof(ControlBlock) + sizeof(T)];

    ControlBlock *ctrl_block =
        new (mem_block) ControlBlock; // Possible exception must be caught by user
    ctrl_block->data_ =
        new (mem_block + sizeof(ControlBlock)) T(std::forward<Args>(args)...);
    ctrl_block->reference_cnt = 1;

    return Shared_ptr<T>(ctrl_block);
}
