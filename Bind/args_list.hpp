#pragma once

#include <functional>
#include <type_traits>

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
    explicit arg(T val) : val_(val) {}

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
type_at_index_t<I, Args...> get_arg(const args_list<Args...> &args)
{
    return static_cast<arg<I, type_at_index_t<I, Args...>>>(args).val_;
}

template <typename T, typename U> T take_arg(const T &single_arg, const U &)
{
    return single_arg;
}

template <typename T, typename... Args, std::size_t I = std::is_placeholder_v<T>,
          typename = typename std::enable_if_t<std::is_placeholder_v<T>>>
type_at_index_t<I - 1, Args...> take_arg(const T &, const args_list<Args...> &lst)
{
    return get_arg<I - 1, Args...>(lst);
}

} // namespace MySTL
