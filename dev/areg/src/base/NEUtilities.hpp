#ifndef AREG_BASE_NEUTILITIES_HPP
#define AREG_BASE_NEUTILITIES_HPP
/************************************************************************
 * \file        areg/src/base/NEUtilities.hpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Utilities
 *              Helper functions and constants.
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/src/base/GEGlobal.h"
#include "areg/src/base/NEMath.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class CEString;
class IEInStream;
class IEOutStream;

struct tm;

//////////////////////////////////////////////////////////////////////////
// NEUtilities namespace declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   Collection of helper functions and constants.
 *          For details see description of constants and functions.
 *
 **/
namespace   NEUtilities
{
/************************************************************************/
// NEUtilities namespace predefined constants
/************************************************************************/

    /**
     * \brief   NEUtilities::MAX_GENERATED_NAME_BUFFER_SIZE
     *          Constants. Predefined value, the maximum length of name
     *          generated by system.
     **/
    const int                   MAX_GENERATED_NAME_BUFFER_SIZE   = 0xFF;

    /**
     * \brief   NEUtilities::DEFAULT_GENERATED_NAME
     *          Constant. Predefined default prefix for generated
     *          by system names.
     **/
    const char* const           DEFAULT_GENERATED_NAME      = "Name";
    /**
     * \brief   NEUtilities::COMPONENT_PATH_SEPARATOR
     *          Constant. Predefined path separator for address of
     *          threads, components, proxies or stubs.
     **/
    const char* const           COMPONENT_PATH_SEPARATOR    = "::";
    /**
     * \brief   NEUtilities::COMPONENT_ITEM_SEPARATOR
     *          Constant. Predefined path separator of component named item.
     *          The name items can be Worker threads and/or timers of
     *          component. If Service Interface requires worker threads
     *          and Service Interface is used in different components with
     *          different role names, the named items can differ by
     *          unique role name contained in the named items.
     **/
    const char* const           COMPONENT_ITEM_SEPARATOR    = ":";
    /**
     * \brief   NEUtilities::DEFAULT_SPECIAL_CHAR
     *          Constant. Defines default special character used in generated names.
     **/
    const char* const           DEFAULT_SPECIAL_CHAR        = "-";
    /**
     * \brief   NEUtilities::ITEM_NAMES_MAX_LENGTH
     *          The maximum length of item names such as thread name, component or service names
     **/
    const unsigned int          ITEM_NAMES_MAX_LENGTH       = 64;

    /**
     * \brief   
     **/
    const uint64_t              WIN_TO_POSIX_EPOCH_BIAS_MICROSECS   = 11644473600000000LL;

    const uint64_t              SEC_TO_MICROSECS                    = 1000000LL;

    const uint64_t              SEC_TO_MILLISECS                    = 1000LL;

    const uint64_t              MICROSEC_TO_100NS                   = 10LL;

    const uint64_t              MICROSEC_TO_NS                      = 1000LL;

    const uint64_t              MILLISEC_TO_NS                      = 1000000LL;

    const uint64_t              MILLISEC_TO_MICROSECS               = 1000LL;

/************************************************************************/
// NEUtilities namespace utility types
/************************************************************************/
    
    /**
     * \brief   NEUtilities::sSystemTime
     *          The structure defines date-time data used in system time
     **/
    typedef struct S_SystemTime
    {
        unsigned short  stYear;         //!< The year in the time
        unsigned short  stMonth;        //!< The month in the time, which starts from 1 (January) until 12 (December)
        unsigned short  stDayOfWeek;    //!< The day of week in the time, wich starts from 0 (Sunday) until 6 (Saturday)
        unsigned short  stDay;          //!< The day of the month in the time, which starts from 1
        unsigned short  stHour;         //!< The hour in the time, which starts from 0
        unsigned short  stMinute;       //!< The minute in the time, which starts from 0
        unsigned short  stSecond;       //!< The second in the time, which starts from 0
        unsigned short  stMillisecs;    //!< The millisecond in the time, which starts from 0
        unsigned short  stMicrosecs;    //<! The microseconds in the time, which starts from 0
    } sSystemTime;

    /**
     * \brief   NEUtilities::sFileTime
     *          Specifies file time type and format.
     **/
    typedef struct S_FileTime
    {
        unsigned long   ftLowDateTime;  //!< Low part of date-time.
        unsigned long   ftHighDateTime; //!< High part of date-time.
    } sFileTime;

    /**
     * \brief   Returns current time. On output 'out_sysTime' system time contains the date-time data.
     * \param   out_sysTime     On output the system time parameter contains date-time of current time.
     **/
    AREG_API void GetSystemTimeNow( sSystemTime & out_sysTime );

