#pragma once

#include "args_list.hpp"
#include <memory>
#include <type_traits>

namespace MySTL {

template <typename Func, typename... Args> class bind;

template <typename Ret, typename... Param, typename... Args>
class bind<Ret(Param...), Args...> {
  public:
    // TODO: Add perfect forwarding for Func also (need to add possobility for
    // containing Func&)
    template <typename Callable>
    bind(Callable func, Args &&...args)
        : func_p_(std::make_unique<CallableImpl<Callable>>(std::move(func))),
          args_list_(std::forward<Args>(args)...)
    {
    }

    template <typename... CallArgs> Ret operator()(CallArgs &&...args)
    {
        return func_p_->call(std::forward<CallArgs>(args)...);
    }

  private:
    struct CallableInterface {
        virtual Ret call(Args &&...args) = 0;
        virtual ~CallableInterface()     = default;
    };

    template <typename Callable> struct CallableImpl : CallableInterface {
        CallableImpl(Callable func) : invoker_(std::move(func)) {}

        Ret call(Args &&...args) override
        {
            return invoker_.invoke(std::forward<Args>(args)...);
        }

        struct InvokerBase {
            InvokerBase(Callable func) : func_(std::move(func)) {}

            Callable func_;

            ~InvokerBase() = default;
        };

        struct MemberFunctionInvoker : InvokerBase {
            MemberFunctionInvoker(Callable func) : InvokerBase(func) {}

            template <typename ObjType> Ret invoke(ObjType &&obj, Args &&...args)
            {
                return (obj.f)(std::forward<Args>(args)...);
            }
        };

        struct FreeFunctionInvoker : InvokerBase {
            FreeFunctionInvoker(Callable func) : InvokerBase(func) {}

            Ret invoke(Args &&...args)
            {
                return this->func_(std::forward<Args>(args)...);
            }
        };

        using invoker_t = std::conditional_t<std::is_member_function_pointer_v<Callable>,
                                             MemberFunctionInvoker, FreeFunctionInvoker>;
        invoker_t invoker_;
    };

    std::unique_ptr<CallableInterface> func_p_;
    args_list<Args...>                 args_list_;
};
} // namespace MySTL
