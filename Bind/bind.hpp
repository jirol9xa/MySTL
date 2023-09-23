#pragma once

#include "args_list.hpp"
#include <type_traits>

namespace MySTL {

template <typename Func, typename... BinderArgs> class bind {
  public:
    template <typename FuncImpl, typename... BinderArgsImpl>
    bind(const FuncImpl &func, BinderArgsImpl &&...args)
        : func_(std::move(func)), args_list_{std::forward<BinderArgs>(args)...}
    {
    }

    template <typename... CallArgs> void operator()(CallArgs &&...args)
    {
        return call(make_indices<sizeof...(BinderArgs)>{},
                    std::forward<CallArgs>(args)...);
    }

  private:
    template <std::size_t... Indices, typename... Args>
    void call(const indices<Indices...> &, Args &&...args)
    {
        struct empty_list {
            empty_list(Args &&...) {}
        };

        using args_list_t =
            std::conditional_t<sizeof...(Args) == 0, empty_list, args_list<Args...>>;
        args_list_t passed_args_lst{std::forward<Args>(args)...};

        return invoker_.invoke(
            func_,
            take_arg(get_arg<Indices, BinderArgs...>(args_list_), passed_args_lst)...);
    }

    struct MemberFunctionInvoker {
        template <typename ObjType, typename... Args>
        void invoke(const Func &func, ObjType &&obj, Args &&...args)
        {
            return (obj.*func)(std::forward<Args>(args)...);
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

template <typename Func, typename... Args>
bind(const Func &, Args &&...) -> bind<Func, Args...>;

} // namespace MySTL
