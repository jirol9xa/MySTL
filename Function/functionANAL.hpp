#pragma once

#include <functional>
#include <memory>
#include <type_traits>

namespace MySTL {

template <typename T> class function;

template <typename Ret, typename... Param> class function<Ret(Param...)> {
  public:
    template <typename Callable>
    function(Callable func)
        : func_p_(std::make_unique<CallableImpl<Callable>>(std::move(func)))
    {
    }
    function(const function &func) : func_p_(func.func_p_->clone()) {}
    function(function &&func) = default;
    function &operator=(const function &func)
    {
        func_p_ = std::move(func.func_p_->clone());

        return *this;
    }

    template <typename Callable> function &operator=(Callable func)
    {
        func_p_.reset(new CallableImpl<Callable>(std::move(func)));

        return *this;
    }

    Ret operator()(Param &&...args)
    {
        return func_p_->call(std::forward<Param>(args)...);
    }

  private:
    struct CallableInterface {
        virtual Ret                                call(Param &&...par) = 0;
        virtual std::unique_ptr<CallableInterface> clone()              = 0;

        virtual ~CallableInterface() = default;
    };

    template <typename Callable> struct CallableImpl : CallableInterface {
        CallableImpl() = default;
        CallableImpl(Callable func) : func_(std::move(func)) {}

        Ret call(Param &&...par) override
        {
            return std::invoke(func_, std::forward<Param>(par)...);
        }
        std::unique_ptr<CallableInterface> clone() override
        {
            return std::make_unique<CallableImpl>(func_);
        }

        Callable func_;
    };

    std::unique_ptr<CallableInterface> func_p_;
};
} // namespace MySTL