    /**
     * \brief   Returns current time. On output 'out_fileTime' time contains the date-time data.
     * \param   out_fileTime    On output the file-time parameter contains date-time of current time.
     **/
    AREG_API void GetSystemTimeNow( sFileTime & out_fileTime );

    /**
     * \brief   Returns current system time data as a 64-bit integer value. The returned value is
     *          passed microseconds since January 1, 1970 (UNIX epoch).
     * \return  Returns microseconds passed since January 1, 1970 (UNIX epoch).
     **/
    AREG_API TIME64 GetSystemTimeNow( void );

    /**
     * \brief   Returns system time data as a 64-bit integer value. The returned value is
     *          passed microseconds since January 1, 1970 (UNIX epoch).
     * \param   sysTime     The system time structure with data to convert.
     * \return  Returns microseconds passed since January 1, 1970 (UNIX epoch).
     **/
    AREG_API TIME64 ConvertSystemTime( const sSystemTime & sysTime );

    /**
     * \brief   Returns file time data as a 64-bit integer value. The returned value is
     *          passed microseconds since January 1, 1970 (UNIX epoch).
     * \param   fileTime    The system time structure with data to convert.
     * \return  Returns microseconds passed since January 1, 1970 (UNIX epoch).
     **/
    AREG_API TIME64 ConvertFileTime( const sFileTime & fileTime );

    /**
     * \brief   Converts 64-bit value of microseconds passed since January 1 1970 into system time data structure.
     * \param   timeValue       64-bit value as microseconds passed since January 1 1970.
     * \param   out_sysTime     On output the system time parameter contains date-time of converted time.
     **/
    AREG_API void ConvertToSystemTime( const TIME64 &  timeValue, sSystemTime & out_sysTime );

    /**
     * \brief   Converts 64-bit value of microseconds passed since January 1 1970 into file time data structure.
     * \param   timeValue       64-bit value as microseconds passed since January 1 1970.
     * \param   out_fileTime    On output the file-time parameter contains date-time of converted time.
     **/
    AREG_API void ConvertToFileTime( const TIME64 &  timeValue, sFileTime & out_fileTime );

    /**
     * \brief   Converts given file-time data structure into system time data structure.
     * \param   fileTime        The file-time data structure to convert.
     * \param   out_sysTime     On output the system time parameter contains date-time of converted time.
     **/
    AREG_API void ConvertFileTimeToSystemTime( const sFileTime & fileTime, sSystemTime & out_sysTime );

    /**
     * \brief   Converts given system-time data structure into file-time data structure.
     * \param   sysTime         The system-time data structure to convert.
     * \param   out_fileTime    On output the file-time parameter contains date-time of converted time.
     **/
    AREG_API void ConvertSystemTimeToFileTime( const sSystemTime & sysTime, sFileTime & out_fileTime );

    /**
     * \brief   Compare 2 system-time data structures and returns result indicating equality of data.
     * \param   lhs     Left-Hand Operand to compare
     * \param   rhs     Right-Hand Operand to compare
     * \return  Returns one of possible results: 
     *              - NEMath::CompSmaller if Left-Hand Operand 'lhs' is smaller than Right-Hand Operand 'rhs'
     *              - NEMath::CompEqual if both operands are equal
     *              - NEMath::CompGreater  if Left-Hand Operand 'lhs' is greater than Right-Hand Operand 'rhs'
     **/
    AREG_API NEMath::eCompare CompareSystemTimes( const sSystemTime & lhs, const sSystemTime & rhs );

    /**
     * \brief   Compare 2 file-time data structures and returns result indicating equality of data.
     * \param   lhs     Left-Hand Operand to compare
     * \param   rhs     Right-Hand Operand to compare
     * \return  Returns one of possible results: 
     *              - NEMath::CompSmaller if Left-Hand Operand 'lhs' is smaller than Right-Hand Operand 'rhs'
     *              - NEMath::CompEqual if both operands are equal
     *              - NEMath::CompGreater  if Left-Hand Operand 'lhs' is greater than Right-Hand Operand 'rhs'
     **/
    AREG_API NEMath::eCompare CompareFileTimes( const sFileTime & lhs, const sFileTime & rhs );

    /**
     * \brief   Compare 2 64-bit time values and returns result indicating equality of data. The given 64-values
     *          are microseconds passed since 1 January 1970.
     * \param   lhs     Left-Hand Operand to compare
     * \param   rhs     Right-Hand Operand to compare
     * \return  Returns one of possible results: 
     *              - NEMath::CompSmaller if Left-Hand Operand 'lhs' is smaller than Right-Hand Operand 'rhs'
     *              - NEMath::CompEqual if both operands are equal
     *              - NEMath::CompGreater  if Left-Hand Operand 'lhs' is greater than Right-Hand Operand 'rhs'
     **/
    AREG_API NEMath::eCompare CompareTimes( const TIME64 & lhs, const TIME64 & rhs );

