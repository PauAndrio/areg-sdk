/************************************************************************
 * \file        areg/trace/private/DebugOutputLogger.cpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Debug Output Logger object to log message into the
 *              system Output console object
 ************************************************************************/
#include "areg/trace/private/DebugOutputLogger.hpp"

#include "areg/trace/private/IETraceConfiguration.hpp"
#include "areg/trace/private/TraceProperty.hpp"
#include "areg/base/Process.hpp"
#include "areg/base/DateTime.hpp"
#include "areg/base/IEByteBuffer.hpp"
#include "areg/base/NEUtilities.hpp"
#include "areg/base/WideString.hpp"
#include "areg/base/String.hpp"

#include "areg/base/private/NEDebug.hpp"

DebugOutputLogger::DebugOutputLogger( IETraceConfiguration & tracerConfig )
    : LoggerBase        ( tracerConfig )
    , IEOutStream       ( )

    , mIsOpened         ( false )
    , mOutputMessageA   ( )
{
    ; // do nothing
}

DebugOutputLogger::~DebugOutputLogger(void)
{
    ; // do nothing
}

bool DebugOutputLogger::openLogger(void)
{
#if defined(_OUTPUT_DEBUG)
    if ( mIsOpened == false )
    {
        const IETraceConfiguration & traceConfig = getTraceConfiguration();
        ASSERT( static_cast<bool>(traceConfig.propertyStatus().getValue()) );
        
        const TraceProperty & prop = traceConfig.propertyDebugOutput();
        if ( prop.isValid() && static_cast<bool>(prop.getValue()) )
        {

            mIsOpened = createLayouts( );

            if ( mIsOpened )
            {
                Process & curProcess = Process::getInstance();
                NETrace::sLogMessage logMsgHello;
                NEMemory::zeroData<NETrace::sLogMessage>(logMsgHello);

                logMsgHello.lmHeader.logLength      = sizeof(NETrace::sLogMessage);
                logMsgHello.lmHeader.logType        = NETrace::LogMessage;
                logMsgHello.lmHeader.logModuleId    = 0;

                logMsgHello.lmTrace.traceThreadId   = 0;
                logMsgHello.lmTrace.traceScopeId    = 0;
                logMsgHello.lmTrace.traceTimestamp  = DateTime::getNow();
                logMsgHello.lmTrace.traceMessagePrio= NETrace::PrioIgnoreLayout;
                String::formatString( logMsgHello.lmTrace.traceMessage
                                    , NETrace::LOG_MESSAGE_BUFFER_SIZE
                                    , LoggerBase::FOMAT_MESSAGE_HELLO
                                    , Process::getString(curProcess.getEnvironment())
                                    , curProcess.getFullPath()
                                    , curProcess.getId());

                logMessage(logMsgHello);
            }
        }
        else
        {
            ; // no property was set
        }
    }
#endif  // !defined(_OUTPUT_DEBUG)

    return mIsOpened;
}

void DebugOutputLogger::closeLogger(void)
{
#if defined(_OUTPUT_DEBUG)
    if ( mIsOpened )
    {
        Process & curProcess = Process::getInstance();
        NETrace::sLogMessage logMsgHello;
        NEMemory::zeroData<NETrace::sLogMessage>(logMsgHello);

        logMsgHello.lmHeader.logLength      = sizeof(NETrace::sLogMessage);
        logMsgHello.lmHeader.logType        = NETrace::LogMessage;
        logMsgHello.lmHeader.logModuleId    = 0;

        logMsgHello.lmTrace.traceThreadId   = 0;
        logMsgHello.lmTrace.traceScopeId    = 0;
        logMsgHello.lmTrace.traceTimestamp  = DateTime::getNow();
        logMsgHello.lmTrace.traceMessagePrio= NETrace::PrioIgnoreLayout;
        String::formatString( logMsgHello.lmTrace.traceMessage
                            , NETrace::LOG_MESSAGE_BUFFER_SIZE
                            , LoggerBase::FORMAT_MESSAGE_BYE
                            , Process::getString(curProcess.getEnvironment())
                            , curProcess.getFullPath()
                            , curProcess.getId());

        logMessage(logMsgHello);
    }
#endif  // !defined(_OUTPUT_DEBUG)

    releaseLayouts();
    mIsOpened = false;
}

#if defined(_OUTPUT_DEBUG)
bool DebugOutputLogger::logMessage(const NETrace::sLogMessage & logMessage)
{
    bool result = false;

    if ( mIsOpened )
    {
        switch (logMessage.lmHeader.logType)
        {
        case NETrace::LogMessage:
            result = getLayoutMessage().logMessage(logMessage, static_cast<IEOutStream &>(*this));
            break;

        case NETrace::LogScopeEnter:
            result = getLayoutEnterScope().logMessage( logMessage, static_cast<IEOutStream &>(*this) );
            break;

        case NETrace::LogScopeExit:
            result = getLayoutExitScope().logMessage( logMessage, static_cast<IEOutStream &>(*this) );
            break;

        default:
            ASSERT(false);  // unexpected message to log
            break;
        }

        flush();
    }
    return result;
}

#else // !defined(_OUTPUT_DEBUG)

bool DebugOutputLogger::logMessage(const NETrace::sLogMessage & /*logMessage*/)
{
    return false;
}

#endif // !defined(_OUTPUT_DEBUG)


bool DebugOutputLogger::isLoggerOpened(void) const
{
    return mIsOpened;
}

unsigned int DebugOutputLogger::write(const unsigned char * /*buffer*/, unsigned int size)
{
    return size;
}

unsigned int DebugOutputLogger::write(const IEByteBuffer & buffer)
{
    return buffer.getSizeUsed();
}

unsigned int DebugOutputLogger::write( const String & asciiString )
{
#if defined(_OUTPUT_DEBUG)
    mOutputMessageA += asciiString;
#endif  // defined(_OUTPUT_DEBUG)
    return asciiString.getUsedSpace();
}

unsigned int DebugOutputLogger::write( const WideString & wideString )
{
#if defined(_OUTPUT_DEBUG)
    mOutputMessageA += wideString;
#endif  // !defined(_OUTPUT_DEBUG)
    return wideString.getUsedSpace();
}

void DebugOutputLogger::flush(void)
{
#if defined(_OUTPUT_DEBUG)
    NEDebug::outputMessageOS(mOutputMessageA.getString());
#endif // !defined(_OUTPUT_DEBUG)

    mOutputMessageA.clear();
}

unsigned int DebugOutputLogger::getSizeWritable(void) const
{
    return static_cast<unsigned int>(0xFFFF);
}

void DebugOutputLogger::flushLogs(void)
{
    flush( );
}
