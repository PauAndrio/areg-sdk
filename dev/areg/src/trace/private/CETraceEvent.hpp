#ifndef AREG_TRACE_PRIVATE_CETRACEEVENT_HPP
#define AREG_TRACE_PRIVATE_CETRACEEVENT_HPP
/************************************************************************
 * \file        areg/src/trace/private/CETraceEvent.hpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, The logging thread, which is receiving logging events and performs log operations. 
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/src/base/GEGlobal.h"
#include "areg/src/component/TEEvent.hpp"
#include "areg/src/base/CESharedBuffer.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class CELogMessage;

//////////////////////////////////////////////////////////////////////////
// CETraceEventData class declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   The tracing (or logging) event is triggered each time when application
 *          needs a logging action to be performed. The event is forwarded to a logging thread
 *          for further processing, like configuring logging, output message, etc.
 *          The logging event contains data with the instruction to perform certain actions.
 *          This logging event data is automatically generated by system, when a certain API
 *          is called.
 **/
class AREG_API CETraceEventData
{
//////////////////////////////////////////////////////////////////////////
// Internal types and constants
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   The list of supported actions for logging
     **/
    typedef enum E_TraceAction
    {
          TraceUndefined                //!< Action is undefined, do nothing
        , TraceConfigure                //!< Action to notify to configure logging
        , TraceChangeConfig             //!< Action to notify to change configuration
        , TraceStartLogs                //!< Action to notify to start logging
        , TraceStopLogs                 //!< Action to notify to stop logging
        , TraceEnableLogs               //!< Action to notify to enable logging
        , TraceDisableLogs              //!< Action to notify to stop logging
        , TraceChangeScopes             //!< Action to notify the scopes have changes
        , TraceSaveScopes               //!< Action to notify to save scope list
        , TraceMessage                  //!< Action to output logging message
        , TraceThreadRegistered         //!< Action to notify the logging thread registered
        , TraceThreadUnregistered       //!< Action to notify the logging thread unregistered
        , TraceChangeStack              //!< Action to notify the to change logging stack size
        , TraceProcessModuleId          //!< Action to set process module ID
    } eTraceAction;

    /**
     * \brief   Converts and returns the string of CETraceEventData::eTraceAction value
     **/
    static inline const char * GetString( CETraceEventData::eTraceAction action );

//////////////////////////////////////////////////////////////////////////
// Constructors / Destructor
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Creates the logging even data with undefined action
     **/
    CETraceEventData( void );

    /**
     * \brief   Creates the logging even data with specified action
     * \param   action  The action ID to set in event data
     **/
    CETraceEventData( CETraceEventData::eTraceAction action );
    
    /**
     * \brief   Creates the logging even data with specified action and data
     * \param   action      The action ID to set in event data
     * \param   dataBuffer  The buffer of data set.
     **/
    CETraceEventData( CETraceEventData::eTraceAction action, const CESharedBuffer & dataBuffer );

    /**
     * \brief   Creates the logging even data with specified action and logging message data
     * \param   action  The action ID to set in event data
     * \param   logData The buffer of logging message data set.
     **/
    CETraceEventData( CETraceEventData::eTraceAction action, const CELogMessage & logData );

    /**
     * \brief   Copies logging event data from given source.
     * \param   src     The source to copy data.
     **/
    CETraceEventData( const CETraceEventData & src );

    /**
     * \brief   Destructor
     **/
    ~CETraceEventData( void );

//////////////////////////////////////////////////////////////////////////
// Operators
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Copies data from given source
     * \param   src     The source of data to copy.
     **/
    const CETraceEventData & operator = ( const CETraceEventData & src );

//////////////////////////////////////////////////////////////////////////
// Attributes
//////////////////////////////////////////////////////////////////////////
public:

    /**
     * \brief   Returns the value of action set in event data.
     **/
    inline CETraceEventData::eTraceAction GetTraceAction( void ) const;

    /**
     * \brief   Returns the streaming buffer for writing.
     **/
    inline CESharedBuffer & GetWritableStream( void );

    /**
     * \brief   Returns the streaming buffer for reading.
     **/
    inline const CESharedBuffer & GetReadableStream( void ) const;

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
private:
    CETraceEventData::eTraceAction  mAction;
    CESharedBuffer                  mDataBuffer;
};

//////////////////////////////////////////////////////////////////////////
// Tracing Event and Consumer declaration
//////////////////////////////////////////////////////////////////////////

/**
 * \brief   Declare CETraceEvent class, derived from TEEvent object,
 *          and IETraceEventConsumer class, derived from TEEventConsumer object
 **/
DECLARE_EVENT(CETraceEventData, CETraceEvent, IETraceEventConsumer)

//////////////////////////////////////////////////////////////////////////
// CETraceEventData class inline functions
//////////////////////////////////////////////////////////////////////////

inline CETraceEventData::eTraceAction CETraceEventData::GetTraceAction( void ) const
{   return mAction;         }

inline CESharedBuffer & CETraceEventData::GetWritableStream( void )
{   return mDataBuffer;     }

inline const CESharedBuffer & CETraceEventData::GetReadableStream( void ) const
{   return mDataBuffer;     }

inline const char * CETraceEventData::GetString( CETraceEventData::eTraceAction action )
{
    switch ( action )
    {
    CASE_MAKE_STRING(CETraceEventData::TraceUndefined);
    CASE_MAKE_STRING(CETraceEventData::TraceConfigure);
    CASE_MAKE_STRING(CETraceEventData::TraceChangeConfig);
    CASE_MAKE_STRING(CETraceEventData::TraceStartLogs);
    CASE_MAKE_STRING(CETraceEventData::TraceStopLogs);
    CASE_MAKE_STRING(CETraceEventData::TraceEnableLogs);
    CASE_MAKE_STRING(CETraceEventData::TraceDisableLogs);
    CASE_MAKE_STRING(CETraceEventData::TraceChangeScopes);
    CASE_MAKE_STRING(CETraceEventData::TraceSaveScopes);
    CASE_MAKE_STRING(CETraceEventData::TraceMessage);
    CASE_MAKE_STRING(CETraceEventData::TraceThreadRegistered);
    CASE_MAKE_STRING(CETraceEventData::TraceThreadUnregistered);
    CASE_MAKE_STRING(CETraceEventData::TraceChangeStack);
    CASE_DEFAULT("ERR: Undefined CETraceEventData::eTraceAction value!");
    }
}

#endif  // AREG_TRACE_PRIVATE_CETRACEEVENT_HPP