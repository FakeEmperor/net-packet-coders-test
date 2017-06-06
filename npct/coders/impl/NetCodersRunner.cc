#include "NetCodersRunner.h"
#include "coders/CoderErrorMessage.h"
#include "coders/ECoderErrorCodes.h"

namespace npct::coders::impl
{


    NetCodersRunner::NetCodersRunner(asio::io_service &io_loop, uint16_t port):
            ReceiverBase(io_loop, port)
    {
        
    }


    void NetCodersRunner::start()
    {
        using namespace std::placeholders;
        ReceiverBase::onPacketReceive(std::bind(&NetCodersRunner::handle_receive_message_, this, _1, _2, _3));
        ReceiverBase::start();
    }

    void NetCodersRunner::handle_receive_message_(const ReceiverBase *source, SessionObject *session, const MessageObject *message) const
    {
        // try to parse this message
        try
        {
            auto response = commitMessage(message);
            if (response->first.error)
                session->send(CoderErrorMessage(response->first.error, *message));
            else
                session->send(*response->second);

        } 
        catch (const CoderError &e)
        {
            session->send(CoderErrorMessage(e, *message));
        }
        catch (const std::runtime_error &e)
        {
            session->send(CoderErrorMessage(
                    CoderError(ECoderErrorCodes::E_INTERNAL_SERVER_ERROR,
                               e.what()), *message));
        }
        catch (...) {
            session->send(CoderErrorMessage(
                    CoderError(ECoderErrorCodes::E_INTERNAL_SERVER_ERROR,
                               "Unknown message format sent or error occured during processing"), *message));
        }

    }
}
