#ifndef AREG_IPC_PRIVATE_NECONNECTION_HPP
#define AREG_IPC_PRIVATE_NECONNECTION_HPP

/************************************************************************
 * \file        areg/ipc/private/NEConnection.hpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform Connection constants.
 ************************************************************************/

/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "areg/base/NEMemory.hpp"
#include "areg/component/Timer.hpp"
#include "areg/appbase/NEApplication.hpp"
#include "areg/ipc/NERemoteService.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class StubAddress;
class ProxyAddress;
class RemoteMessage;
class Channel;

//////////////////////////////////////////////////////////////////////////
// NEConnection namespace declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   The connection namespace with shared objects and helper methods
 *          to create fixed communication messages.
 **/
namespace NEConnection
{
    /**
     * \brief   NEConnection::MessageHelloServer
     *          Fixed predefined message to start server connection
     **/
    extern AREG_API const NEMemory::sRemoteMessage      MessageHelloServer;
    /**
     * \brief   NEConnection::MessageByeServer
     *          Fixed predefined message to stop server connection
     **/
    extern const NEMemory::sRemoteMessage      MessageByeServer;
    /**
     * \brief   NEConnection::MessageAcceptClient
     *          Fixed message to notify client connection acceptance
     **/
    extern const NEMemory::sRemoteMessage      MessageAcceptClient;
    /**
     * \brief   NEConnection::MessageRejectClient
     *          Fixed message to notify client connected rejection
     **/
    extern const NEMemory::sRemoteMessage      MessageRejectClient;
    /**
     * \brief   NEConnection::MessageByeClient
     *          Fixed message to notify client disconnect
     **/
    extern const NEMemory::sRemoteMessage      MessageByeClient;
    /**
     * \brief   NEConnection::MessageRegisterService
     *          Fixed message to start service registration
     **/
    extern const NEMemory::sRemoteMessage      MessageRegisterService;
    /**
     * \brief   NEConnection::MessageQueryService
     *          Fixed message to query available services
     **/
    extern const NEMemory::sRemoteMessage      MessageQueryService;
    /**
     * \brief   NEConnection::MessageRegisterNotify
     *          Fixed message to register notification
     **/
    extern const NEMemory::sRemoteMessage      MessageRegisterNotify;

