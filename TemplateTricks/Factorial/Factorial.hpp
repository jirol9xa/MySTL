#pragma once

/// Struct implementing factorial of value N
template <int N> struct Factorial
{
    enum {res = N * Factorial<N - 1>::res};
};

template <> struct Factorial<0>
{
    enum {res = 1};
};
