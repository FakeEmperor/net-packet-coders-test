#ifndef NPCT_CODER_CODERERRORMESSAGE_H
#define NPCT_CODER_CODERERRORMESSAGE_H

#include "CoderError.h"
#include "CoderMessage.h"

namespace npct::coders
{
    class CoderErrorMessage: public CoderError, public CoderMessage
    {
    public:
        ~CoderErrorMessage() override = default;
        CoderErrorMessage(const CoderError &error, const CoderMessage &message): CoderMessage(message), CoderError(error)
        {
        }
        CoderErrorMessage(const CoderError &error) : CoderError(error)
        {
        }

        nlohmann::json to_json() const override {
            auto j = CoderError::to_json();
            j["error_linked_message"] = CoderMessage::to_json();
            return j;
        };
        


    };



}


#endif // !NPCT_CODER_CODERERRORMESSAGE_H
