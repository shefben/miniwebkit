/*
**********************************************************************
*   Copyright (C) 1996-2008, International Business Machines
*   Corporation and others.  All Rights Reserved.
**********************************************************************
*
*  FILE NAME : UTYPES.H (formerly ptypes.h)
*
*   Date        Name        Description
*   12/11/96    helena      Creation.
*   02/27/97    aliu        Added typedefs for UClassID, int8, int16, int32,
*                           uint8, uint16, and uint32.
*   04/01/97    aliu        Added XP_CPLUSPLUS and modified to work under C as
*                            well as C++.
*                           Modified to use memcpy() for uprv_arrayCopy() fns.
*   04/14/97    aliu        Added TPlatformUtilities.
*   05/07/97    aliu        Added import/export specifiers (replacing the old
*                           broken EXT_CLASS).  Added version number for our
*                           code.  Cleaned up header.
*    6/20/97    helena      Java class name change.
*   08/11/98    stephen     UErrorCode changed from typedef to enum
*   08/12/98    erm         Changed T_ANALYTIC_PACKAGE_VERSION to 3
*   08/14/98    stephen     Added uprv_arrayCopy() for int8_t, int16_t, int32_t
*   12/09/98    jfitz       Added BUFFER_OVERFLOW_ERROR (bug 1100066)
*   04/20/99    stephen     Cleaned up & reworked for autoconf.
*                           Renamed to utypes.h.
*   05/05/99    stephen     Changed to use <inttypes.h>
*   12/07/99    helena      Moved copyright notice string from ucnv_bld.h here.
*******************************************************************************
*/

#ifndef UTYPES_H_1
#define UTYPES_H_1


#include "unicode/umachine.h"
#include "unicode/utf.h"
#include "unicode/uversion.h"
#include "unicode/uconfig.h"

#if !U_DEFAULT_SHOW_DRAFT && !defined(U_SHOW_DRAFT_API)
#define U_HIDE_DRAFT_API 1
#endif

#ifdef U_HIDE_DRAFT_API
#include "unicode/udraft.h"
#endif

#ifdef U_HIDE_DEPRECATED_API
#include "unicode/udeprctd.h"
#endif

#ifdef U_HIDE_DEPRECATED_API
#include "unicode/uobslete.h"
#endif

#ifdef U_HIDE_INTERNAL_API
#include "unicode/uintrnal.h"
#endif

#ifdef U_HIDE_SYSTEM_API
#include "unicode/usystem.h"
#endif

/*!
 * \file
 * \brief Basic definitions for ICU, for both C and C++ APIs
 *
 * This file defines basic types, constants, and enumerations directly or
 * indirectly by including other header files, especially utf.h for the
 * basic character and string definitions and umachine.h for consistent
 * integer and other types.
 */

/*===========================================================================*/
/* char Character set family                                                 */
/*===========================================================================*/

/**
 * U_CHARSET_FAMILY is equal to this value when the platform is an ASCII based platform.
 * @stable ICU 2.0
 */
#define U_ASCII_FAMILY 0

/**
 * U_CHARSET_FAMILY is equal to this value when the platform is an EBCDIC based platform.
 * @stable ICU 2.0
 */
#define U_EBCDIC_FAMILY 1

/**
 * \def U_CHARSET_FAMILY
 *
 * <p>These definitions allow to specify the encoding of text
 * in the char data type as defined by the platform and the compiler.
 * It is enough to determine the code point values of "invariant characters",
 * which are the ones shared by all encodings that are in use
 * on a given platform.</p>
 *
 * <p>Those "invariant characters" should be all the uppercase and lowercase
 * latin letters, the digits, the space, and "basic punctuation".
 * Also, '\\n', '\\r', '\\t' should be available.</p>
 *
 * <p>The list of "invariant characters" is:<br>
 * \code
 *    A-Z  a-z  0-9  SPACE  "  %  &amp;  '  (  )  *  +  ,  -  .  /  :  ;  <  =  >  ?  _
 * \endcode
 * <br>
 * (52 letters + 10 numbers + 20 punc/sym/space = 82 total)</p>
 *
 * <p>This matches the IBM Syntactic Character Set (CS 640).</p>
 *
 * <p>In other words, all the graphic characters in 7-bit ASCII should
 * be safely accessible except the following:</p>
 *
 * \code
 *    '\' <backslash>
 *    '[' <left bracket>
 *    ']' <right bracket>
 *    '{' <left brace>
 *    '}' <right brace>
 *    '^' <circumflex>
 *    '~' <tilde>
 *    '!' <exclamation mark>
 *    '#' <number sign>
 *    '|' <vertical line>
 *    '$' <dollar sign>
 *    '@' <commercial at>
 *    '`' <grave accent>
 * \endcode
 * @stable ICU 2.0
 */

