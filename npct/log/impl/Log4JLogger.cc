#ifdef NPCT_LOG_ENABLE_LOG4CPP

#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Priority.hh>

#include <iostream>
#include <cstdarg>

#include "common/errors/errors.h"

#include "Log4JLogger.h"

#define _CRT_NO_VA_START_VALIDATION
namespace npct::log::impl
{
    std::shared_ptr<ILogger<>> Log4JLogger::Root_ = nullptr;
    std::shared_ptr<log4cpp::Appender> Log4JLogger::DefaultAppender_ = nullptr;

    Log4JLogger::Log4JLogger(log4cpp::Category& logger)
        :     logger_(logger),
              log_level_(fromPriorityLevel((log4cpp::Priority::PriorityLevel)logger.getPriority()))
    {

    }

    Log4JLogger::Log4JLogger(log4cpp::Category& logger, LogLevel log_level)
        : Log4JLogger(logger)
    {
        setLevel(log_level);
    }

    Log4JLogger::Log4JLogger(const std::string &name, LogLevel log_level, bool configured)
        : logger_(log4cpp::Category::getInstance(name)),
          appender_(configured ?
                    std::shared_ptr<log4cpp::Appender>(
                          log4cpp::Category::getInstance(name).getAppender(),
                            [](log4cpp::Appender *){}
                    ) :
                    std::move(getDefaultAppender()) )
    {
        setLevel(log_level);
        logger_.addAppender(appender_.get());

    }

    void Log4JLogger::log(LogLevel log_level, const StringClass& msg)
    {
        logger_.log(fromLogLevel(log_level), msg);
    }


    void Log4JLogger::log(LogLevel log_level, const CharClass *msg, ...)
    {
        va_list args;
        va_start(args, msg);
        logger_.logva(fromLogLevel(log_level), msg, args);
        va_end(args);
    }

    void Log4JLogger::setLevel(LogLevel log_level)
    {
        log_level_ = log_level;
        logger_.setPriority(fromLogLevel(log_level));
    }

    LogLevel Log4JLogger::getLevel() const
    {
        return log_level_;
    }

    LoggerCapabilites Log4JLogger::capabilites() const
    {
        return { LoggerCapabilites::FlowControl::SYNC, LoggerCapabilites::ThreadSupport::NONE, LoggerCapabilites::CacheControl::NONE };
    }



    std::shared_ptr<ILogger<>> Log4JLogger::GetRoot(LogLevel log_level)
    {
        GetRoot();
        Root_->setLevel(log_level);
        return Root_;
    }

    std::shared_ptr<ILogger<>> Log4JLogger::GetRoot() {
        if (!Root_) {
            Root_ = std::make_shared<Log4JLogger>(
                    log4cpp::Category::getRoot()
            );
        }
        return Root_;
    }

    log4cpp::Priority::PriorityLevel Log4JLogger::fromLogLevel(LogLevel log_level)
    {
        switch(log_level)
        {
        case LogLevel::LL_ERROR:   return log4cpp::Priority::ERROR;
        case LogLevel::LL_WARNING: return log4cpp::Priority::WARN;
        case LogLevel::LL_INFO:    return log4cpp::Priority::INFO;
        case LogLevel::LL_DEBUG:   return log4cpp::Priority::DEBUG;
        }
        throw common::errors::ValueError("log_level");
    }

    LogLevel Log4JLogger::fromPriorityLevel(log4cpp::Priority::PriorityLevel priority_level) {
        switch(priority_level)
        {

            case log4cpp::Priority::EMERG:  return LogLevel::LL_ERROR;
            case log4cpp::Priority::ALERT:  return LogLevel::LL_ERROR;
            case log4cpp::Priority::CRIT:   return LogLevel::LL_ERROR;
            case log4cpp::Priority::ERROR:  return LogLevel::LL_ERROR;
            case log4cpp::Priority::WARN:   return LogLevel::LL_WARNING;
            case log4cpp::Priority::NOTICE: return LogLevel::LL_WARNING;
            case log4cpp::Priority::INFO:   return LogLevel::LL_INFO;
            case log4cpp::Priority::DEBUG:  return LogLevel::LL_DEBUG;
            case log4cpp::Priority::NOTSET: return LogLevel::LL_DEBUG;
        }
        throw common::errors::ValueError("priority_level");
    }

    void Log4JLogger::configureFromFile(const std::string &path) {
        log4cpp::PropertyConfigurator::configure(path);
    }

    std::shared_ptr<log4cpp::Appender> Log4JLogger::getDefaultAppender() {
        if(!DefaultAppender_) {
            DefaultAppender_ = std::make_shared<log4cpp::OstreamAppender>("console", &std::cout);
            DefaultAppender_->setLayout(new log4cpp::BasicLayout());
        }
        return DefaultAppender_;
    }


}
#endif