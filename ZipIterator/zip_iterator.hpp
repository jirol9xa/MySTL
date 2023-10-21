#pragma once

#include <iterator>
#include <utility>

template <typename T> class ZipIterator {
    using iterType = typename T::iterator;

    iterType iter1_;
    iterType iter2_;

  public:
    ZipIterator(iterType iter1, iterType iter2) : iter1_(iter1), iter2_(iter2) {}

    ZipIterator &operator++()
    {
        ++iter1_;
        ++iter2_;

        return *this;
    }

    bool operator!=(const ZipIterator &other) const
    {
        return iter1_ != other.iter1_ && iter2_ != other.iter2_;
    }

    bool operator==(const ZipIterator &other) const { return !operator!=(other); }

    std::pair<typename T::value_type, typename T::value_type> operator*() const
    {
        return {*iter1_, *iter2_};
    }
};

template <typename T> struct std::iterator_traits<ZipIterator<T>> {
    using iterator_category = std::forward_iterator_tag;
    using value_type        = std::pair<typename T::value_type, typename T::value_type>;
    using difference_type   = ssize_t;
};

template <typename T> class Zipper {
    T &seq1_;
    T &seq2_;

    using ZipIterType = ZipIterator<T>;

  public:
    Zipper(T &seq1, T &seq2) : seq1_(seq1), seq2_(seq2) {}

    ZipIterType begin() const { return {std::begin(seq1_), std::begin(seq2_)}; }

    ZipIterType end() const { return {std::end(seq1_), std::end(seq2_)}; }
};
