//////////////////////////////////////////////////////////////////////////
// Begin generate shared/generated/private/CECentralMessagerEvents.cpp file
//////////////////////////////////////////////////////////////////////////

/************************************************************************
 * (c) copyright    2019
 *                  Create by AREGtech code generator tool from source CentralMessager.
 * Generated at     03.09.2019  02:48:07 GMT+02:00 
 ************************************************************************/

/************************************************************************
 * \file            shared/generated/private/CECentralMessagerEvents.cpp
 * \ingroup         CentralMessager Service Interface
 * \brief           This is an automatic generated code of CentralMessager Service Interface Event classes implementation.
 ************************************************************************/

/************************************************************************
 * Include files
 ************************************************************************/
 #include "shared/generated/private/CECentralMessagerEvents.hpp"
 
//////////////////////////////////////////////////////////////////////////
// CECentralMessagerRequestEvent class implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerRequestEvent class, Runtime implementation
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   CECentralMessagerRequestEvent Event runtime implementation
 **/
IMPLEMENT_RUNTIME_EVENT( CECentralMessagerRequestEvent, CERemoteRequestEvent );

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerRequestEvent class, Constructors / Destructor
//////////////////////////////////////////////////////////////////////////

CECentralMessagerRequestEvent::CECentralMessagerRequestEvent( const CEProxyAddress & fromSource, const CEStubAddress & toTarget, const unsigned int reqId )
    : CERemoteRequestEvent( fromSource, toTarget, reqId )
{
    ; // do nothing
}

CECentralMessagerRequestEvent::CECentralMessagerRequestEvent( const CEEventDataStream & args, const CEProxyAddress & fromSource, const CEStubAddress & toTarget, const unsigned int reqId )
    : CERemoteRequestEvent( args, fromSource, toTarget, reqId, NECentralMessager::GetString( static_cast<const NECentralMessager::eMessageIDs>(reqId) ) )
{
    ; // do nothing
}

CECentralMessagerRequestEvent::CECentralMessagerRequestEvent( const IEInStream & stream )
    : CERemoteRequestEvent( stream )
{
    ; // do nothing
}    

CECentralMessagerRequestEvent::~CECentralMessagerRequestEvent( void )
{
    ; // do nothing
}

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerNotifyRequestEvent class implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerNotifyRequestEvent class, Runtime implementation
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_RUNTIME_EVENT( CECentralMessagerNotifyRequestEvent, CERemoteNotifyRequestEvent );

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerNotifyRequestEvent class, Constructors / Destructor
//////////////////////////////////////////////////////////////////////////

CECentralMessagerNotifyRequestEvent::CECentralMessagerNotifyRequestEvent( const CEProxyAddress & fromProxy, const CEStubAddress & toStub, const unsigned int msgId, const NEService::eRequestType & reqType )
    : CERemoteNotifyRequestEvent( fromProxy, toStub, msgId, reqType )
{
    ; // do nothing
}

CECentralMessagerNotifyRequestEvent::CECentralMessagerNotifyRequestEvent( const IEInStream & stream )
    : CERemoteNotifyRequestEvent( stream )
{
    ; // do nothing
}    

CECentralMessagerNotifyRequestEvent::~CECentralMessagerNotifyRequestEvent( void )
{
    ; // do nothing
}

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerResponseEvent class implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerResponseEvent class, Runtime implementation
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_RUNTIME_EVENT( CECentralMessagerResponseEvent, CERemoteResponseEvent );

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerResponseEvent class, Constructors / Destructor
//////////////////////////////////////////////////////////////////////////

CECentralMessagerResponseEvent::CECentralMessagerResponseEvent( const CEProxyAddress & proxyTarget, const NEService::eResultType & result, const unsigned int responseId, unsigned int seqNr /* = NEService::SEQUENCE_NUMBER_NOTIFY */ )
    : CERemoteResponseEvent( proxyTarget, result, responseId, seqNr )
{
    ; // do nothing
}

CECentralMessagerResponseEvent::CECentralMessagerResponseEvent( const CEEventDataStream & args, const CEProxyAddress & proxyTarget, const NEService::eResultType & result, const unsigned int responseId, unsigned int seqNr /* = NEService::SEQUENCE_NUMBER_NOTIFY */ )
    : CERemoteResponseEvent( args, proxyTarget, result, responseId, seqNr, NECentralMessager::GetString(static_cast<const NECentralMessager::eMessageIDs>(responseId)) )
{
    ; // do nothing
}

CECentralMessagerResponseEvent::CECentralMessagerResponseEvent( const CEProxyAddress & proxyTarget, const CERemoteResponseEvent & source )
    : CERemoteResponseEvent( proxyTarget, source )
{
    ; // do nothing
}

CECentralMessagerResponseEvent::CECentralMessagerResponseEvent( const IEInStream & stream )
    : CERemoteResponseEvent( stream )
{
    ; // do nothing
}    
CECentralMessagerResponseEvent::~CECentralMessagerResponseEvent( void )
{
    ; // do nothing
}

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerResponseEvent class, Methods
//////////////////////////////////////////////////////////////////////////

CEServiceResponseEvent* CECentralMessagerResponseEvent::CloneEventForTarget( const CEProxyAddress & target ) const
{
    return static_cast<CEServiceResponseEvent *>( DEBUG_NEW CECentralMessagerResponseEvent(target, static_cast<const CERemoteResponseEvent &>(*this)) );
}

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerNotificationEvent class implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerNotificationEvent class, Runtime implementation
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_RUNTIME_EVENT( CECentralMessagerNotificationEvent, CENotificationEvent );

//////////////////////////////////////////////////////////////////////////
// CECentralMessagerNotificationEvent class, Constructors / Destructor
//////////////////////////////////////////////////////////////////////////

CECentralMessagerNotificationEvent::CECentralMessagerNotificationEvent( const CENotificationEventData & data )
    : CENotificationEvent   ( data )
{
    ; // do nothing
}

CECentralMessagerNotificationEvent::~CECentralMessagerNotificationEvent( void )
{
    ; // do nothing
}

//////////////////////////////////////////////////////////////////////////
// End generate shared/generated/private/CECentralMessagerEvents.cpp file
//////////////////////////////////////////////////////////////////////////