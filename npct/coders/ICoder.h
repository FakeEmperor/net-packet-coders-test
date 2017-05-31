#ifndef NPCT_CODERS_ICODER_H
#define NPCT_CODERS_ICODER_H

#include <memory>
#include <utility>

#include "CoderError.h"
#include "CoderMessage.h"
#include "CoderResultInfo.h"
#include <utils/string.h>

namespace npct::coders
{
    using CoderResult = std::pair<CoderResultInfo, std::unique_ptr<CoderMessage>>;

    class ICoder
    {
    public:
        template <class T>
        using HolderType = std::unique_ptr<T>;

        template <typename T>
        using RefType = decltype(std::declval<HolderType<T>>().get());

        virtual HolderType<CoderResult> processInput(RefType<const CoderMessage> &source) const = 0;
        virtual HolderType<CoderResult> processOutput(RefType<const CoderMessage> &encoded) const = 0;
        
        virtual CoderMessage::MessageDirectionType canProcess(RefType<const CoderMessage> &msg) const
        {
            if (utils::string::ascii_icmp(msg->algorithm, std::string(name())) && msg->message_direction != CoderMessage::MT_NONE)
                return msg->message_direction;
            return CoderMessage::MT_NONE;
        };

        virtual const char* name() const { return "";  }

        virtual ~ICoder() = default;

    };

}

#endif // !NPCT_CODERS_ICODER_H
