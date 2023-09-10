#pragma once

#include "args_list.hpp"
#include <memory>
#include <type_traits>

namespace MySTL {

template <typename Func, typename... Args> class bind {
  public:
    // TODO: Add perfect forwarding for Func also (need to add possobility for
    // containing Func&)
    bind(Func func, Args &&...args)
        : func_(std::make_unique<CallableImpl<Func>>(std::move(func))),
          args_list_(std::forward<Args>(args)...)
    {
    }

    template <typename... CallArgs> void operator()(CallArgs &&...args)
    {
        return func_->call(std::forward<CallArgs>(args)...);
    }

  private:
    // TODO: Create just 1 struct for processing all calls,
    // struct should be temlate with call-operator, should process
    // members and free-fcuntions calls
    struct CallableInterface {
        virtual void call(Args &&...args) = 0;
        virtual ~CallableInterface()      = default;
    };

    template <typename Callable> struct CallableImpl : CallableInterface {
        CallableImpl(Callable func) : invoker_(std::move(func)) {}

        void call(Args &&...args) override
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

            template <typename ObjType> void invoke(ObjType &&obj, Args &&...args)
            {
                return (obj.f)(std::forward<Args>(args)...);
            }
        };

        struct FreeFunctionInvoker : InvokerBase {
            FreeFunctionInvoker(Callable func) : InvokerBase(func) {}

            void invoke(Args &&...args)
            {
                return this->func_(std::forward<Args>(args)...);
            }
        };

        using invoker_t = std::conditional_t<std::is_member_function_pointer_v<Callable>,
                                             MemberFunctionInvoker, FreeFunctionInvoker>;
        invoker_t invoker_;
    };

    std::unique_ptr<CallableInterface> func_;
    args_list<Args...>                 args_list_;
};
} // namespace MySTL
