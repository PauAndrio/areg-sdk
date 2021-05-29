#ifndef AREG_BASE_GEGLOBAL_H
#define AREG_BASE_GEGLOBAL_H
/************************************************************************
 * \file        areg/base/GEGlobal.h
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, global include header file
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/

/**
 * \brief   This header file is included in every file of a project.
 *          The header file contains macro defines for MC Windows OS,
 *          includes standard headers, header of switches 
 *          to use import / exports and standard global macro.
 *          Anything global, which is relevant for all includes,
 *          should be included and/or defined here.
 **/

#include "system/GEPlatform.h"

#define OUTPUT_DEBUG_LEVEL_DEBUG    4
#define OUTPUT_DEBUG_LEVEL_INFO     3
#define OUTPUT_DEBUG_LEVEL_WARN     2
#define OUTPUT_DEBUG_LEVEL_ERROR    1
#define OUTPUT_DEBUG_LEVEL_NONE     0
#define OUTPUT_DEBUG_LEVEL_ALL      OUTPUT_DEBUG_LEVEL_DEBUG

#ifdef _OUTPUT_DEBUG_LEVEL
    #undef  OUTPUT_DEBUG_LEVEL
    #define OUTPUT_DEBUG_LEVEL  _OUTPUT_DEBUG_LEVEL
#endif //!_OUTPUT_DEBUG_LEVEL

#ifdef DEBUG
    #ifndef OUTPUT_DEBUG_LEVEL
        #define OUTPUT_DEBUG_LEVEL      OUTPUT_DEBUG_LEVEL_ALL
    #endif // !OUTPUT_DEBUG_LEVEL
#else
    #undef  OUTPUT_DEBUG_LEVEL
    #define OUTPUT_DEBUG_LEVEL          OUTPUT_DEBUG_LEVEL_NONE
#endif // DEBUG




/**
 * \brief   Basic types
 **/
#include "areg/base/GETypes.h"

/**
 * \brief   AREG core Switches and MACRO
 **/
#include "areg/base/GESwitches.h"
#include "areg/base/GEMacros.h"

#ifdef _DEBUG
    #if ((defined(OUTPUT_DEBUG) && (OUTPUT_DEBUG != 0)) || (defined(_OUTPUT_DEBUG) && (_OUTPUT_DEBUG != 0)))
        
        #undef  OUTPUT_DEBUG
        #undef _OUTPUT_DEBUG
        #define OUTPUT_DEBUG
        #define _OUTPUT_DEBUG

    #else // !((defined(OUTPUT_DEBUG) && (OUTPUT_DEBUG != 0)) || (defined(_OUTPUT_DEBUG) && (_OUTPUT_DEBUG != 0)))

        #undef  OUTPUT_DEBUG
        #undef _OUTPUT_DEBUG
    
    #endif  // !((defined(OUTPUT_DEBUG) && (OUTPUT_DEBUG != 0)) || (defined(_OUTPUT_DEBUG) && (_OUTPUT_DEBUG != 0)))
#else   // _NDEBUG
        #undef  OUTPUT_DEBUG
        #undef _OUTPUT_DEBUG
#endif  // _DEBUG

#endif  // AREG_BASE_GEGLOBAL_H
