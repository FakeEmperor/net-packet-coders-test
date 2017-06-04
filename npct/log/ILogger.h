#ifndef NPCT_LOG_ILOGGER_H
#define NPCT_LOG_ILOGGER_H

#include <string>
#include <type_traits>

namespace npct::log
{
    struct LoggerCapabilites
    {
    public:
        enum class FlowControl { SYNC, ASYNC } flow_control;
        enum class ThreadSupport { NONE, FULL } thread_support;
        enum class CacheControl { NONE, PARTIAL, FULL } cache_control;
    };
    

    enum class LogLevel { LL_ERROR, LL_WARNING, LL_INFO, LL_DEBUG };

    template <class StringClass_ = std::string>
    class ILogger
    {
    public:
        using CharClass = typename StringClass_::value_type;
        using StringClass = StringClass_;

        static_assert(
            std::is_base_of<
                std::basic_string<CharClass, typename StringClass_::traits_type, typename StringClass_::allocator_type>,
                StringClass>::value, 
            "Template parameter 'StringClass' must have std::basic_string as its base class"
            );
    
    public:
        virtual ~ILogger() = default;
        
        virtual void log(LogLevel log_level, const StringClass &msg) = 0;
        virtual void log(LogLevel log_level, const CharClass *msg) = 0;
        virtual void log(LogLevel log_level, const CharClass *msg,  ...) = 0;

        virtual LoggerCapabilites capabilites() const = 0;

    };

}


#endif // !NPCT_LOG_ILOGGER_H