#ifndef U_CHARSET_FAMILY
#   define U_CHARSET_FAMILY 0
#endif

/*===========================================================================*/
/* ICUDATA naming scheme                                                     */
/*===========================================================================*/

/**
 * \def U_ICUDATA_TYPE_LETTER
 *
 * This is a platform-dependent string containing one letter:
 * - b for big-endian, ASCII-family platforms
 * - l for little-endian, ASCII-family platforms
 * - e for big-endian, EBCDIC-family platforms
 * This letter is part of the common data file name.
 * @stable ICU 2.0
 */

/**
 * \def U_ICUDATA_TYPE_LITLETTER
 * The non-string form of U_ICUDATA_TYPE_LETTER
 * @stable ICU 2.0
 */
#if U_CHARSET_FAMILY
#   if U_IS_BIG_ENDIAN
   /* EBCDIC - should always be BE */
#     define U_ICUDATA_TYPE_LETTER "e"
#     define U_ICUDATA_TYPE_LITLETTER e
#   else
#     error "Don't know what to do with little endian EBCDIC!"
#     define U_ICUDATA_TYPE_LETTER "x"
#     define U_ICUDATA_TYPE_LITLETTER x
#   endif
#else
#   if U_IS_BIG_ENDIAN
      /* Big-endian ASCII */
#     define U_ICUDATA_TYPE_LETTER "b"
#     define U_ICUDATA_TYPE_LITLETTER b
#   else
      /* Little-endian ASCII */
#     define U_ICUDATA_TYPE_LETTER "l"
#     define U_ICUDATA_TYPE_LITLETTER l
#   endif
#endif

/**
 * A single string literal containing the icudata stub name. i.e. 'icudt18e' for
 * ICU 1.8.x on EBCDIC, etc..
 * @stable ICU 2.0
 */
#define U_ICUDATA_NAME    "icudt" U_ICU_VERSION_SHORT U_ICUDATA_TYPE_LETTER


/**
 *  U_ICU_ENTRY_POINT is the name of the DLL entry point to the ICU data library.
 *    Defined as a literal, not a string.
 *    Tricky Preprocessor use - ## operator replaces macro paramters with the literal string
 *                              from the corresponding macro invocation, _before_ other macro substitutions.
 *                              Need a nested \#defines to get the actual version numbers rather than
 *                              the literal text U_ICU_VERSION_MAJOR_NUM into the name.
 *                              The net result will be something of the form
 *                                  \#define U_ICU_ENTRY_POINT icudt19_dat
 * @stable ICU 2.4
 */
#define U_ICUDATA_ENTRY_POINT  U_DEF2_ICUDATA_ENTRY_POINT(U_ICU_VERSION_MAJOR_NUM, U_ICU_VERSION_MINOR_NUM)

/**
 * Do not use.
 * @internal
 */
#define U_DEF2_ICUDATA_ENTRY_POINT(major, minor) U_DEF_ICUDATA_ENTRY_POINT(major, minor)
/**
 * Do not use.
 * @internal
 */
#define U_DEF_ICUDATA_ENTRY_POINT(major, minor) icudt##major##minor##_dat


/**
 * \def NULL
 * Define NULL if necessary, to 0 for C++ and to ((void *)0) for C.
 * @stable ICU 2.0
 */
#ifndef NULL
#ifdef XP_CPLUSPLUS
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

/*===========================================================================*/
/* Calendar/TimeZone data types                                              */
/*===========================================================================*/

/**
 * Date and Time data type.
 * This is a primitive data type that holds the date and time
 * as the number of milliseconds since 1970-jan-01, 00:00 UTC.
 * UTC leap seconds are ignored.
 * @stable ICU 2.0
 */
typedef double UDate;

/** The number of milliseconds per second @stable ICU 2.0 */
#define U_MILLIS_PER_SECOND        (1000)
/** The number of milliseconds per minute @stable ICU 2.0 */
#define U_MILLIS_PER_MINUTE       (60000)
/** The number of milliseconds per hour @stable ICU 2.0 */
#define U_MILLIS_PER_HOUR       (3600000)
/** The number of milliseconds per day @stable ICU 2.0 */
#define U_MILLIS_PER_DAY       (86400000)


/*===========================================================================*/
/* UClassID-based RTTI */
/*===========================================================================*/

