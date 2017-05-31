#ifndef NPCT_CODERS_ECODERERRORCODES_H
#define NPCT_CODERS_ECODERERRORCODES_H

#include <cstdint>

namespace npct::coders
{
    enum ECoderErrorCodes : uint16_t
    {
        E_UNKNOWN_MESSAGE_FORMAT = 422, // HTTP 422 - UNPROCESSABLE ENTITY
        E_INTERNAL_SERVER_ERROR = 500, // HTTP 500 - INTERNAL ERROR
        
        E_USER_INPUT_ERROR = 400, // HTTP 400 - USER INPUT ERROR
        

        E_NO_CODERS_FOUND = 555, // if message is not processed
        
    };

}

#endif // !NPCT_CODERS_ECODERERRORCODES_H
