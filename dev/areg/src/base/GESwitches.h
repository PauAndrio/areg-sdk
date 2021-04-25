#ifndef AREG_BASE_GESWITCHES_H
#define AREG_BASE_GESWITCHES_H
/************************************************************************
 * \file        areg/src/base/GESwitches.h
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Framework Switches.
 *              This header should be included before using any of 
 *              Framework classes.
 *
 *              1.  To build framework as a shared library (export symbols), define one of preprocessor definitions:
 *                  EXP_AREG_DLL, or _EXP_AREG_DLL, or EXPORT_SHARED_SYMBOLS
 *
 *              2.  To build framework as a static library (export symbols), define one of preprocessor definitions:
 *                  EXP_AREG_LIB, or _EXP_AREG_LIB, or EXPORT_STATIC_SYMBOLS
 *
 *              3.  To link application with shared framework library (import symbols), define one of preprocessor definitions:
 *                  IMP_AREG_DLL, or _IMP_AREG_DLL, or IMPORT_SHARED_SYMBOLS
 *
 *              4.  To link application with static framework library (import symbols), define one of preprocessor definitions:
 *                  IMP_AREG_LIB, or _IMP_AREG_DLL, or IMPORT_STATIC_SYMBOLS
 *
 *              If none of these is defined, by default it will assume "IMP_ARE_DLL"
 *
 ************************************************************************/

/**
 * Do not define AREG_API, this is the job of switcher
 */
#ifdef  AREG_API
   #error   "AREG_API is already defined. Undefine before include this file!"
#endif

#if defined(IMP_AREG_DLL) || defined(_IMP_AREG_DLL) || defined(IMPORT_SHARED_SYMBOLS) || defined(_IMPORT_SHARED_SYMBOLS)

    #undef  IMP_AREG_DLL
    #undef  IMP_AREG_LIB
    #undef  EXP_AREG_DLL
    #undef  EXP_AREG_LIB

    #undef  _IMP_AREG_DLL
    #undef  _IMP_AREG_LIB
    #undef  _EXP_AREG_DLL
    #undef  _EXP_AREG_LIB

    #undef  IMPORT_SHARED_SYMBOLS
    #undef  IMPORT_STATIC_SYMBOLS
    #undef  EXPORT_SHARED_SYMBOLS
    #undef  EXPORT_STATIC_SYMBOLS

    #define IMP_AREG_DLL
    #define _IMP_AREG_DLL

#endif  // defined(IMP_AREG_DLL) || defined(_IMP_AREG_DLL) || defined(IMPORT_SHARED_SYMBOLS)

#if defined(IMP_AREG_LIB) || defined(_IMP_AREG_LIB) || defined(IMPORT_STATIC_SYMBOLS) || defined(_IMPORT_STATIC_SYMBOLS)

    #undef  IMP_AREG_DLL
    #undef  IMP_AREG_LIB
    #undef  EXP_AREG_DLL
    #undef  EXP_AREG_LIB

    #undef  _IMP_AREG_DLL
    #undef  _IMP_AREG_LIB
    #undef  _EXP_AREG_DLL
    #undef  _EXP_AREG_LIB

    #undef  IMPORT_SHARED_SYMBOLS
    #undef  IMPORT_STATIC_SYMBOLS
    #undef  EXPORT_SHARED_SYMBOLS
    #undef  EXPORT_STATIC_SYMBOLS

    #define IMP_AREG_LIB
    #define _IMP_AREG_LIB

#endif  // defined(IMP_AREG_LIB) || defined(_IMP_AREG_LIB) || defined(IMPORT_STATIC_SYMBOLS)

#if defined(EXP_AREG_DLL) || defined(_EXP_AREG_DLL) || defined(EXPORT_SHARED_SYMBOLS) || defined(_EXPORT_SHARED_SYMBOLS)

    #undef  IMP_AREG_DLL
    #undef  IMP_AREG_LIB
    #undef  EXP_AREG_DLL
    #undef  EXP_AREG_LIB

    #undef  _IMP_AREG_DLL
    #undef  _IMP_AREG_LIB
    #undef  _EXP_AREG_DLL
    #undef  _EXP_AREG_LIB

    #undef  IMPORT_SHARED_SYMBOLS
    #undef  IMPORT_STATIC_SYMBOLS
    #undef  EXPORT_SHARED_SYMBOLS
    #undef  EXPORT_STATIC_SYMBOLS

    #define EXP_AREG_DLL
    #define _EXP_AREG_DLL

#endif  // defined(EXP_AREG_DLL) || defined(_EXP_AREG_DLL) || defined(EXPORT_SHARED_SYMBOLS)

#if defined(EXP_AREG_LIB) || defined(_EXP_AREG_LIB) || defined(EXPORT_STATIC_SYMBOLS) || defined(_EXPORT_STATIC_SYMBOLS)

    #undef  IMP_AREG_DLL
    #undef  IMP_AREG_LIB
    #undef  EXP_AREG_DLL
    #undef  EXP_AREG_LIB

    #undef  _IMP_AREG_DLL
    #undef  _IMP_AREG_LIB
    #undef  _EXP_AREG_DLL
    #undef  _EXP_AREG_LIB

    #undef  IMPORT_SHARED_SYMBOLS
    #undef  IMPORT_STATIC_SYMBOLS
    #undef  EXPORT_SHARED_SYMBOLS
    #undef  EXPORT_STATIC_SYMBOLS

    #define EXP_AREG_LIB
    #define _EXP_AREG_LIB

#endif  // defined(EXP_AREG_LIB) || defined(_EXP_AREG_LIB) || defined(EXPORT_STATIC_SYMBOLS)

#if !defined(EXP_AREG_DLL) && !defined(EXP_AREG_LIB) && !defined(IMP_AREG_DLL) && !defined(IMP_AREG_LIB)
    #pragma message ("WARNING: None of preprocessor defines are set. By default use LINK_AREG_API_DLL = 1. See file Switches of AREG API Core")
    #define EXP_AREG_DLL        1
#endif  // !defined(LINK_AREG_API_DLL) && !defined(LINK_AREG_API_LIB) && !defined(IMP_AREG_API_DLL) && !defined(IMP_AREG_API_LIB)

/**
 * \brief   Set Exports / Imports for dynamic and static libraries
 **/
#ifdef EXP_AREG_DLL
   #if (defined(_WINDOWS) || defined(WINDOWS) || defined(_WIN32))
      #define AREG_API       __declspec(dllexport)
   #else
      #define AREG_API
   #endif   // (defined(_WINDOWS) || defined(WINDOWS) || defined(_WIN32))
#endif   // LINK_AREG_DLL

#ifdef IMP_AREG_DLL
   #if (defined(_WINDOWS) || defined(WINDOWS) || defined(_WIN32))
      #define AREG_API       __declspec(dllimport)
   #else
      #define AREG_API
   #endif   // (defined(_WINDOWS) || defined(WINDOWS) || defined(_WIN32))
#endif   // IMP_AREG_DLL

#if defined(EXP_AREG_LIB) || defined(IMP_AREG_LIB)
    #define AREG_API
#endif // (EXP_AREG_LIB)


#ifndef AREG_API
   #define AREG_API
#endif  // AREG_API

#endif  // AREG_BASE_GESWITCHES_H