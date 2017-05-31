#ifndef NPCT_CODERS_ICODERSRUNNER_H
#define NPCT_CODERS_ICODERSRUNNER_H

#include <memory>
#include <deque>
#include <utility>
#include "ICoder.h"
#include <common/callbacks.h>

namespace npct::coders
{

    
    


    template <template <class ...Args> class ResultQueueType_ = std::deque>
    class ICodersRunner
    {
    public:
        using Base = ICodersRunner;

        enum ECodersRunnerEvents { ON_DECODE, ON_ENCODE, ON_MESSAGE };
        

        struct CodersRunnerStrategy
        {
            enum class MultipleCodersMessageMatch { THROW, LOG, NONE } mcmm_type;
            enum class MessageQueueProcessing { SYNC, ASYNC } mqp_type;
            enum class ResultQueueRotating { PRESERVE_ALL, ONLY_LATEST, FIXED_SIZE, DYNAMIC_SIZE } rqr_type;
        };

        template <class T>
        using HolderType = std::unique_ptr<T>;

        template <typename T>
        using RefType = typename HolderType<T>::pointer;

        template <typename T>
        using QueueType = ResultQueueType_<HolderType<T>>;
        
        using OnMessageFn = npct::common::HandlerFn<ECodersRunnerEvents, RefType<CoderResult>, RefType<ICoder>>;

    public:
        virtual ~ICodersRunner() = default;

        virtual void on(ECodersRunnerEvents event_type, const OnMessageFn &handler) noexcept = 0;

        /**
         * \brief Class allows to 'start' that is, initialize any objects and activities needed for class to accept messages
         *        from designated sources. May be overriden as an empty function.
         */
        virtual void start() noexcept(false) = 0;

        virtual RefType<CoderResult> commitMessage(RefType<const CoderMessage> &message) const noexcept(false) = 0;

        virtual void registerCoder(HolderType<ICoder> &&coder) noexcept = 0;


        virtual CodersRunnerStrategy getStrategy()  const noexcept = 0;

        virtual RefType<CoderResult> lastResult() const noexcept(false) = 0;
        
        virtual const QueueType<CoderResult> & results() const noexcept(false) = 0;

        virtual HolderType<CoderResult> acquireResult(const RefType<CoderResult> &) noexcept(false) = 0;

    };


}


#endif // !NPCT_CODERS_ICODERSRUNNER_H
