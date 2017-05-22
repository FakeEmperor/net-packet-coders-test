#ifndef NPCT_COMMON_HANDLERS_H
#define NPCT_COMMON_HANDLERS_H

#include <functional>

namespace npct::common
{
    template <class ...Args>
    using HandlerFn = std::function<void(Args...)>;
}

#endif // !NPCT_COMMON_HANDLERS_H
