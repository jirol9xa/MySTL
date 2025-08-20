#include <atomic>
#include <vector>
#include <cstddef>
#include <new>

namespace Concurrent {

template <typename T>
class SPSCRingBuffer {
    struct Slot {
        alignas(64) T value;
    };

public:
    explicit SPSCRingBuffer(const size_t capacity) : buffer_(capacity + 1) {}

    bool TryProduce(T value) {
        const size_t curr_tail = tail_.load(std::memory_order_relaxed);

        // if tail reached old cached value of head
        if (IsFull(cached_head_, curr_tail)) {
            cached_head_ = head_.load(std::memory_order_acquire);
        }
        const size_t curr_head = cached_head_;

        if (IsFull(curr_head, curr_tail)) {
            return false;
        }

        buffer_[curr_tail] = std::move(value);
        tail_.store(Next(curr_tail), std::memory_order_release);
        return true;
    }

    bool TryConsume(T& value) {
        const size_t curr_head = head_.load(std::memory_order_relaxed);

        // if head reached old cached value of tail
        if (IsEmpty(curr_head, cached_tail_)) {
            cached_tail_ = tail_.load(std::memory_order_acquire);
        }
        const size_t curr_tail = cached_tail_;

        if (IsEmpty(curr_head, curr_tail)) {
            return false;
        }

        value = std::move(buffer_[curr_head]);
        head_.store(Next(curr_head), std::memory_order_release);
        return true;
    }

private:
    bool IsFull(const size_t head, const size_t tail) const {
        return Next(tail) == head;
    }

    bool IsEmpty(const size_t head, const size_t tail) const {
        return head == tail;
    }

    size_t Next(const size_t idx) const {
        return (idx + 1) % buffer_.size();
    }

private:
    std::vector<Slot> buffer_;

    char padding1[std::hardware_destructive_interference_size];

    std::atomic<size_t> head_{0};
    size_t cached_head_{0};

    char padding2[std::hardware_destructive_interference_size];

    std::atomic<size_t> tail_{0};
    size_t cached_tail_{0};
};

} // namespace Concurrent
