#ifndef NPCT_COMMON_HELPERS_CALLBACKHELPER_H
#define NPCT_COMMON_HELPERS_CALLBACKHELPER_H

#include <set>
#include <memory>
#include <map>
#include "../callbacks.h"
#include <utils/utils.h>
#include <mpark/variant.hpp>

namespace npct::common::helpers
{
    template <class ...Args>
    struct HandlerHelper {
        static_assert(                                                                                  
            std::integral_constant<Args..., false>::value,
            "Handler Helper have invalid arguments");                                 
    };  
    template <class ...Args>
    struct HandlerHelper<HandlerFn<Args...>>
    {
        using Fn = HandlerFn<Args...>;
        using FnHnd = std::unique_ptr<Fn>;
        using FnPtr = HandlerFn<Args...>*;

        HandlerHelper(const HandlerHelper&) = delete;
        HandlerHelper(HandlerHelper&&) = default;
        HandlerHelper() = default;

        virtual ~HandlerHelper() = default;

        virtual bool removeCallback(const FnPtr &fn_ptr) noexcept;
        virtual FnPtr addCallback(const Fn &fn) noexcept;

        virtual void triggerCallbacks(Args ...args) const noexcept;

        
        std::set<FnHnd, utils::pointer_cmp<Fn>> callbacks_;
    };
    

    template <class KeyType, class ...HandlerTypes>
    struct MultiHandlerHelper
    {
        virtual ~MultiHandlerHelper() = default;

        template<class HandlerType>
        bool removeCallback(const KeyType &key, const HandlerType* &fn_ptr) noexcept;
        template<class HandlerType>
        HandlerType* addCallback(const KeyType &key, const HandlerType &fn) noexcept;

        template<class HandlerType, class ...Args>
        void triggerCallbacks(const KeyType &key, Args ...args) const noexcept;

        using variant_t = mpark::variant<HandlerHelper<HandlerTypes>...>;
        std::map<KeyType, variant_t> callbacks_;    
    };




    
}

#include "handler_helpers.tcc"

#endif // !NPCT_COMMON_HELPERS_CALLBACKHELPER_H