    /**
     * \brief   Converts system-time data structure to standard 'tm' type. In conversion, a milliseconds part of data will be lost.
     * \param   sysTime     The system-time data structure to convert.
     * \param   out_time    On output the parameter contains date-time of converted system time without information of milliseconds.
     **/
    AREG_API void SysTimeToTime( const sSystemTime & sysTime, tm & out_time );

    /**
     * \brief   Converts standard 'tm' type to system-time data structure. In conversion, a milliseconds part of data will not exist.
     * \param   time            Contains date-time of converted system time without information of milliseconds.
     * \param   out_sysTime     On output, the parameter contains date-time information in system-time data structure format without millisecond information.
     **/
    AREG_API void TimeToSysTime( const tm & time, sSystemTime & out_sysTime );

    /**
     * \brief   Returns the tick counts information in milliseconds since process has started.
     **/
    AREG_API uint64_t GetTickCount( void );

    /**
     * \brief   Converts the system UTC time to local time.
     * \param   inUtcTime       The UTC time
     * \param   outLocalTime    On return this structure contains the local time information.
     * \return  Returns true if conversion succeeded.
     **/
    AREG_API bool ToLocalTime( const sSystemTime &inUtcTime, sSystemTime & outLocalTime );

    /**
     * \brief   Converts the system UTC time to local time.
     * \param   inUtcTime       The UTC time in microseconds passed since January 1 1970
     * \param   outLocalTime    On return this structure contains the local time information.
     * \return  Returns true if conversion succeeded.
     **/
    AREG_API bool ToLocalTime( const TIME64 & inUtcTime, sSystemTime & outLocalTime );

/************************************************************************/
// NEUtilities namespace utility functions, generate names
/************************************************************************/
    /**
     * \brief   Creates Component item name separating by COMPONENT_ITEM_SEPARATOR if component name is given.
     *          Otherwise, it will return itemName
     * \param   componentName   The name of item parent component, normally component role name
     * \param   itemName        The name of component item.
     * \return  Returns created new string containing componentName and itemName separated by COMPONENT_ITEM_SEPARATOR.
     **/
    AREG_API CEString CreateComponentItemName(const char * componentName, const char* itemName);

    /**
     * \brief   This function generates and returns name 
     *          using timestamp value in nanoseconds in the name.
     *          If passed prefix is NULL, it will use predefined
     *          NEUtilities::DEFAULT_GENERATED_NAME constant as a
     *          prefix for name. The maximum length of generated
     *          name is NEUtilities::MAX_GENERATED_NAME_BUFFER_SIZE.
     *          To provide uniqueness for generated name, make
     *          sure that the length of passed prefix is not
     *          more than (NEUtilities::MAX_GENERATED_NAME_BUFFER_SIZE - 34)
     * \param   prefix  The prefix to add in generated name.
     *                  If this value is NULL it will use
     *                  NEUtilities::DEFAULT_GENERATED_NAME as
     *                  a prefix for name.
     * \return  Returns system generated unique name.
     **/
    AREG_API CEString GenerateName( const char * prefix );

    /**
     * \brief   This function generates and returns name 
     *          using timestamp value in nanoseconds in the name.
     *          If passed prefix is NULL, it will use predefined
     *          NEUtilities::DEFAULT_GENERATED_NAME constant as a
     *          prefix for name. Before calling this function,
     *          the output buffer should be allocated and the length
     *          of buffer should be big enough to contain prefix
     *          and the timestamp. The timestamp requires at least
     *          34 characters. Generated output name is in 
     *          format <prefix>:{nn:nn:nn:nn:nn:nn:nn:nn})
     * \param   prefix      The prefix to add in generated name.
     *                      If this value is NULL it will use
     *                      NEUtilities::DEFAULT_GENERATED_NAME as
     *                      a prefix for name.
     * \param   out_buffer  The output buffer to fill generated name.
     *                      The length of buffer should be big enough
     *                      to contain prefix and prefix.
     * \param   length      The length of buffer to set name.
     * \param   specChar    Special character used in generated name.
     **/
    AREG_API void GenerateName( const char * prefix, char * out_buffer, int length);
    AREG_API void GenerateName( const char * prefix, char * out_buffer, int length, const char * specChar);

/************************************************************************/
// NEUtilities namespace utility functions, generate unique ID
/************************************************************************/
    /**
     * \brief	Generates and returns unique unsigned int value
     **/
    AREG_API unsigned int GenerateUniqueId( void );
}

#endif  // AREG_BASE_NEUTILITIES_HPP