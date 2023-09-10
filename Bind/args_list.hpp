#pragma once

#include <cstdlib>
#include <utility>

namespace MySTL {
template <std::size_t... Indices> struct indices {
    using type = indices<Indices...>;
};

template <std::size_t I, typename Indices> struct append_indices;

template <std::size_t I, std::size_t... Indices>
struct append_indices<I, indices<Indices...>> : indices<Indices..., I> {
};

template <std::size_t N>
struct make_indices : append_indices<N - 1, typename make_indices<N - 1>::type> {
};

template <> struct make_indices<1> : indices<0> {
};

/// That struct provide type of arg on I-th place
template <std::size_t I, typename Head, typename... Tail> struct type_at_index {
    using type = typename type_at_index<I - 1, Tail...>::type;
};

template <typename Head, typename... Tail> struct type_at_index<0, Head, Tail...> {
    using type = Head;
};

template <std::size_t I, typename... Args>
using type_at_index_t = typename type_at_index<I, Args...>::type;

template <std::size_t I, typename T> struct arg {
    explicit arg(T val) : val_(std::move(val)) {}

    const T val_;
};

/// Struct for conataning pairs <idx like obj, value of arg>
/// We use idx for making differents between args of the same type
template <typename, typename...> struct args_list_impl;

template <std::size_t... Indices, typename... Args>
struct args_list_impl<indices<Indices...>, Args...> : arg<Indices, Args>... {
    template <typename... OtherArgs>
    explicit args_list_impl(OtherArgs &&...args)
        : arg<Indices, Args>(std::forward<Args>(args))...
    {
    }
};

template <typename... Args>
struct args_list : args_list_impl<typename make_indices<sizeof...(Args)>::type, Args...> {
    using list_impl_t =
        args_list_impl<typename make_indices<sizeof...(Args)>::type, Args...>;

    template <typename... OtherArgs>
    args_list(OtherArgs &&...args) : list_impl_t(std::forward<OtherArgs>(args)...)
    {
    }
};

template <std::size_t I, typename... Args>
type_at_index_t<I, Args...> take_arg_at_index(args_list<Args...> &&args)
{
    return static_cast<arg<I, Args...>>(args).val_;
}

} // namespace MySTL
