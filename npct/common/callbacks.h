#ifndef NPCT_COMMON_HANDLERS_H
#define NPCT_COMMON_HANDLERS_H

#include <functional>

namespace npct::common
{
    template <class ...Args>
    using HandlerFn = std::function<void(Args...)>;

    template <typename ReturnType, class ...Args>
    using CallbackFn = std::function<ReturnType(Args...)>;
}

#endif // !NPCT_COMMON_HANDLERS_H
