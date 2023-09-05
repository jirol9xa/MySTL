#pragma once

#include <iostream>

namespace MySTL {
template <typename IterTy>
constexpr void Advance(IterTy &iter, size_t dist) noexcept(noexcept(++iter))
{
    for (size_t i = 0; i < dist; ++i, ++iter)
        continue;
}

template <typename IterTy>
constexpr void Subtraction(IterTy &iter, size_t dist) noexcept(noexcept(--iter))
{
    using category = typename std::iterator_traits<IterTy>::iterator_category;
    static_assert(std::is_base_of_v<std::bidirectional_iterator_tag, category>);

    for (size_t i = 0; i < dist; ++i, --iter)
        continue;
}

template <typename IterTy>
constexpr IterTy Next(IterTy iter) noexcept(noexcept(Advance(iter, 1)))
{
    Advance(iter, 1);
    return iter;
}

template <typename IterTy>
constexpr IterTy Prev(IterTy iter) noexcept(noexcept(Subtraction(iter, 1)))
{
    Subtraction(iter, 1);
    return iter;
}

template <typename ForwardIt1, typename ForwardIt2>
constexpr void IterSwap(ForwardIt1 first,
                        ForwardIt2 second) noexcept(noexcept(std::swap(*first, *second)))
{
    std::swap(*first, *second);
}

template <typename InputIter> void RangePrinter(InputIter begin, InputIter end)
{
    if (MySTL::Next(begin) == end)
        return;

    for (auto iter = begin; iter != end; ++iter)
        std::cout << *iter << ' ';
    std::cout << '\n';
}

template <typename InputIter>
bool IsRangesEq(InputIter begin1, InputIter end1, InputIter begin2)
{
    for (auto iter = begin1; iter != end1; ++iter, ++begin2) {
        if (*iter != *begin2)
            return false;
    }

    return true;
}

template <typename RandomIter> void Qsort(RandomIter begin, RandomIter end)
{
    if (MySTL::Next(begin) == end)
        return;

    auto left = begin, right = MySTL::Prev(end);
    auto pivot = *left;

    while (right >= left) {
        while (*left < pivot)
            ++left;
        while (*right > pivot)
            --right;

        if (left <= right)
            MySTL::IterSwap(left++, right--);
    }

    if (begin < right)
        Qsort(begin, ++right);
    if (MySTL::Prev(end) > left)
        Qsort(left, end);
}

} // namespace MySTL