    /**
     * \brief   NEConnection::CLIENT_SEND_MESSAGE_THREAD
     *          Fixed name of client message sender thread
     **/
    const char * const      CLIENT_SEND_MESSAGE_THREAD      = "CLIENT_SEND_MESSAGE_THREAD";
    /**
     * \brief   NEConnection::CLIENT_RECEIVE_MESSAGE_THREAD
     *          Fixed name of client message receiver thread
     **/
    const char * const      CLIENT_RECEIVE_MESSAGE_THREAD   = "CLIENT_RECEIVE_MESSAGE_THREAD";
    /**
     * \brief   NEConnection::CLIENT_DISPATCH_MESSAGE_THREAD
     *          Fixed name of client message dispatcher thread
     **/
    const char * const      CLIENT_DISPATCH_MESSAGE_THREAD  = "CLIENT_DISPATCH_MESSAGE_THREAD";
    /**
     * \brief   NEConnection::SERVER_SEND_MESSAGE_THREAD
     *          Fixed name of server message sender thread
     **/
    const char * const      SERVER_SEND_MESSAGE_THREAD      = "SERVER_SEND_MESSAGE_THREAD";
    /**
     * \brief   NEConnection::SERVER_RECEIVE_MESSAGE_THREAD
     *          Fixed name of server message receiver thread
     **/
    const char * const      SERVER_RECEIVE_MESSAGE_THREAD   = "SERVER_RECEIVE_MESSAGE_THREAD";
    /**
     * \brief   NEConnection::SERVER_DISPATCH_MESSAGE_THREAD
     *          Fixed name of server message dispatcher thread
     **/
    const char * const      SERVER_DISPATCH_MESSAGE_THREAD  = "SERVER_DISPATCH_MESSAGE_THREAD";
    /**
     * \brief   NEConnection::CLIENT_CONNECT_TIMER_NAME
     *          Fixed name of client connection retry timer name
     **/
    const char * const      CLIENT_CONNECT_TIMER_NAME       = "CLIENT_CONNECT_TIMER_NAME";
    /**
     * \brief   NEConnection::SERVER_CONNECT_TIMER_NAME
     *          Fixed name of server connection retry timer name
     **/
    const char * const      SERVER_CONNECT_TIMER_NAME       = "SERVER_CONNECT_TIMER_NAME";
    /**
     * \brief   NEConnection::DEFAULT_ROUTER_CONFIG_FILE
     *          Default path of remote routing service configuration file name
     **/
    const char * const      DEFAULT_ROUTER_CONFIG_FILE      = NERemoteService::DEFAULT_CONFIG_FILE;
    /**
     * \brief   NEConnection::DEFAULT_RETRY_CONNECT_TIMEOUT
     *          Default connect retry timer timeout value in milliseconds
     **/
    const unsigned int      DEFAULT_RETRY_CONNECT_TIMEOUT   = Timer::TIMEOUT_500_MS;  // 500 ms
    /**
     * \brief   NEConnection::DEFAULT_REMOTE_SERVICE_HOST
     *          Default IP-address of remote routing service.
     *          The default values are used if failed to read and parse router configuration file.
     **/
    const char * const      DEFAULT_REMOTE_SERVICE_HOST     = NERemoteService::DEFAULT_ROUTER_HOST;
    /**
     * \brief   NEConnection::DEFAULT_REMOTE_SERVICE_PORT
     *          Default connection port number of remote routing service
     *          The default values are used if failed to read and parse router configuration file.
     **/
    const unsigned short    DEFAULT_REMOTE_SERVICE_PORT     = NERemoteService::DEFAULT_ROUTER_PORT;
    /**
     * \brief   NEConnection::DEFAULT_REMOVE_SERVICE_ENABLED
     *          Message router enable / disable default flag. If true, by default it is enabled.
     *          The default values are used if failed to read and parse router configuration file.
     **/
    const bool              DEFAULT_REMOVE_SERVICE_ENABLED  = true;

