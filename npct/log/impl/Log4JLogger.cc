#include <iostream>
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Priority.hh>
#include "common/errors/errors.h"
#include <cstdarg>

#include "Log4JLogger.h"

#define _CRT_NO_VA_START_VALIDATION
namespace npct::log::impl
{
    Log4JLogger::Log4JLogger(log4cpp::Category& logger, LogLevel log_level)
        : appender_(std::make_shared<log4cpp::OstreamAppender>("console", &std::cout)),
          log_level_(log_level),
          logger_(logger)
    {
        setLevel(log_level_);
    }

    Log4JLogger::Log4JLogger(const std::string &name, LogLevel log_level)
        : appender_(std::make_shared<log4cpp::OstreamAppender>("console", &std::cout)), 
          log_level_(log_level),
          logger_(log4cpp::Category::getInstance(name))
    {
        appender_->setLayout(new log4cpp::BasicLayout());

        setLevel(log_level_);
        logger_.addAppender(appender_.get());
    }

    void Log4JLogger::log(LogLevel log_level, const StringClass& msg)
    {
        logger_.log(fromLogLevel(log_level), msg);
    }

    void Log4JLogger::log(LogLevel log_level, const CharClass *msg)
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

    std::unique_ptr<Log4JLogger> Log4JLogger::GetRoot(LogLevel log_level)
    {
        return std::move(std::unique_ptr<Log4JLogger>(new Log4JLogger(log4cpp::Category::getRoot(), log_level)));
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
}
