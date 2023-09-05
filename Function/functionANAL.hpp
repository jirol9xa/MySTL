#pragma once

#include <functional>
#include <memory>

namespace MySTL {

template <typename T> class function;

template <typename Ret, typename... Param> class function<Ret(Param...)> {
  public:
    template <typename Callable>
    function(Callable func)
        : func_p_(std::make_unique<CallableImpl<Callable>>(std::move(func)))
    {
    }

    Ret operator()(Param... args) { return func_p_->call(args...); }

  private:
    struct CallableInterface {
        virtual Ret call(Param... par) = 0;
        virtual ~CallableInterface()   = default;
    };

    template <typename Callable> struct CallableImpl : CallableInterface {
        CallableImpl(Callable func) : func_(std::move(func)) {}
        Ret call(Param... par) override { return std::invoke(func_, par...); }

        Callable func_;
    };

    std::unique_ptr<CallableInterface> func_p_;
};
} // namespace MySTL
