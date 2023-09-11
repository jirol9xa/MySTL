#pragma once

#include "args_list.hpp"
#include <iostream>
#include <memory>
#include <type_traits>

namespace MySTL {

template <typename Func, typename... BinderArgs> class bind {
  public:
    // TODO: Add perfect forwarding for Func also (need to add possobility for
    // containing Func&)
    bind(Func func, BinderArgs &&...args)
        : func_(std::move(func)), args_list_(std::forward<BinderArgs>(args)...)
    {
        call(make_indices<10>{}, std::forward<BinderArgs>(args)...);
    }

    template <typename... CallArgs> void operator()(CallArgs &&...args)
    {
        // return call(std::forward<CallArgs>(args)...);
    }

  private:
    template <std::size_t... Indices, typename... Args>
    void call(const indices<Indices...> &, Args &&...args)
    {
        struct empty_list {
            empty_list(Args &&...args) {}
        };

        using args_list_t =
            std::conditional_t<sizeof...(Args) == 0, empty_list, args_list<Args...>>;
        args_list_t args_lst{std::forward<Args>(args)...};

        return invoker_.invoke(func_, std::forward<BinderArgs>(args)...);
    }

    struct MemberFunctionInvoker {
        template <typename ObjType>
        void invoke(const Func &func, ObjType &&obj, BinderArgs &&...args)
        {
            return (obj.func)(std::forward<BinderArgs>(args)...);
        }
    };

    struct FreeFunctionInvoker {
        void invoke(const Func &func, BinderArgs &&...args)
        {
            return func(std::forward<BinderArgs>(args)...);
        }
    };

    using invoker_t = std::conditional_t<std::is_member_function_pointer_v<Func>,
                                         MemberFunctionInvoker, FreeFunctionInvoker>;

    Func                     func_;
    args_list<BinderArgs...> args_list_;
    invoker_t                invoker_;
};
} // namespace MySTL
