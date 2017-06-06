#ifdef NPCT_LOG_ENABLE_LOG4CPP
#ifndef NPCT_LOG_IMPL_LOG4JLOGGER_H
#define NPCT_LOG_IMPL_LOG4JLOGGER_H

#include <memory>

#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Priority.hh>

#include "../ILogger.h"

namespace npct::log::impl
{
    

    class Log4JLogger final: public ILogger<>
    {
    public:
        Log4JLogger(log4cpp::Category &logger, LogLevel log_level);
        Log4JLogger(log4cpp::Category &logger);
        Log4JLogger(const std::string &name, LogLevel log_level, bool configured = false);
        Log4JLogger(const Log4JLogger &) = default;

        void log(LogLevel log_level, const StringClass &msg) override;
        void log(LogLevel log_level, const CharClass *msg, ...) override;

        void setLevel(LogLevel log_level) override;
        LogLevel getLevel() const override;
        
        LoggerCapabilites capabilites() const override;
        static std::shared_ptr<ILogger<>> GetRoot(LogLevel log_level);
        static std::shared_ptr<ILogger<>> GetRoot();

        static void configureFromFile(const std::string &path) noexcept(false);
    private:
        static std::shared_ptr<ILogger<>> Root_;
        static std::shared_ptr<log4cpp::Appender> DefaultAppender_;

        static log4cpp::Priority::PriorityLevel fromLogLevel(LogLevel log_level);
        static LogLevel fromPriorityLevel(log4cpp::Priority::PriorityLevel priority_level);

        static std::shared_ptr<log4cpp::Appender> getDefaultAppender();

        std::shared_ptr<log4cpp::Appender> appender_;

        LogLevel log_level_ ;
        log4cpp::Category &logger_;

        
    };


}


#endif // !NPCT_LOG_IMPL_LOG4JLOGGER_H
#endif // NPCT_LOG_ENABLE_LOG4CPP