/**
 * UClassID is used to identify classes without using RTTI, since RTTI
 * is not yet supported by all C++ compilers.  Each class hierarchy which needs
 * to implement polymorphic clone() or operator==() defines two methods,
 * described in detail below.  UClassID values can be compared using
 * operator==(). Nothing else should be done with them.
 *
 * \par
 * getDynamicClassID() is declared in the base class of the hierarchy as
 * a pure virtual.  Each concrete subclass implements it in the same way:
 *
 * \code
 *      class Base {
 *      public:
 *          virtual UClassID getDynamicClassID() const = 0;
 *      }
 *
 *      class Derived {
 *      public:
 *          virtual UClassID getDynamicClassID() const
 *            { return Derived::getStaticClassID(); }
 *      }
 * \endcode
 *
 * Each concrete class implements getStaticClassID() as well, which allows
 * clients to test for a specific type.
 *
 * \code
 *      class Derived {
 *      public:
 *          static UClassID U_EXPORT2 getStaticClassID();
 *      private:
 *          static char fgClassID;
 *      }
 *
 *      // In Derived.cpp:
 *      UClassID Derived::getStaticClassID()
 *        { return (UClassID)&Derived::fgClassID; }
 *      char Derived::fgClassID = 0; // Value is irrelevant
 * \endcode
 * @stable ICU 2.0
 */
typedef void* UClassID;

/*===========================================================================*/
/* Shared library/DLL import-export API control                              */
/*===========================================================================*/

/*
 * Control of symbol import/export.
 * ICU is separated into three libraries.
 */

/*
 * \def U_COMBINED_IMPLEMENTATION
 * Set to export library symbols from inside the ICU library
 * when all of ICU is in a single library.
 * This can be set as a compiler option while building ICU, and it
 * needs to be the first one tested to override U_COMMON_API, U_I18N_API, etc.
 * @stable ICU 2.0
 */

/**
 * \def U_DATA_API
 * Set to export library symbols from inside the stubdata library,
 * and to import them from outside.
 * @stable ICU 3.0
 */

/**
 * \def U_COMMON_API
 * Set to export library symbols from inside the common library,
 * and to import them from outside.
 * @stable ICU 2.0
 */

/**
 * \def U_I18N_API
 * Set to export library symbols from inside the i18n library,
 * and to import them from outside.
 * @stable ICU 2.0
 */

/**
 * \def U_LAYOUT_API
 * Set to export library symbols from inside the layout engine library,
 * and to import them from outside.
 * @stable ICU 2.0
 */

/**
 * \def U_LAYOUTEX_API
 * Set to export library symbols from inside the layout extensions library,
 * and to import them from outside.
 * @stable ICU 2.6
 */

/**
 * \def U_IO_API
 * Set to export library symbols from inside the ustdio library,
 * and to import them from outside.
 * @stable ICU 2.0
 */

/**
 * \def U_TOOLUTIL_API
 * Set to export library symbols from inside the toolutil library,
 * and to import them from outside.
 * @stable ICU 3.4
 */

#if defined(U_COMBINED_IMPLEMENTATION)
#define U_DATA_API     U_EXPORT
#define U_COMMON_API   U_EXPORT
#define U_I18N_API     U_EXPORT
#define U_LAYOUT_API   U_EXPORT
#define U_LAYOUTEX_API U_EXPORT
#define U_IO_API       U_EXPORT
#define U_TOOLUTIL_API U_EXPORT
#elif defined(U_STATIC_IMPLEMENTATION)
#define U_DATA_API
#define U_COMMON_API
#define U_I18N_API
#define U_LAYOUT_API
#define U_LAYOUTEX_API
#define U_IO_API
#define U_TOOLUTIL_API
#elif defined(U_COMMON_IMPLEMENTATION)
#define U_DATA_API     U_IMPORT
#define U_COMMON_API   U_EXPORT
#define U_I18N_API     U_IMPORT
#define U_LAYOUT_API   U_IMPORT
#define U_LAYOUTEX_API U_IMPORT
#define U_IO_API       U_IMPORT
#define U_TOOLUTIL_API U_IMPORT
#elif defined(U_I18N_IMPLEMENTATION)
#define U_DATA_API     U_IMPORT
#define U_COMMON_API   U_IMPORT
#define U_I18N_API     U_EXPORT
#define U_LAYOUT_API   U_IMPORT
#define U_LAYOUTEX_API U_IMPORT
#define U_IO_API       U_IMPORT
#define U_TOOLUTIL_API U_IMPORT
#elif defined(U_LAYOUT_IMPLEMENTATION)
#define U_DATA_API     U_IMPORT
#define U_COMMON_API   U_IMPORT
#define U_I18N_API     U_IMPORT
#define U_LAYOUT_API   U_EXPORT
#define U_LAYOUTEX_API U_IMPORT
#define U_IO_API       U_IMPORT
#define U_TOOLUTIL_API U_IMPORT
#elif defined(U_LAYOUTEX_IMPLEMENTATION)
#define U_DATA_API     U_IMPORT
#define U_COMMON_API   U_IMPORT
#define U_I18N_API     U_IMPORT
#define U_LAYOUT_API   U_IMPORT
#define U_LAYOUTEX_API U_EXPORT
#define U_IO_API       U_IMPORT
#define U_TOOLUTIL_API U_IMPORT
#elif defined(U_IO_IMPLEMENTATION)
#define U_DATA_API     U_IMPORT
#define U_COMMON_API   U_IMPORT
#define U_I18N_API     U_IMPORT
#define U_LAYOUT_API   U_IMPORT
#define U_LAYOUTEX_API U_IMPORT
#define U_IO_API       U_EXPORT
#define U_TOOLUTIL_API U_IMPORT
#elif defined(U_TOOLUTIL_IMPLEMENTATION)
#define U_DATA_API     U_IMPORT
#define U_COMMON_API   U_IMPORT
#define U_I18N_API     U_IMPORT
#define U_LAYOUT_API   U_IMPORT
#define U_LAYOUTEX_API U_IMPORT
#define U_IO_API       U_IMPORT
#define U_TOOLUTIL_API U_EXPORT
#else
#define U_DATA_API     U_IMPORT
#define U_COMMON_API   U_IMPORT
#define U_I18N_API     U_IMPORT
#define U_LAYOUT_API   U_IMPORT
#define U_LAYOUTEX_API U_IMPORT
#define U_IO_API       U_IMPORT
#define U_TOOLUTIL_API U_IMPORT
#endif

