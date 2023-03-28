#pragma once

/// Struct implementing factorial of value N
template <int N> struct Factorial
{
    int res;

    Factorial() : res(Factorial<N - 1>().res * N) {}
};

template <> struct Factorial<0>
{
    int res = 1;
};
