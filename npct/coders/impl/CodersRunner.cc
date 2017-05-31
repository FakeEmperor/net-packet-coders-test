#include "CodersRunner.h"
#include "coders/ECoderErrorCodes.h"

namespace npct::coders::impl
{
    

    void CodersRunner::on(ECodersRunnerEvents event_type, const OnMessageFn &handler) noexcept
    {
        mh_helper_.addCallback(event_type, handler);

    }

    CodersRunner::RefType<CoderResult> CodersRunner::commitMessage(RefType<const CoderMessage> &message) const noexcept(false)
    {
        if (!coder_queue_.size())
            throw CoderError(ECoderErrorCodes::E_NO_CODERS_FOUND);

        bool found = false;
        for (auto it = coder_queue_.begin(); !found and it != coder_queue_.end(); ++it)
        {
            auto msg_type = it->get()->canProcess(message);
            switch(msg_type)
            {
            case CoderMessage::MT_INPUT: found = true; queue_.push_back(it->get()->processInput(message)); break;
            case CoderMessage::MT_OUTPUT: found = true; queue_.push_back(it->get()->processOutput(message)); break;
            case CoderMessage::MT_NONE: break;
            }
        }
        if (found)
            return queue_.back().get();
        else
            throw CoderError(ECoderErrorCodes::E_NO_CODERS_FOUND);
    }

    void CodersRunner::registerCoder(HolderType<ICoder> &&coder) noexcept
    {
        coder_queue_.push_back(std::move(coder));
    }

    ICodersRunner<>::CodersRunnerStrategy CodersRunner::getStrategy() const noexcept
    {
        return STRATEGY;
    }

    CodersRunner::RefType<CoderResult> CodersRunner::lastResult() const noexcept(false)
    {
        return queue_.back().get();
    }

    const CodersRunner::QueueType<CoderResult> & CodersRunner::results() const noexcept(false)
    {
        return queue_;
    }



    CodersRunner::HolderType<CoderResult> CodersRunner::acquireResult(const RefType<CoderResult> &ref) noexcept(false)
    {
        for (auto it = queue_.begin(); it != queue_.end(); ++it)
            if (it->get() == ref)
            {
                auto moved = std::move(*it);
                queue_.erase(it);
                return moved;
            }
        throw std::range_error("Reference not found");
    }

    void CodersRunner::start() noexcept(false)
    {
        // do nothing - this class provides only manual control.
    }
}
