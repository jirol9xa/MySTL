#pragma once

#include <type_traits>

namespace MySTL {

template <typename... Ts> struct common_type;

template <typename... Ts> using common_type_t = typename common_type<Ts...>::type;

// Empty specialisation does nothing with type deduction
template <> struct common_type<> {
};

template <typename T> struct common_type<T> {
    using type = std::decay_t<T>;
};

template <typename T, typename U> struct common_type<T, U> {
    using type = std::decay_t<decltype(true ? std::declval<std::decay_t<T>>()
                                            : std::declval<std::decay_t<U>>())>;
};

template <typename T, typename U, typename... Ts> struct common_type<T, U, Ts...> {
    using type = common_type_t<common_type_t<T, U>, common_type_t<Ts...>>;
};

} // namespace MySTL
