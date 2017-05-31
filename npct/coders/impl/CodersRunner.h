#ifndef NPCT_CODERS_IMPL_CODERS_RUNNER
#define NPCT_CODERS_IMPL_CODERS_RUNNER

#include "../ICodersRunner.h"
#include <common/helpers/handler_helpers.h>
#include <utils/utils.h>
#include <queue>

namespace npct::coders::impl
{
    class CodersRunner : public ICodersRunner<>
    {
    public:
        void on(ECodersRunnerEvents event_type, const OnMessageFn &handler) noexcept override;
        RefType<CoderResult> commitMessage(RefType<const CoderMessage> &message) const noexcept(false) override;


        void registerCoder(HolderType<ICoder> &&coder) noexcept override;

        template <typename CoderType>
        typename std::enable_if<std::is_base_of<ICoder, CoderType>::value>::type registerCoder(HolderType<CoderType> &&coder) noexcept {
            registerCoder(npct::utils::static_unique_pointer_cast<ICoder>(std::move(coder)));
        }


        CodersRunnerStrategy getStrategy() const noexcept override;
        RefType<CoderResult> lastResult() const noexcept(false) override;
        const QueueType<CoderResult> & results() const noexcept(false) override;
        HolderType<CoderResult> acquireResult(const RefType<CoderResult> &result) noexcept(false) override;
        
        

        static constexpr CodersRunnerStrategy STRATEGY = { 
            CodersRunnerStrategy::MultipleCodersMessageMatch::NONE, 
            CodersRunnerStrategy::MessageQueueProcessing::SYNC, 
            CodersRunnerStrategy::ResultQueueRotating::PRESERVE_ALL
        };

        ~CodersRunner() override = default;


        void start() noexcept(false) override;
    protected:
        common::helpers::MultiHandlerHelper<Base::ECodersRunnerEvents, Base::OnMessageFn> mh_helper_;
        mutable QueueType<CoderResult> queue_;
        QueueType<ICoder> coder_queue_;
        
    };


}


#endif // !NPCT_CODERS_IMPL_CODERS_RUNNER
