#pragma once

#include <numeric>

namespace MySTL {
auto map(auto transf_func)
{
    return [=](auto bin_op) {
        return [=](auto accum, auto elem) { return bin_op(accum, transf_func(elem)); };
    };
}

auto filter(auto predicate)
{
    return [=](auto bin_op) {
        return [=](auto accum, auto elem) {
            if (predicate(elem))
                return bin_op(accum, elem);
            else
                return accum;
        };
    };
}

template <class InputIter, class OutputIter, class UnaryOperation, class Predicate>
OutputIter transform_if(InputIter first, InputIter last, OutputIter first_out,
                        UnaryOperation unary_op, Predicate predicate)
{
    auto copy_and_next = [](auto iter, auto value) {
        *iter = value;
        ++iter;

        return iter;
    };
    return std::accumulate(first, last, first_out,
                           filter(predicate)(map(unary_op)(copy_and_next)));
}
} // namespace MySTL
