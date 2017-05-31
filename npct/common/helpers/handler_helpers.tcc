#include "handler_helpers.h"

namespace npct::common::helpers
{
    template <class ... Args>
    bool HandlerHelper<HandlerFn<Args...>>::removeCallback(const FnPtr& fn_ptr) noexcept
    {
        auto iter = callbacks_.find(fn_ptr);
        bool found = iter != callbacks_.end();
        if (found)
            callbacks_.erase(iter);
        return found;
    }

    template <class ... Args>
    typename HandlerHelper<HandlerFn<Args...>>::FnPtr HandlerHelper<HandlerFn<Args...>>::addCallback(const Fn& fn) noexcept
    {
        return callbacks_.insert(std::make_unique<Fn>(fn)).first->get();
    }

    template <class ... Args>
    void HandlerHelper<HandlerFn<Args...>>::triggerCallbacks(Args ...args) const noexcept
    {
        for (auto begin = callbacks_.cbegin(); begin != callbacks_.cend(); ++begin)
            begin->get()->operator()(args...);
    }

    template <class KeyType, class ... HandlerTypes>
    template <class HandlerType>
    bool MultiHandlerHelper<KeyType, HandlerTypes...>::removeCallback(const KeyType &key, const HandlerType*& fn_ptr) noexcept
    {
        if (callbacks_.find(key) == callbacks_.end()) return false;
        else return mpark::get<HandlerHelper<HandlerType>>(callbacks_[key]).removeCallback(fn_ptr);
    }

    template <class KeyType, class ... HandlerTypes>
    template <class HandlerType>
    HandlerType* MultiHandlerHelper<KeyType, HandlerTypes...>::addCallback(const KeyType &key, const HandlerType& fn) noexcept
    {
        if (callbacks_.find(key) == callbacks_.end()) callbacks_.emplace(key, HandlerHelper<HandlerType>());
        return mpark::get<HandlerHelper<HandlerType>>(callbacks_[key]).addCallback(fn);
    }

    template <class KeyType, class ... HandlerTypes>
    template <class HandlerType, class ... Args>
    void MultiHandlerHelper<KeyType, HandlerTypes...>::triggerCallbacks(const KeyType &key, Args... args) const noexcept
    {
        if (callbacks_.find(key) == callbacks_.end()) return;
        else mpark::get<HandlerHelper<HandlerType>>(callbacks_.at(key)).triggerCallbacks(args...);
    }


}



