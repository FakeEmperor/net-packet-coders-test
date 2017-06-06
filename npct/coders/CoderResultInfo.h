#ifndef NPCT_CODERS_CODERRESULTINFO_H
#define NPCT_CODERS_CODERRESULTINFO_H
#include "CoderError.h"

namespace npct::coders
{

    /**
     * \brief Result status of a coder's any operation.
     *        You can specify more info for more result meta data
     *
     */
    struct CoderResultInfo
    {
        enum StatusFlags { CR_ENCODED, CR_DECODED, CR_VERIFIED, CR_NONE } status = { StatusFlags::CR_NONE };
        CoderError error;
        bool result_ready = { true };

        CoderResultInfo() = default;
        CoderResultInfo(CoderResultInfo &&) noexcept = default;
        CoderResultInfo(const CoderResultInfo &) = default;
        CoderResultInfo(const StatusFlags &status, const CoderError &error, const bool &result_ready): status(status), error(error), result_ready(result_ready) {} ;

        CoderResultInfo & operator = (const CoderResultInfo &) = default;
        CoderResultInfo & operator = (CoderResultInfo &&) noexcept = default;

    };

}


#endif // !NPCT_CODERS_CODERRESULTINFO_H
