#pragma once

#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace safePrint
{
void print(const char *str)
{
    assert(str != nullptr);

    std::cout << str;
    return;
}

template <typename T, typename U> inline constexpr bool compareTypes()
{
    return std::is_same<std::remove_cvref_t<T>, U>::value;
}

template <size_t N> inline constexpr std::string_view makeString(const char (&str)[N])
{
    return std::string_view{str, N - 1};
}

template <typename T> inline constexpr std::string_view getType()
{
    if constexpr (compareTypes<T, int>())
        return makeString("int");
    if constexpr (compareTypes<T, char>())
        return makeString("char");
    if constexpr (compareTypes<T, float>())
        return makeString("float");
    if constexpr (compareTypes<T, long>())
        return makeString("long");
    if constexpr (compareTypes<T, double>())
        return makeString("double");
    if constexpr (compareTypes<T, char *>())
        return makeString("char *");
}

template <typename T> void getErrMsg(char print_flag)
{
    constexpr auto err_msg1 = makeString("Error!!! Mismatching of argument type <");
    constexpr auto err_msg2 = makeString("> and passed flag <");
    constexpr auto type_str = getType<T>();

    std::cout << err_msg1 << type_str << err_msg2 << print_flag << ">!!!\n";
}

template <typename T, typename... ArgsT>
void print(const char *str, T &&value, ArgsT &&...args)
{
    assert(str != nullptr);

    for (char sym = *str; sym != '%' && sym != '\0'; sym = *++str)
        putchar(sym);

    if (*str == '\0')
        return;

    // Skip percent symbol and compare it with correct flags
    if constexpr (compareTypes<T, int>())
    {
        if (*++str == 'd')
        {
            std::cout << value;
        }
        else
        {
            getErrMsg<T>('d');
            return;
        }
    }
    else if constexpr (compareTypes<T, char>())
    {
        if (*++str == 'c')
        {
            std::cout << value;
        }
        else
        {
            getErrMsg<T>('c');
            return;
        }
    }
    else if constexpr (compareTypes<T, float>())
    {
        if (*++str == 'f')
        {
            std::cout << value;
        }
        else
        {
            getErrMsg<T>('f');
            return;
        }
    }
    else if constexpr (compareTypes<T, long>())
    {
        if (*++str == 'l')
        {
            std::cout << value;
        }
        else
        {
            getErrMsg<T>('l');
            return;
        }
    }
    else if constexpr (compareTypes<T, double>())
    {
        if (*++str == 'g')
        {
            std::cout << value;
        }
        else
        {
            getErrMsg<T>('g');
            return;
        }
    }
    else if constexpr (compareTypes<T, char *>())
    {
        if (*++str == 's')
        {
            std::cout << value;
        }
        else
        {
            getErrMsg<T>('s');
            return;
        }
    }
    else
    {
        std::cout << "\n!!!ERROR!!! ---> Invalid print flag <" << *++str << ">\n";
        return;
    }

    // Skip print flag
    ++str;
    if constexpr (sizeof...(ArgsT))
        print(str, std::forward<ArgsT...>(args...));
    if constexpr (sizeof...(ArgsT) == 0)
        print(str);

    return;
}
} // namespace safePrint
