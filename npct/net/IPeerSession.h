#ifndef NPCT_NET_ISESSION_H
#define NPCT_NET_ISESSION_H

#include <type_traits>
#include <cstdint>
#include <common/callbacks.h>
#include <utils/compile_time.h>

namespace npct::net
{
    /** 
     * \brief Basic SessionSettings contains optional flags field with implementation-specific bits
     *
     */
    class BaseSessionSettings
    {
    public:
        uint64_t flags;
        virtual ~BaseSessionSettings() = default;
    };



   
   

    /**
     * \brief This class is an abstraction over any connection
     *        It uses concept of `messages` which, in fact, can be a streamable object.
     *
     */
    template <class Impl_, class SessionSettingsClass_ , class MessageClass_ >
    class IPeerSession
    {
        
        static_assert(std::is_base_of<BaseSessionSettings, SessionSettingsClass_>::value , "SessionSettingsClass must be a subclass of BaseSessionSettings");

    public:
        using Impl = Impl_;
        using SessionSettingsClass = SessionSettingsClass_ ;
        using MessageClass = MessageClass_;

        typedef common::HandlerFn<Impl*, const MessageClass*> OnMessageFn;
        typedef common::HandlerFn<Impl*> OnEventFn;

        virtual ~IPeerSession() = default;
        
        //////////// SETTINGS

        virtual SessionSettingsClass getSettings() const = 0;
        virtual void setSettings(const SessionSettingsClass&) = 0;

        /////////// ACTIONS

        virtual void send(const MessageClass_& data) = 0;

        ////////// CALLBACKS

        virtual void onSessionClosing(const OnEventFn & handler) = 0;
        virtual void onMessageReceived(const OnMessageFn & handler) = 0;
        virtual void onMessageSent(const OnMessageFn & handler) = 0;


    };


}


#endif // !NPCT_NET_ISESSION_H
