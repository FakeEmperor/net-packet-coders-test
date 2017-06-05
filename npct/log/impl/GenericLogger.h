//
// Created by lol on 6/4/2017.
//

#ifndef NPCT_LOG_IMPL_GENERICLOGGER_H
#define NPCT_LOG_IMPL_GENERICLOGGER_H

#include "../ILogger.h"

namespace npct::log::impl {

    class GenericLogger final: public ILogger<>
    {
    public:
        void log(LogLevel log_level, const StringClass &msg) override ;
        void log(LogLevel log_level, const CharClass *msg) override ;
        void log(LogLevel log_level, const CharClass *msg,  ...) override ;

        virtual LoggerCapabilites capabilites() const ;


    };

}

#endif //!NPCT_LOG_IMPL_GENERICLOGGER_H