    /**
     * \brief   NEConnection::CreateConnectRequest
     *          Initializes connection request message.
     **/
    RemoteMessage createConnectRequest( void );
    /**
     * \brief   NEConnection::CreateDisconnectRequest
     *          Initializes disconnect request message.
     * \param   cookie  The cookie set by routing service for the client, which is set in message
     **/
    RemoteMessage createDisconnectRequest( ITEM_ID cookie );
    /**
     * \brief   NEConnection::CreateConnectNotify
     *          Initializes connection notification message
     * \param   cookie  The cookie set by routing service for the client, which is set in message
     **/
    RemoteMessage createConnectNotify( ITEM_ID cookie );
    /**
     * \brief   NEConnection::CreateRejectNotify
     *          Initializes connection rejected message
     * \param   cookie  The cookie set by routing service for the client, which is set in message
     **/
    RemoteMessage createRejectNotify( ITEM_ID cookie );
    /**
     * \brief   NEConnection::CreateDisconnectNotify
     *          Initializes disconnect notification message sent by server to clients.
     * \param   cookie  The cookie set by routing service for the client, which is set in message
     **/
    RemoteMessage createDisconnectNotify( ITEM_ID cookie );
    /**
     * \brief   NEConnection::createRouterRegisterService
     *          Initializes Stub register message at router.
     * \param   stub    The address of remote Stub service, which is registering.
     * \param   source  The ID of source, which initiates message.
     * \return  Returns register stub message to send.
     * \see     createRouterUnregisterService
     **/
    RemoteMessage createRouterRegisterService( const StubAddress & stub, ITEM_ID source );
    /**
     * \brief   NEConnection::createRouterUnregisterService
     *          Initializes Stub unregister message at router.
     * \param   stub    The address of remote Stub service, which is registering.
     * \param   source  The ID of source, which initiates message.
     * \return  Returns unregister stub message to send.
     * \see     createRouterRegisterService
     **/
    RemoteMessage createRouterUnregisterService( const StubAddress & stub, ITEM_ID source );
    /**
     * \brief   NEConnection::createRouterRegisterClient
     *          Initializes Proxy register message at router.
     * \param   proxy   The address of remote Proxy, which is registering.
     * \param   source  The ID of source, which initiates message.
     * \return  Returns register proxy message to send.
     * \see     createRouterUnregisterClient
     **/
    RemoteMessage createRouterRegisterClient( const ProxyAddress & proxy, ITEM_ID source );
    /**
     * \brief   NEConnection::createRouterUnregisterClient
     *          Initializes Proxy unregister message at router.
     * \param   proxy   The address of remote Proxy, which is registering.
     * \param   source  The ID of source, which initiates message.
     * \return  Returns unregister proxy message to send.
     * \see     createRouterRegisterClient
     **/
    RemoteMessage createRouterUnregisterClient( const ProxyAddress & proxy, ITEM_ID source );
    /**
     * \brief   NEConnection::createServiceRegisteredNotification
     *          Initializes Stub registered notification message to broadcast.
     * \param   stub    The address of remote Stub to notify registering.
     * \param   target  The ID of target to notify message.
     * \return  Returns registered stub notification message to broadcast.
     * \see     createServiceUnregisteredNotification
     **/
    RemoteMessage createServiceRegisteredNotification( const StubAddress & stub, ITEM_ID target );
    /**
     * \brief   NEConnection::createServiceUnregisteredNotification
     *          Initializes Stub service unregistering notification message to broadcast.
     * \param   stub    The address of remote Stub to notify unregistering.
     * \param   target  The ID of target to notify message.
     * \return  Returns unregister stub notification message to broadcast.
     * \see     createServiceRegisteredNotification
     **/
    RemoteMessage createServiceUnregisteredNotification( const StubAddress & stub, ITEM_ID target );
    /**
     * \brief   NEConnection::createServiceClientRegisteredNotification
     *          Initializes service proxy registering notification message to broadcast.
     * \param   proxy   The address of remote Proxy to notify registering.
     * \param   target  The ID of target to notify message.
     * \return  Returns service proxy registering notification message to broadcast.
     * \see     createServiceClientUnregisteredNotification
     **/
    RemoteMessage createServiceClientRegisteredNotification( const ProxyAddress & proxy, ITEM_ID target );
    /**
     * \brief   NEConnection::createServiceClientUnregisteredNotification
     *          Initializes service proxy unregistering notification message to broadcast.
     * \param   proxy   The address of remote Proxy to notify unregistering.
     * \param   target  The ID of target to notify message.
     * \return  Returns service proxy unregistering notification message to broadcast
     * \see     createServiceClientRegisteredNotification
     **/
    RemoteMessage createServiceClientUnregisteredNotification( const ProxyAddress & proxy, ITEM_ID target );
    /**
     * \brief   NEConnection::isMessageHelloServer
     *          Checks whether specified message is a connect request.
     * \param   msgHelloServer      The instance of remote message to check.
     * \return  Returns true if the message is connect request.
     **/
    bool isMessageHelloServer( const RemoteMessage & msgHelloServer );
    /**
     * \brief   NEConnection::isMessageByeServer
     *          Checks whether specified message is a disconnect request.
     * \param   msgByeServer        The instance of remote message to check.
     * \return  Returns true if the message is disconnect request.
     **/
    bool isMessageByeServer( const RemoteMessage & msgByeServer );
    /**
     * \brief   NEConnection::isMessagNotifyClient
     *          Checks whether specified message is client notification request.
     * \param   msgNotifyClient     The instance of remote message to check.
     * \return  Returns true if the message is a client notification request.
     **/
    bool isMessagNotifyClient( const RemoteMessage & msgNotifyClient );
    /**
     * \brief   NEConnection::isMessageRegisterService
     *          Checks whether specified message is service notification request.
     * \param   msgRegisterService  The instance of remote message to check.
     * \return  Returns true if the message is a service notification request.
     **/
    bool isMessageRegisterService( const RemoteMessage & msgRegisterService );
}

#endif  // AREG_IPC_PRIVATE_NECONNECTION_HPP