/**
 * \def U_STANDARD_CPP_NAMESPACE
 * Control of C++ Namespace
 * @stable ICU 2.0
 */
#ifdef __cplusplus
#define U_STANDARD_CPP_NAMESPACE        ::
#else
#define U_STANDARD_CPP_NAMESPACE
#endif


/*===========================================================================*/
/* Global delete operator                                                    */
/*===========================================================================*/

/*
 * The ICU4C library must not use the global new and delete operators.
 * These operators here are defined to enable testing for this.
 * See Jitterbug 2581 for details of why this is necessary.
 *
 * Verification that ICU4C's memory usage is correct, i.e.,
 * that global new/delete are not used:
 *
 * a) Check for imports of global new/delete (see uobject.cpp for details)
 * b) Verify that new is never imported.
 * c) Verify that delete is only imported from object code for interface/mixin classes.
 * d) Add global delete and delete[] only for the ICU4C library itself
 *    and define them in a way that crashes or otherwise easily shows a problem.
 *
 * The following implements d).
 * The operator implementations crash; this is intentional and used for library debugging.
 *
 * Note: This is currently only done on Windows because
 * some Linux/Unix compilers have problems with defining global new/delete.
 * On Windows, U_WINDOWS is defined, and it is _MSC_VER>=1200 for MSVC 6.0 and higher.
 */
#if defined(XP_CPLUSPLUS) && defined(U_WINDOWS) && U_DEBUG && U_OVERRIDE_CXX_ALLOCATION && (_MSC_VER>=1200) && !defined(U_STATIC_IMPLEMENTATION) && (defined(U_COMMON_IMPLEMENTATION) || defined(U_I18N_IMPLEMENTATION) || defined(U_IO_IMPLEMENTATION) || defined(U_LAYOUT_IMPLEMENTATION) || defined(U_LAYOUTEX_IMPLEMENTATION))

#ifndef U_HIDE_INTERNAL_API
/**
 * Global operator new, defined only inside ICU4C, must not be used.
 * Crashes intentionally.
 * @internal
 */
inline void *
operator new(size_t /*size*/) {
    char *q=NULL;
    *q=5; /* break it */
    return q;
}

#ifdef _Ret_bytecap_
/* This is only needed to suppress a Visual C++ 2008 warning for operator new[]. */
_Ret_bytecap_(_Size)
#endif
/**
 * Global operator new[], defined only inside ICU4C, must not be used.
 * Crashes intentionally.
 * @internal
 */
inline void *
operator new[](size_t /*size*/) {
    char *q=NULL;
    *q=5; /* break it */
    return q;
}

/**
 * Global operator delete, defined only inside ICU4C, must not be used.
 * Crashes intentionally.
 * @internal
 */
inline void
operator delete(void * /*p*/) {
    char *q=NULL;
    *q=5; /* break it */
}

/**
 * Global operator delete[], defined only inside ICU4C, must not be used.
 * Crashes intentionally.
 * @internal
 */
inline void
operator delete[](void * /*p*/) {
    char *q=NULL;
    *q=5; /* break it */
}

#endif /* U_HIDE_INTERNAL_API */
#endif


/**
 * Return a string for a UErrorCode value.
 * The string will be the same as the name of the error code constant
 * in the UErrorCode enum above.
 * @stable ICU 2.0
 */
U_STABLE const char * U_EXPORT2
u_errorName(UErrorCode code);


#endif /* _UTYPES */
