/*
**********************************************************************
*   Copyright (C) 1997-2008, International Business Machines
*   Corporation and others.  All Rights Reserved.
**********************************************************************
*
* File UCHAR.H
*
* Modification History:
*
*   Date        Name        Description
*   04/02/97    aliu        Creation.
*   03/29/99    helena      Updated for C APIs.
*   4/15/99     Madhu       Updated for C Implementation and Javadoc
*   5/20/99     Madhu       Added the function u_getVersion()
*   8/19/1999   srl         Upgraded scripts to Unicode 3.0
*   8/27/1999   schererm    UCharDirection constants: U_...
*   11/11/1999  weiv        added u_isalnum(), cleaned comments
*   01/11/2000  helena      Renamed u_getVersion to u_getUnicodeVersion().
******************************************************************************
*/

#ifndef UCHAR_H
#define UCHAR_H

#include "unicode/uversion.h"
#include "unicode/utypes.h"

U_CDECL_BEGIN

/*==========================================================================*/
/* Unicode version number                                                   */
/*==========================================================================*/
/**
 * Unicode version number, default for the current ICU version.
 * The actual Unicode Character Database (UCD) data is stored in uprops.dat
 * and may be generated from UCD files from a different Unicode version.
 * Call u_getUnicodeVersion to get the actual Unicode version of the data.
 *
 * @see u_getUnicodeVersion
 * @stable ICU 2.0
 */
#define U_UNICODE_VERSION "5.1"

/**
 * \file
 * \brief C API: Unicode Properties
 *
 * This C API provides low-level access to the Unicode Character Database.
 * In addition to raw property values, some convenience functions calculate
 * derived properties, for example for Java-style programming.
 *
 * Unicode assigns each code point (not just assigned character) values for
 * many properties.
 * Most of them are simple boolean flags, or constants from a small enumerated list.
 * For some properties, values are strings or other relatively more complex types.
 *
 * For more information see
 * "About the Unicode Character Database" (http://www.unicode.org/ucd/)
 * and the ICU User Guide chapter on Properties (http://icu-project.org/userguide/properties.html).
 *
 * Many functions are designed to match java.lang.Character functions.
 * See the individual function documentation,
 * and see the JDK 1.4 java.lang.Character documentation
 * at http://java.sun.com/j2se/1.4/docs/api/java/lang/Character.html
 *
 * There are also functions that provide easy migration from C/POSIX functions
 * like isblank(). Their use is generally discouraged because the C/POSIX
 * standards do not define their semantics beyond the ASCII range, which means
 * that different implementations exhibit very different behavior.
 * Instead, Unicode properties should be used directly.
 *
 * There are also only a few, broad C/POSIX character classes, and they tend
 * to be used for conflicting purposes. For example, the "isalpha()" class
 * is sometimes used to determine word boundaries, while a more sophisticated
 * approach would at least distinguish initial letters from continuation
 * characters (the latter including combining marks).
 * (In ICU, BreakIterator is the most sophisticated API for word boundaries.)
 * Another example: There is no "istitle()" class for titlecase characters.
 *
 * ICU 3.4 and later provides API access for all twelve C/POSIX character classes.
 * ICU implements them according to the Standard Recommendations in
 * Annex C: Compatibility Properties of UTS #18 Unicode Regular Expressions
 * (http://www.unicode.org/reports/tr18/#Compatibility_Properties).
 *
 * API access for C/POSIX character classes is as follows:
 * - alpha:     u_isUAlphabetic(c) or u_hasBinaryProperty(c, UCHAR_ALPHABETIC)
 * - lower:     u_isULowercase(c) or u_hasBinaryProperty(c, UCHAR_LOWERCASE)
 * - upper:     u_isUUppercase(c) or u_hasBinaryProperty(c, UCHAR_UPPERCASE)
 * - punct:     u_ispunct(c)
 * - digit:     u_isdigit(c) or u_charType(c)==U_DECIMAL_DIGIT_NUMBER
 * - xdigit:    u_isxdigit(c) or u_hasBinaryProperty(c, UCHAR_POSIX_XDIGIT)
 * - alnum:     u_hasBinaryProperty(c, UCHAR_POSIX_ALNUM)
 * - space:     u_isUWhiteSpace(c) or u_hasBinaryProperty(c, UCHAR_WHITE_SPACE)
 * - blank:     u_isblank(c) or u_hasBinaryProperty(c, UCHAR_POSIX_BLANK)
 * - cntrl:     u_charType(c)==U_CONTROL_CHAR
 * - graph:     u_hasBinaryProperty(c, UCHAR_POSIX_GRAPH)
 * - print:     u_hasBinaryProperty(c, UCHAR_POSIX_PRINT)
 *
 * Note: Some of the u_isxyz() functions in uchar.h predate, and do not match,
 * the Standard Recommendations in UTS #18. Instead, they match Java
 * functions according to their API documentation.
 *
 * \htmlonly
 * The C/POSIX character classes are also available in UnicodeSet patterns,
 * using patterns like [:graph:] or \p{graph}.
 * \endhtmlonly
 *
 * Note: There are several ICU whitespace functions.
 * Comparison:
 * - u_isUWhiteSpace=UCHAR_WHITE_SPACE: Unicode White_Space property;
 *       most of general categories "Z" (separators) + most whitespace ISO controls
 *       (including no-break spaces, but excluding IS1..IS4 and ZWSP)
 * - u_isWhitespace: Java isWhitespace; Z + whitespace ISO controls but excluding no-break spaces
 * - u_isJavaSpaceChar: Java isSpaceChar; just Z (including no-break spaces)
 * - u_isspace: Z + whitespace ISO controls (including no-break spaces)
 * - u_isblank: "horizontal spaces" = TAB + Zs - ZWSP
 */

/**
 * Constants.
 */

/** The lowest Unicode code point value. Code points are non-negative. @stable ICU 2.0 */
#define UCHAR_MIN_VALUE 0

/**
 * The highest Unicode code point value (scalar value) according to
 * The Unicode Standard. This is a 21-bit value (20.1 bits, rounded up).
 * For a single character, UChar32 is a simple type that can hold any code point value.
 *
 * @see UChar32
 * @stable ICU 2.0
 */
#define UCHAR_MAX_VALUE 0x10ffff

/*
 * !! Note: Several comments in this file are machine-read by the
 * genpname tool.  These comments describe the correspondence between
 * icu enum constants and UCD entities.  Do not delete them.  Update
 * these comments as needed.
 *
 * Any comment of the form "/ *[name]* /" (spaces added) is such
 * a comment.
 *
 * The U_JG_* and U_GC_*_MASK constants are matched by their symbolic
 * name, which must match PropertyValueAliases.txt.
 */

/**
 * U_GC_XX_MASK constants are bit flags corresponding to Unicode
 * general category values.
 * For each category, the nth bit is set if the numeric value of the
 * corresponding UCharCategory constant is n.
 *
 * There are also some U_GC_Y_MASK constants for groups of general categories
 * like L for all letter categories.
 *
 * @see u_charType
 * @see U_GET_GC_MASK
 * @see UCharCategory
 * @stable ICU 2.1
 */
#define U_GC_CN_MASK    U_MASK(U_GENERAL_OTHER_TYPES)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LU_MASK    U_MASK(U_UPPERCASE_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LL_MASK    U_MASK(U_LOWERCASE_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LT_MASK    U_MASK(U_TITLECASE_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LM_MASK    U_MASK(U_MODIFIER_LETTER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_LO_MASK    U_MASK(U_OTHER_LETTER)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_MN_MASK    U_MASK(U_NON_SPACING_MARK)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ME_MASK    U_MASK(U_ENCLOSING_MARK)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_MC_MASK    U_MASK(U_COMBINING_SPACING_MARK)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ND_MASK    U_MASK(U_DECIMAL_DIGIT_NUMBER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_NL_MASK    U_MASK(U_LETTER_NUMBER)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_NO_MASK    U_MASK(U_OTHER_NUMBER)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ZS_MASK    U_MASK(U_SPACE_SEPARATOR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ZL_MASK    U_MASK(U_LINE_SEPARATOR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_ZP_MASK    U_MASK(U_PARAGRAPH_SEPARATOR)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CC_MASK    U_MASK(U_CONTROL_CHAR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CF_MASK    U_MASK(U_FORMAT_CHAR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CO_MASK    U_MASK(U_PRIVATE_USE_CHAR)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_CS_MASK    U_MASK(U_SURROGATE)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PD_MASK    U_MASK(U_DASH_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PS_MASK    U_MASK(U_START_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PE_MASK    U_MASK(U_END_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PC_MASK    U_MASK(U_CONNECTOR_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PO_MASK    U_MASK(U_OTHER_PUNCTUATION)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SM_MASK    U_MASK(U_MATH_SYMBOL)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SC_MASK    U_MASK(U_CURRENCY_SYMBOL)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SK_MASK    U_MASK(U_MODIFIER_SYMBOL)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_SO_MASK    U_MASK(U_OTHER_SYMBOL)

/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PI_MASK    U_MASK(U_INITIAL_PUNCTUATION)
/** Mask constant for a UCharCategory. @stable ICU 2.1 */
#define U_GC_PF_MASK    U_MASK(U_FINAL_PUNCTUATION)


/** Mask constant for multiple UCharCategory bits (L Letters). @stable ICU 2.1 */
#define U_GC_L_MASK \
            (U_GC_LU_MASK|U_GC_LL_MASK|U_GC_LT_MASK|U_GC_LM_MASK|U_GC_LO_MASK)

/** Mask constant for multiple UCharCategory bits (LC Cased Letters). @stable ICU 2.1 */
#define U_GC_LC_MASK \
            (U_GC_LU_MASK|U_GC_LL_MASK|U_GC_LT_MASK)

/** Mask constant for multiple UCharCategory bits (M Marks). @stable ICU 2.1 */
#define U_GC_M_MASK (U_GC_MN_MASK|U_GC_ME_MASK|U_GC_MC_MASK)

/** Mask constant for multiple UCharCategory bits (N Numbers). @stable ICU 2.1 */
#define U_GC_N_MASK (U_GC_ND_MASK|U_GC_NL_MASK|U_GC_NO_MASK)

/** Mask constant for multiple UCharCategory bits (Z Separators). @stable ICU 2.1 */
#define U_GC_Z_MASK (U_GC_ZS_MASK|U_GC_ZL_MASK|U_GC_ZP_MASK)

/** Mask constant for multiple UCharCategory bits (C Others). @stable ICU 2.1 */
#define U_GC_C_MASK \
            (U_GC_CN_MASK|U_GC_CC_MASK|U_GC_CF_MASK|U_GC_CO_MASK|U_GC_CS_MASK)

/** Mask constant for multiple UCharCategory bits (P Punctuation). @stable ICU 2.1 */
#define U_GC_P_MASK \
            (U_GC_PD_MASK|U_GC_PS_MASK|U_GC_PE_MASK|U_GC_PC_MASK|U_GC_PO_MASK| \
             U_GC_PI_MASK|U_GC_PF_MASK)

/** Mask constant for multiple UCharCategory bits (S Symbols). @stable ICU 2.1 */
#define U_GC_S_MASK (U_GC_SM_MASK|U_GC_SC_MASK|U_GC_SK_MASK|U_GC_SO_MASK)

/** @stable ICU 2.0 */
typedef enum UBlockCode UBlockCode;

/**
 * East Asian Width constants.
 *
 * @see UCHAR_EAST_ASIAN_WIDTH
 * @see u_getIntPropertyValue
 * @stable ICU 2.2
 */
typedef enum UEastAsianWidth {
    U_EA_NEUTRAL,   /*[N]*/ /*See note !!*/
    U_EA_AMBIGUOUS, /*[A]*/
    U_EA_HALFWIDTH, /*[H]*/
    U_EA_FULLWIDTH, /*[F]*/
    U_EA_NARROW,    /*[Na]*/
    U_EA_WIDE,      /*[W]*/
    U_EA_COUNT
} UEastAsianWidth;
/*
 * Implementation note:
 * Keep UEastAsianWidth constant values in sync with names list in genprops/props2.c.
 */

/**
 * Selector constants for u_charName().
 * u_charName() returns the "modern" name of a
 * Unicode character; or the name that was defined in
 * Unicode version 1.0, before the Unicode standard merged
 * with ISO-10646; or an "extended" name that gives each
 * Unicode code point a unique name.
 *
 * @see u_charName
 * @stable ICU 2.0
 */
typedef enum UCharNameChoice {
    U_UNICODE_CHAR_NAME,
    U_UNICODE_10_CHAR_NAME,
    U_EXTENDED_CHAR_NAME,
    U_CHAR_NAME_CHOICE_COUNT
} UCharNameChoice;

/**
 * Selector constants for u_getPropertyName() and
 * u_getPropertyValueName().  These selectors are used to choose which
 * name is returned for a given property or value.  All properties and
 * values have a long name.  Most have a short name, but some do not.
 * Unicode allows for additional names, beyond the long and short
 * name, which would be indicated by U_LONG_PROPERTY_NAME + i, where
 * i=1, 2,...
 *
 * @see u_getPropertyName()
 * @see u_getPropertyValueName()
 * @stable ICU 2.4
 */
typedef enum UPropertyNameChoice {
    U_SHORT_PROPERTY_NAME,
    U_LONG_PROPERTY_NAME,
    U_PROPERTY_NAME_CHOICE_COUNT
} UPropertyNameChoice;


/**
 * Joining Type constants.
 *
 * @see UCHAR_JOINING_TYPE
 * @stable ICU 2.2
 */
typedef enum UJoiningType {
    U_JT_NON_JOINING,       /*[U]*/ /*See note !!*/
    U_JT_JOIN_CAUSING,      /*[C]*/
    U_JT_DUAL_JOINING,      /*[D]*/
    U_JT_LEFT_JOINING,      /*[L]*/
    U_JT_RIGHT_JOINING,     /*[R]*/
    U_JT_TRANSPARENT,       /*[T]*/
    U_JT_COUNT /* 6 */
} UJoiningType;

/**
 * Joining Group constants.
 *
 * @see UCHAR_JOINING_GROUP
 * @stable ICU 2.2
 */
typedef enum UJoiningGroup {
    U_JG_NO_JOINING_GROUP,
    U_JG_AIN,
    U_JG_ALAPH,
    U_JG_ALEF,
    U_JG_BEH,
    U_JG_BETH,
    U_JG_DAL,
    U_JG_DALATH_RISH,
    U_JG_E,
    U_JG_FEH,
    U_JG_FINAL_SEMKATH,
    U_JG_GAF,
    U_JG_GAMAL,
    U_JG_HAH,
    U_JG_HAMZA_ON_HEH_GOAL,
    U_JG_HE,
    U_JG_HEH,
    U_JG_HEH_GOAL,
    U_JG_HETH,
    U_JG_KAF,
    U_JG_KAPH,
    U_JG_KNOTTED_HEH,
    U_JG_LAM,
    U_JG_LAMADH,
    U_JG_MEEM,
    U_JG_MIM,
    U_JG_NOON,
    U_JG_NUN,
    U_JG_PE,
    U_JG_QAF,
    U_JG_QAPH,
    U_JG_REH,
    U_JG_REVERSED_PE,
    U_JG_SAD,
    U_JG_SADHE,
    U_JG_SEEN,
    U_JG_SEMKATH,
    U_JG_SHIN,
    U_JG_SWASH_KAF,
    U_JG_SYRIAC_WAW,
    U_JG_TAH,
    U_JG_TAW,
    U_JG_TEH_MARBUTA,
    U_JG_TETH,
    U_JG_WAW,
    U_JG_YEH,
    U_JG_YEH_BARREE,
    U_JG_YEH_WITH_TAIL,
    U_JG_YUDH,
    U_JG_YUDH_HE,
    U_JG_ZAIN,
    U_JG_FE,        /**< @stable ICU 2.6 */
    U_JG_KHAPH,     /**< @stable ICU 2.6 */
    U_JG_ZHAIN,     /**< @stable ICU 2.6 */
    U_JG_BURUSHASKI_YEH_BARREE, /**< @draft ICU 4.0 */
    U_JG_COUNT
} UJoiningGroup;

/**
 * Grapheme Cluster Break constants.
 *
 * @see UCHAR_GRAPHEME_CLUSTER_BREAK
 * @stable ICU 3.4
 */
typedef enum UGraphemeClusterBreak {
    U_GCB_OTHER = 0,            /*[XX]*/ /*See note !!*/
    U_GCB_CONTROL = 1,          /*[CN]*/
    U_GCB_CR = 2,               /*[CR]*/
    U_GCB_EXTEND = 3,           /*[EX]*/
    U_GCB_L = 4,                /*[L]*/
    U_GCB_LF = 5,               /*[LF]*/
    U_GCB_LV = 6,               /*[LV]*/
    U_GCB_LVT = 7,              /*[LVT]*/
    U_GCB_T = 8,                /*[T]*/
    U_GCB_V = 9,                /*[V]*/
    U_GCB_SPACING_MARK = 10,    /*[SM]*/ /* from here on: new in Unicode 5.1/ICU 4.0 */
    U_GCB_PREPEND = 11,         /*[PP]*/
    U_GCB_COUNT = 12
} UGraphemeClusterBreak;

/**
 * Word Break constants.
 * (UWordBreak is a pre-existing enum type in ubrk.h for word break status tags.)
 *
 * @see UCHAR_WORD_BREAK
 * @stable ICU 3.4
 */
typedef enum UWordBreakValues {
    U_WB_OTHER = 0,             /*[XX]*/ /*See note !!*/
    U_WB_ALETTER = 1,           /*[LE]*/
    U_WB_FORMAT = 2,            /*[FO]*/
    U_WB_KATAKANA = 3,          /*[KA]*/
    U_WB_MIDLETTER = 4,         /*[ML]*/
    U_WB_MIDNUM = 5,            /*[MN]*/
    U_WB_NUMERIC = 6,           /*[NU]*/
    U_WB_EXTENDNUMLET = 7,      /*[EX]*/
    U_WB_CR = 8,                /*[CR]*/ /* from here on: new in Unicode 5.1/ICU 4.0 */
    U_WB_EXTEND = 9,            /*[Extend]*/
    U_WB_LF = 10,               /*[LF]*/
    U_WB_MIDNUMLET =11,         /*[MB]*/
    U_WB_NEWLINE =12,           /*[NL]*/
    U_WB_COUNT = 13
} UWordBreakValues;

/**
 * Sentence Break constants.
 *
 * @see UCHAR_SENTENCE_BREAK
 * @stable ICU 3.4
 */
typedef enum USentenceBreak {
    U_SB_OTHER = 0,             /*[XX]*/ /*See note !!*/
    U_SB_ATERM = 1,             /*[AT]*/
    U_SB_CLOSE = 2,             /*[CL]*/
    U_SB_FORMAT = 3,            /*[FO]*/
    U_SB_LOWER = 4,             /*[LO]*/
    U_SB_NUMERIC = 5,           /*[NU]*/
    U_SB_OLETTER = 6,           /*[LE]*/
    U_SB_SEP = 7,               /*[SE]*/
    U_SB_SP = 8,                /*[SP]*/
    U_SB_STERM = 9,             /*[ST]*/
    U_SB_UPPER = 10,            /*[UP]*/
    U_SB_CR = 11,               /*[CR]*/ /* from here on: new in Unicode 5.1/ICU 4.0 */
    U_SB_EXTEND = 12,           /*[EX]*/
    U_SB_LF = 13,               /*[LF]*/
    U_SB_SCONTINUE = 14,        /*[SC]*/
    U_SB_COUNT = 15
} USentenceBreak;

/**
 * Numeric Type constants.
 *
 * @see UCHAR_NUMERIC_TYPE
 * @stable ICU 2.2
 */
typedef enum UNumericType {
    U_NT_NONE,              /*[None]*/ /*See note !!*/
    U_NT_DECIMAL,           /*[de]*/
    U_NT_DIGIT,             /*[di]*/
    U_NT_NUMERIC,           /*[nu]*/
    U_NT_COUNT
} UNumericType;

/**
 * Hangul Syllable Type constants.
 *
 * @see UCHAR_HANGUL_SYLLABLE_TYPE
 * @stable ICU 2.6
 */
typedef enum UHangulSyllableType {
    U_HST_NOT_APPLICABLE,   /*[NA]*/ /*See note !!*/
    U_HST_LEADING_JAMO,     /*[L]*/
    U_HST_VOWEL_JAMO,       /*[V]*/
    U_HST_TRAILING_JAMO,    /*[T]*/
    U_HST_LV_SYLLABLE,      /*[LV]*/
    U_HST_LVT_SYLLABLE,     /*[LVT]*/
    U_HST_COUNT
} UHangulSyllableType;

/**
 * Check a binary Unicode property for a code point.
 *
 * Unicode, especially in version 3.2, defines many more properties than the
 * original set in UnicodeData.txt.
 *
 * The properties APIs are intended to reflect Unicode properties as defined
 * in the Unicode Character Database (UCD) and Unicode Technical Reports (UTR).
 * For details about the properties see http://www.unicode.org/ucd/ .
 * For names of Unicode properties see the UCD file PropertyAliases.txt.
 *
 * Important: If ICU is built with UCD files from Unicode versions below 3.2,
 * then properties marked with "new in Unicode 3.2" are not or not fully available.
 *
 * @param c Code point to test.
 * @param which UProperty selector constant, identifies which binary property to check.
 *        Must be UCHAR_BINARY_START<=which<UCHAR_BINARY_LIMIT.
 * @return TRUE or FALSE according to the binary Unicode property value for c.
 *         Also FALSE if 'which' is out of bounds or if the Unicode version
 *         does not have data for the property at all, or not for this code point.
 *
 * @see UProperty
 * @see u_getIntPropertyValue
 * @see u_getUnicodeVersion
 * @stable ICU 2.1
 */
U_STABLE UBool U_EXPORT2
u_hasBinaryProperty(UChar32 c, UProperty which);

/**
 * Check if a code point has the Alphabetic Unicode property.
 * Same as u_hasBinaryProperty(c, UCHAR_ALPHABETIC).
 * This is different from u_isalpha!
 * @param c Code point to test
 * @return true if the code point has the Alphabetic Unicode property, false otherwise
 *
 * @see UCHAR_ALPHABETIC
 * @see u_isalpha
 * @see u_hasBinaryProperty
 * @stable ICU 2.1
 */
U_STABLE UBool U_EXPORT2
u_isUAlphabetic(UChar32 c);

/**
 * Check if a code point has the Lowercase Unicode property.
 * Same as u_hasBinaryProperty(c, UCHAR_LOWERCASE).
 * This is different from u_islower!
 * @param c Code point to test
 * @return true if the code point has the Lowercase Unicode property, false otherwise
 *
 * @see UCHAR_LOWERCASE
 * @see u_islower
 * @see u_hasBinaryProperty
 * @stable ICU 2.1
 */
U_STABLE UBool U_EXPORT2
u_isULowercase(UChar32 c);

/**
 * Check if a code point has the Uppercase Unicode property.
 * Same as u_hasBinaryProperty(c, UCHAR_UPPERCASE).
 * This is different from u_isupper!
 * @param c Code point to test
 * @return true if the code point has the Uppercase Unicode property, false otherwise
 *
 * @see UCHAR_UPPERCASE
 * @see u_isupper
 * @see u_hasBinaryProperty
 * @stable ICU 2.1
 */
U_STABLE UBool U_EXPORT2
u_isUUppercase(UChar32 c);

/**
 * Check if a code point has the White_Space Unicode property.
 * Same as u_hasBinaryProperty(c, UCHAR_WHITE_SPACE).
 * This is different from both u_isspace and u_isWhitespace!
 *
 * Note: There are several ICU whitespace functions; please see the uchar.h
 * file documentation for a detailed comparison.
 *
 * @param c Code point to test
 * @return true if the code point has the White_Space Unicode property, false otherwise.
 *
 * @see UCHAR_WHITE_SPACE
 * @see u_isWhitespace
 * @see u_isspace
 * @see u_isJavaSpaceChar
 * @see u_hasBinaryProperty
 * @stable ICU 2.1
 */
U_STABLE UBool U_EXPORT2
u_isUWhiteSpace(UChar32 c);
/**
 * Get the minimum value for an enumerated/integer/binary Unicode property.
 * Can be used together with u_getIntPropertyMaxValue
 * to allocate arrays of UnicodeSet or similar.
 *
 * @param which UProperty selector constant, identifies which binary property to check.
 *        Must be UCHAR_BINARY_START<=which<UCHAR_BINARY_LIMIT
 *        or UCHAR_INT_START<=which<UCHAR_INT_LIMIT.
 * @return Minimum value returned by u_getIntPropertyValue for a Unicode property.
 *         0 if the property selector is out of range.
 *
 * @see UProperty
 * @see u_hasBinaryProperty
 * @see u_getUnicodeVersion
 * @see u_getIntPropertyMaxValue
 * @see u_getIntPropertyValue
 * @stable ICU 2.2
 */
U_STABLE int32_t U_EXPORT2
u_getIntPropertyMinValue(UProperty which);

/**
 * Get the maximum value for an enumerated/integer/binary Unicode property.
 * Can be used together with u_getIntPropertyMinValue
 * to allocate arrays of UnicodeSet or similar.
 *
 * Examples for min/max values (for Unicode 3.2):
 *
 * - UCHAR_BIDI_CLASS:    0/18 (U_LEFT_TO_RIGHT/U_BOUNDARY_NEUTRAL)
 * - UCHAR_SCRIPT:        0/45 (USCRIPT_COMMON/USCRIPT_TAGBANWA)
 * - UCHAR_IDEOGRAPHIC:   0/1  (FALSE/TRUE)
 *
 * For undefined UProperty constant values, min/max values will be 0/-1.
 *
 * @param which UProperty selector constant, identifies which binary property to check.
 *        Must be UCHAR_BINARY_START<=which<UCHAR_BINARY_LIMIT
 *        or UCHAR_INT_START<=which<UCHAR_INT_LIMIT.
 * @return Maximum value returned by u_getIntPropertyValue for a Unicode property.
 *         <=0 if the property selector is out of range.
 *
 * @see UProperty
 * @see u_hasBinaryProperty
 * @see u_getUnicodeVersion
 * @see u_getIntPropertyMaxValue
 * @see u_getIntPropertyValue
 * @stable ICU 2.2
 */
U_STABLE int32_t U_EXPORT2
u_getIntPropertyMaxValue(UProperty which);

/**
 * Get the numeric value for a Unicode code point as defined in the
 * Unicode Character Database.
 *
 * A "double" return type is necessary because
 * some numeric values are fractions, negative, or too large for int32_t.
 *
 * For characters without any numeric values in the Unicode Character Database,
 * this function will return U_NO_NUMERIC_VALUE.
 *
 * Similar to java.lang.Character.getNumericValue(), but u_getNumericValue()
 * also supports negative values, large values, and fractions,
 * while Java's getNumericValue() returns values 10..35 for ASCII letters.
 *
 * @param c Code point to get the numeric value for.
 * @return Numeric value of c, or U_NO_NUMERIC_VALUE if none is defined.
 *
 * @see U_NO_NUMERIC_VALUE
 * @stable ICU 2.2
 */
U_STABLE double U_EXPORT2
u_getNumericValue(UChar32 c);

/**
 * Special value that is returned by u_getNumericValue when
 * no numeric value is defined for a code point.
 *
 * @see u_getNumericValue
 * @stable ICU 2.2
 */
#define U_NO_NUMERIC_VALUE ((double)-123456789.)


/**
 * Determines whether the specified code point is a titlecase letter.
 * True for general category "Lt" (titlecase letter).
 *
 * Same as java.lang.Character.isTitleCase().
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is an Lt titlecase letter
 *
 * @see u_isupper
 * @see u_islower
 * @see u_totitle
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_istitle(UChar32 c);

/**
 * Determines whether the specified code point is a digit character according to Java.
 * True for characters with general category "Nd" (decimal digit numbers).
 * Beginning with Unicode 4, this is the same as
 * testing for the Numeric_Type of Decimal.
 *
 * Same as java.lang.Character.isDigit().
 *
 * In addition to being equivalent to a Java function, this also serves
 * as a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a digit character according to Character.isDigit()
 *
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isdigit(UChar32 c);

/**
 * Determines whether the specified code point is a letter character.
 * True for general categories "L" (letters).
 *
 * Same as java.lang.Character.isLetter().
 *
 * In addition to being equivalent to a Java function, this also serves
 * as a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a letter character
 *
 * @see u_isdigit
 * @see u_isalnum
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isalpha(UChar32 c);


/**
 * Determines whether the specified code point is a hexadecimal digit.
 * This is equivalent to u_digit(c, 16)>=0.
 * True for characters with general category "Nd" (decimal digit numbers)
 * as well as Latin letters a-f and A-F in both ASCII and Fullwidth ASCII.
 * (That is, for letters with code points
 * 0041..0046, 0061..0066, FF21..FF26, FF41..FF46.)
 *
 * In order to narrow the definition of hexadecimal digits to only ASCII
 * characters, use (c<=0x7f && u_isxdigit(c)).
 *
 * This is a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a hexadecimal digit
 *
 * @stable ICU 2.6
 */
U_STABLE UBool U_EXPORT2
u_isxdigit(UChar32 c);


/**
 * Determines whether the specified code point is a "graphic" character
 * (printable, excluding spaces).
 * TRUE for all characters except those with general categories
 * "Cc" (control codes), "Cf" (format controls), "Cs" (surrogates),
 * "Cn" (unassigned), and "Z" (separators).
 *
 * This is a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a "graphic" character
 *
 * @stable ICU 2.6
 */
U_STABLE UBool U_EXPORT2
u_isgraph(UChar32 c);

/**
 * Determines whether the specified code point is a "blank" or "horizontal space",
 * a character that visibly separates words on a line.
 * The following are equivalent definitions:
 *
 * TRUE for Unicode White_Space characters except for "vertical space controls"
 * where "vertical space controls" are the following characters:
 * U+000A (LF) U+000B (VT) U+000C (FF) U+000D (CR) U+0085 (NEL) U+2028 (LS) U+2029 (PS)
 *
 * same as
 *
 * TRUE for U+0009 (TAB) and characters with general category "Zs" (space separators)
 * except Zero Width Space (ZWSP, U+200B).
 *
 * Note: There are several ICU whitespace functions; please see the uchar.h
 * file documentation for a detailed comparison.
 *
 * This is a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a "blank"
 *
 * @stable ICU 2.6
 */
U_STABLE UBool U_EXPORT2
u_isblank(UChar32 c);

/**
 * Determines whether the specified code point is "defined",
 * which usually means that it is assigned a character.
 * True for general categories other than "Cn" (other, not assigned),
 * i.e., true for all code points mentioned in UnicodeData.txt.
 *
 * Note that non-character code points (e.g., U+FDD0) are not "defined"
 * (they are Cn), but surrogate code points are "defined" (Cs).
 *
 * Same as java.lang.Character.isDefined().
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is assigned a character
 *
 * @see u_isdigit
 * @see u_isalpha
 * @see u_isalnum
 * @see u_isupper
 * @see u_islower
 * @see u_istitle
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isdefined(UChar32 c);

/**
 * Determines if the specified character is a space character or not.
 *
 * Note: There are several ICU whitespace functions; please see the uchar.h
 * file documentation for a detailed comparison.
 *
 * This is a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c    the character to be tested
 * @return  true if the character is a space character; false otherwise.
 *
 * @see u_isJavaSpaceChar
 * @see u_isWhitespace
 * @see u_isUWhiteSpace
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isspace(UChar32 c);

/**
 * Determine if the specified code point is a space character according to Java.
 * True for characters with general categories "Z" (separators),
 * which does not include control codes (e.g., TAB or Line Feed).
 *
 * Same as java.lang.Character.isSpaceChar().
 *
 * Note: There are several ICU whitespace functions; please see the uchar.h
 * file documentation for a detailed comparison.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a space character according to Character.isSpaceChar()
 *
 * @see u_isspace
 * @see u_isWhitespace
 * @see u_isUWhiteSpace
 * @stable ICU 2.6
 */
U_STABLE UBool U_EXPORT2
u_isJavaSpaceChar(UChar32 c);

/**
 * Determines if the specified code point is a whitespace character according to Java/ICU.
 * A character is considered to be a Java whitespace character if and only
 * if it satisfies one of the following criteria:
 *
 * - It is a Unicode separator (categories "Z"), but is not
 *      a no-break space (U+00A0 NBSP or U+2007 Figure Space or U+202F Narrow NBSP).
 * - It is U+0009 HORIZONTAL TABULATION.
 * - It is U+000A LINE FEED.
 * - It is U+000B VERTICAL TABULATION.
 * - It is U+000C FORM FEED.
 * - It is U+000D CARRIAGE RETURN.
 * - It is U+001C FILE SEPARATOR.
 * - It is U+001D GROUP SEPARATOR.
 * - It is U+001E RECORD SEPARATOR.
 * - It is U+001F UNIT SEPARATOR.
 * - It is U+0085 NEXT LINE.
 *
 * Same as java.lang.Character.isWhitespace() except that Java omits U+0085.
 *
 * Note: There are several ICU whitespace functions; please see the uchar.h
 * file documentation for a detailed comparison.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a whitespace character according to Java/ICU
 *
 * @see u_isspace
 * @see u_isJavaSpaceChar
 * @see u_isUWhiteSpace
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isWhitespace(UChar32 c);

/**
 * Determines whether the specified code point is a control character
 * (as defined by this function).
 * A control character is one of the following:
 * - ISO 8-bit control character (U+0000..U+001f and U+007f..U+009f)
 * - U_CONTROL_CHAR (Cc)
 * - U_FORMAT_CHAR (Cf)
 * - U_LINE_SEPARATOR (Zl)
 * - U_PARAGRAPH_SEPARATOR (Zp)
 *
 * This is a C/POSIX migration function.
 * See the comments about C/POSIX character classification functions in the
 * documentation at the top of this header file.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a control character
 *
 * @see UCHAR_DEFAULT_IGNORABLE_CODE_POINT
 * @see u_isprint
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_iscntrl(UChar32 c);

/**
 * Determines whether the specified code point is an ISO control code.
 * True for U+0000..U+001f and U+007f..U+009f (general category "Cc").
 *
 * Same as java.lang.Character.isISOControl().
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is an ISO control code
 *
 * @see u_iscntrl
 * @stable ICU 2.6
 */
U_STABLE UBool U_EXPORT2
u_isISOControl(UChar32 c);


/**
 * Determines whether the specified code point is a base character.
 * True for general categories "L" (letters), "N" (numbers),
 * "Mc" (spacing combining marks), and "Me" (enclosing marks).
 *
 * Note that this is different from the Unicode definition in
 * chapter 3.5, conformance clause D13,
 * which defines base characters to be all characters (not Cn)
 * that do not graphically combine with preceding characters (M)
 * and that are neither control (Cc) or format (Cf) characters.
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is a base character according to this function
 *
 * @see u_isalpha
 * @see u_isdigit
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isbase(UChar32 c);

/**
 * Determines whether the code point has the Bidi_Mirrored property.
 * This property is set for characters that are commonly used in
 * Right-To-Left contexts and need to be displayed with a "mirrored"
 * glyph.
 *
 * Same as java.lang.Character.isMirrored().
 * Same as UCHAR_BIDI_MIRRORED
 *
 * @param c the code point to be tested
 * @return TRUE if the character has the Bidi_Mirrored property
 *
 * @see UCHAR_BIDI_MIRRORED
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isMirrored(UChar32 c);



/**
 * Callback from u_enumCharTypes(), is called for each contiguous range
 * of code points c (where start<=c<limit)
 * with the same Unicode general category ("character type").
 *
 * The callback function can stop the enumeration by returning FALSE.
 *
 * @param context an opaque pointer, as passed into utrie_enum()
 * @param start the first code point in a contiguous range with value
 * @param limit one past the last code point in a contiguous range with value
 * @param type the general category for all code points in [start..limit[
 * @return FALSE to stop the enumeration
 *
 * @stable ICU 2.1
 * @see UCharCategory
 * @see u_enumCharTypes
 */
typedef UBool U_CALLCONV
UCharEnumTypeRange(const void *context, UChar32 start, UChar32 limit, UCharCategory type);

/**
 * Enumerate efficiently all code points with their Unicode general categories.
 *
 * This is useful for building data structures (e.g., UnicodeSet's),
 * for enumerating all assigned code points (type!=U_UNASSIGNED), etc.
 *
 * For each contiguous range of code points with a given general category ("character type"),
 * the UCharEnumTypeRange function is called.
 * Adjacent ranges have different types.
 * The Unicode Standard guarantees that the numeric value of the type is 0..31.
 *
 * @param enumRange a pointer to a function that is called for each contiguous range
 *                  of code points with the same general category
 * @param context an opaque pointer that is passed on to the callback function
 *
 * @stable ICU 2.1
 * @see UCharCategory
 * @see UCharEnumTypeRange
 */
U_STABLE void U_EXPORT2
u_enumCharTypes(UCharEnumTypeRange *enumRange, const void *context);


/**
 * Returns the decimal digit value of a decimal digit character.
 * Such characters have the general category "Nd" (decimal digit numbers)
 * and a Numeric_Type of Decimal.
 *
 * Unlike ICU releases before 2.6, no digit values are returned for any
 * Han characters because Han number characters are often used with a special
 * Chinese-style number format (with characters for powers of 10 in between)
 * instead of in decimal-positional notation.
 * Unicode 4 explicitly assigns Han number characters the Numeric_Type
 * Numeric instead of Decimal.
 * See Jitterbug 1483 for more details.
 *
 * Use u_getIntPropertyValue(c, UCHAR_NUMERIC_TYPE) and u_getNumericValue()
 * for complete numeric Unicode properties.
 *
 * @param c the code point for which to get the decimal digit value
 * @return the decimal digit value of c,
 *         or -1 if c is not a decimal digit character
 *
 * @see u_getNumericValue
 * @stable ICU 2.0
 */
U_STABLE int32_t U_EXPORT2
u_charDigitValue(UChar32 c);


/**
 * Retrieve the name of a Unicode character.
 * Depending on <code>nameChoice</code>, the character name written
 * into the buffer is the "modern" name or the name that was defined
 * in Unicode version 1.0.
 * The name contains only "invariant" characters
 * like A-Z, 0-9, space, and '-'.
 * Unicode 1.0 names are only retrieved if they are different from the modern
 * names and if the data file contains the data for them. gennames may or may
 * not be called with a command line option to include 1.0 names in unames.dat.
 *
 * @param code The character (code point) for which to get the name.
 *             It must be <code>0<=code<=0x10ffff</code>.
 * @param nameChoice Selector for which name to get.
 * @param buffer Destination address for copying the name.
 *               The name will always be zero-terminated.
 *               If there is no name, then the buffer will be set to the empty string.
 * @param bufferLength <code>==sizeof(buffer)</code>
 * @param pErrorCode Pointer to a UErrorCode variable;
 *        check for <code>U_SUCCESS()</code> after <code>u_charName()</code>
 *        returns.
 * @return The length of the name, or 0 if there is no name for this character.
 *         If the bufferLength is less than or equal to the length, then the buffer
 *         contains the truncated name and the returned length indicates the full
 *         length of the name.
 *         The length does not include the zero-termination.
 *
 * @see UCharNameChoice
 * @see u_charFromName
 * @see u_enumCharNames
 * @stable ICU 2.0
 */
U_STABLE int32_t U_EXPORT2
u_charName(UChar32 code, UCharNameChoice nameChoice,
           char *buffer, int32_t bufferLength,
           UErrorCode *pErrorCode);

/**
 * Get the ISO 10646 comment for a character.
 * The ISO 10646 comment is an informative field in the Unicode Character
 * Database (UnicodeData.txt field 11) and is from the ISO 10646 names list.
 *
 * @param c The character (code point) for which to get the ISO comment.
 *             It must be <code>0<=c<=0x10ffff</code>.
 * @param dest Destination address for copying the comment.
 *             The comment will be zero-terminated if possible.
 *             If there is no comment, then the buffer will be set to the empty string.
 * @param destCapacity <code>==sizeof(dest)</code>
 * @param pErrorCode Pointer to a UErrorCode variable;
 *        check for <code>U_SUCCESS()</code> after <code>u_getISOComment()</code>
 *        returns.
 * @return The length of the comment, or 0 if there is no comment for this character.
 *         If the destCapacity is less than or equal to the length, then the buffer
 *         contains the truncated name and the returned length indicates the full
 *         length of the name.
 *         The length does not include the zero-termination.
 *
 * @stable ICU 2.2
 */
U_STABLE int32_t U_EXPORT2
u_getISOComment(UChar32 c,
                char *dest, int32_t destCapacity,
                UErrorCode *pErrorCode);

/**
 * Find a Unicode character by its name and return its code point value.
 * The name is matched exactly and completely.
 * If the name does not correspond to a code point, <i>pErrorCode</i>
 * is set to <code>U_INVALID_CHAR_FOUND</code>.
 * A Unicode 1.0 name is matched only if it differs from the modern name.
 * Unicode names are all uppercase. Extended names are lowercase followed
 * by an uppercase hexadecimal number, and within angle brackets.
 *
 * @param nameChoice Selector for which name to match.
 * @param name The name to match.
 * @param pErrorCode Pointer to a UErrorCode variable
 * @return The Unicode value of the code point with the given name,
 *         or an undefined value if there is no such code point.
 *
 * @see UCharNameChoice
 * @see u_charName
 * @see u_enumCharNames
 * @stable ICU 1.7
 */
U_STABLE UChar32 U_EXPORT2
u_charFromName(UCharNameChoice nameChoice,
               const char *name,
               UErrorCode *pErrorCode);

/**
 * Type of a callback function for u_enumCharNames() that gets called
 * for each Unicode character with the code point value and
 * the character name.
 * If such a function returns FALSE, then the enumeration is stopped.
 *
 * @param context The context pointer that was passed to u_enumCharNames().
 * @param code The Unicode code point for the character with this name.
 * @param nameChoice Selector for which kind of names is enumerated.
 * @param name The character's name, zero-terminated.
 * @param length The length of the name.
 * @return TRUE if the enumeration should continue, FALSE to stop it.
 *
 * @see UCharNameChoice
 * @see u_enumCharNames
 * @stable ICU 1.7
 */
typedef UBool U_CALLCONV UEnumCharNamesFn(void *context,
                               UChar32 code,
                               UCharNameChoice nameChoice,
                               const char *name,
                               int32_t length);

/**
 * Enumerate all assigned Unicode characters between the start and limit
 * code points (start inclusive, limit exclusive) and call a function
 * for each, passing the code point value and the character name.
 * For Unicode 1.0 names, only those are enumerated that differ from the
 * modern names.
 *
 * @param start The first code point in the enumeration range.
 * @param limit One more than the last code point in the enumeration range
 *              (the first one after the range).
 * @param fn The function that is to be called for each character name.
 * @param context An arbitrary pointer that is passed to the function.
 * @param nameChoice Selector for which kind of names to enumerate.
 * @param pErrorCode Pointer to a UErrorCode variable
 *
 * @see UCharNameChoice
 * @see UEnumCharNamesFn
 * @see u_charName
 * @see u_charFromName
 * @stable ICU 1.7
 */
U_STABLE void U_EXPORT2
u_enumCharNames(UChar32 start, UChar32 limit,
                UEnumCharNamesFn *fn,
                void *context,
                UCharNameChoice nameChoice,
                UErrorCode *pErrorCode);

/**
 * Return the Unicode name for a given property, as given in the
 * Unicode database file PropertyAliases.txt.
 *
 * In addition, this function maps the property
 * UCHAR_GENERAL_CATEGORY_MASK to the synthetic names "gcm" /
 * "General_Category_Mask".  These names are not in
 * PropertyAliases.txt.
 *
 * @param property UProperty selector other than UCHAR_INVALID_CODE.
 *         If out of range, NULL is returned.
 *
 * @param nameChoice selector for which name to get.  If out of range,
 *         NULL is returned.  All properties have a long name.  Most
 *         have a short name, but some do not.  Unicode allows for
 *         additional names; if present these will be returned by
 *         U_LONG_PROPERTY_NAME + i, where i=1, 2,...
 *
 * @return a pointer to the name, or NULL if either the
 *         property or the nameChoice is out of range.  If a given
 *         nameChoice returns NULL, then all larger values of
 *         nameChoice will return NULL, with one exception: if NULL is
 *         returned for U_SHORT_PROPERTY_NAME, then
 *         U_LONG_PROPERTY_NAME (and higher) may still return a
 *         non-NULL value.  The returned pointer is valid until
 *         u_cleanup() is called.
 *
 * @see UProperty
 * @see UPropertyNameChoice
 * @stable ICU 2.4
 */
U_STABLE const char* U_EXPORT2
u_getPropertyName(UProperty property,
                  UPropertyNameChoice nameChoice);

/**
 * Return the UProperty enum for a given property name, as specified
 * in the Unicode database file PropertyAliases.txt.  Short, long, and
 * any other variants are recognized.
 *
 * In addition, this function maps the synthetic names "gcm" /
 * "General_Category_Mask" to the property
 * UCHAR_GENERAL_CATEGORY_MASK.  These names are not in
 * PropertyAliases.txt.
 *
 * @param alias the property name to be matched.  The name is compared
 *         using "loose matching" as described in PropertyAliases.txt.
 *
 * @return a UProperty enum, or UCHAR_INVALID_CODE if the given name
 *         does not match any property.
 *
 * @see UProperty
 * @stable ICU 2.4
 */
U_STABLE UProperty U_EXPORT2
u_getPropertyEnum(const char* alias);

/**
 * Return the Unicode name for a given property value, as given in the
 * Unicode database file PropertyValueAliases.txt.
 *
 * Note: Some of the names in PropertyValueAliases.txt can only be
 * retrieved using UCHAR_GENERAL_CATEGORY_MASK, not
 * UCHAR_GENERAL_CATEGORY.  These include: "C" / "Other", "L" /
 * "Letter", "LC" / "Cased_Letter", "M" / "Mark", "N" / "Number", "P"
 * / "Punctuation", "S" / "Symbol", and "Z" / "Separator".
 *
 * @param property UProperty selector constant.
 *        Must be UCHAR_BINARY_START<=which<UCHAR_BINARY_LIMIT
 *        or UCHAR_INT_START<=which<UCHAR_INT_LIMIT
 *        or UCHAR_MASK_START<=which<UCHAR_MASK_LIMIT.
 *        If out of range, NULL is returned.
 *
 * @param value selector for a value for the given property.  If out
 *         of range, NULL is returned.  In general, valid values range
 *         from 0 up to some maximum.  There are a few exceptions:
 *         (1.) UCHAR_BLOCK values begin at the non-zero value
 *         UBLOCK_BASIC_LATIN.  (2.)  UCHAR_CANONICAL_COMBINING_CLASS
 *         values are not contiguous and range from 0..240.  (3.)
 *         UCHAR_GENERAL_CATEGORY_MASK values are not values of
 *         UCharCategory, but rather mask values produced by
 *         U_GET_GC_MASK().  This allows grouped categories such as
 *         [:L:] to be represented.  Mask values range
 *         non-contiguously from 1..U_GC_P_MASK.
 *
 * @param nameChoice selector for which name to get.  If out of range,
 *         NULL is returned.  All values have a long name.  Most have
 *         a short name, but some do not.  Unicode allows for
 *         additional names; if present these will be returned by
 *         U_LONG_PROPERTY_NAME + i, where i=1, 2,...

 * @return a pointer to the name, or NULL if either the
 *         property or the nameChoice is out of range.  If a given
 *         nameChoice returns NULL, then all larger values of
 *         nameChoice will return NULL, with one exception: if NULL is
 *         returned for U_SHORT_PROPERTY_NAME, then
 *         U_LONG_PROPERTY_NAME (and higher) may still return a
 *         non-NULL value.  The returned pointer is valid until
 *         u_cleanup() is called.
 *
 * @see UProperty
 * @see UPropertyNameChoice
 * @stable ICU 2.4
 */
U_STABLE const char* U_EXPORT2
u_getPropertyValueName(UProperty property,
                       int32_t value,
                       UPropertyNameChoice nameChoice);

/**
 * Return the property value integer for a given value name, as
 * specified in the Unicode database file PropertyValueAliases.txt.
 * Short, long, and any other variants are recognized.
 *
 * Note: Some of the names in PropertyValueAliases.txt will only be
 * recognized with UCHAR_GENERAL_CATEGORY_MASK, not
 * UCHAR_GENERAL_CATEGORY.  These include: "C" / "Other", "L" /
 * "Letter", "LC" / "Cased_Letter", "M" / "Mark", "N" / "Number", "P"
 * / "Punctuation", "S" / "Symbol", and "Z" / "Separator".
 *
 * @param property UProperty selector constant.
 *        Must be UCHAR_BINARY_START<=which<UCHAR_BINARY_LIMIT
 *        or UCHAR_INT_START<=which<UCHAR_INT_LIMIT
 *        or UCHAR_MASK_START<=which<UCHAR_MASK_LIMIT.
 *        If out of range, UCHAR_INVALID_CODE is returned.
 *
 * @param alias the value name to be matched.  The name is compared
 *         using "loose matching" as described in
 *         PropertyValueAliases.txt.
 *
 * @return a value integer or UCHAR_INVALID_CODE if the given name
 *         does not match any value of the given property, or if the
 *         property is invalid.  Note: U CHAR_GENERAL_CATEGORY values
 *         are not values of UCharCategory, but rather mask values
 *         produced by U_GET_GC_MASK().  This allows grouped
 *         categories such as [:L:] to be represented.
 *
 * @see UProperty
 * @stable ICU 2.4
 */
U_STABLE int32_t U_EXPORT2
u_getPropertyValueEnum(UProperty property,
                       const char* alias);

/**
 * Determines if the specified character is permissible as the
 * first character in an identifier according to Unicode
 * (The Unicode Standard, Version 3.0, chapter 5.16 Identifiers).
 * True for characters with general categories "L" (letters) and "Nl" (letter numbers).
 *
 * Same as java.lang.Character.isUnicodeIdentifierStart().
 * Same as UCHAR_ID_START
 *
 * @param c the code point to be tested
 * @return TRUE if the code point may start an identifier
 *
 * @see UCHAR_ID_START
 * @see u_isalpha
 * @see u_isIDPart
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isIDStart(UChar32 c);

/**
 * Determines if the specified character is permissible
 * in an identifier according to Java.
 * True for characters with general categories "L" (letters),
 * "Nl" (letter numbers), "Nd" (decimal digits),
 * "Mc" and "Mn" (combining marks), "Pc" (connecting punctuation), and
 * u_isIDIgnorable(c).
 *
 * Same as java.lang.Character.isUnicodeIdentifierPart().
 * Almost the same as Unicode's ID_Continue (UCHAR_ID_CONTINUE)
 * except that Unicode recommends to ignore Cf which is less than
 * u_isIDIgnorable(c).
 *
 * @param c the code point to be tested
 * @return TRUE if the code point may occur in an identifier according to Java
 *
 * @see UCHAR_ID_CONTINUE
 * @see u_isIDStart
 * @see u_isIDIgnorable
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isIDPart(UChar32 c);

/**
 * Determines if the specified character should be regarded
 * as an ignorable character in an identifier,
 * according to Java.
 * True for characters with general category "Cf" (format controls) as well as
 * non-whitespace ISO controls
 * (U+0000..U+0008, U+000E..U+001B, U+007F..U+0084, U+0086..U+009F).
 *
 * Same as java.lang.Character.isIdentifierIgnorable()
 * except that Java also returns TRUE for U+0085 Next Line
 * (it omits U+0085 from whitespace ISO controls).
 *
 * Note that Unicode just recommends to ignore Cf (format controls).
 *
 * @param c the code point to be tested
 * @return TRUE if the code point is ignorable in identifiers according to Java
 *
 * @see UCHAR_DEFAULT_IGNORABLE_CODE_POINT
 * @see u_isIDStart
 * @see u_isIDPart
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isIDIgnorable(UChar32 c);

/**
 * Determines if the specified character is permissible as the
 * first character in a Java identifier.
 * In addition to u_isIDStart(c), true for characters with
 * general categories "Sc" (currency symbols) and "Pc" (connecting punctuation).
 *
 * Same as java.lang.Character.isJavaIdentifierStart().
 *
 * @param c the code point to be tested
 * @return TRUE if the code point may start a Java identifier
 *
 * @see     u_isJavaIDPart
 * @see     u_isalpha
 * @see     u_isIDStart
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isJavaIDStart(UChar32 c);

/**
 * Determines if the specified character is permissible
 * in a Java identifier.
 * In addition to u_isIDPart(c), true for characters with
 * general category "Sc" (currency symbols).
 *
 * Same as java.lang.Character.isJavaIdentifierPart().
 *
 * @param c the code point to be tested
 * @return TRUE if the code point may occur in a Java identifier
 *
 * @see     u_isIDIgnorable
 * @see     u_isJavaIDStart
 * @see     u_isalpha
 * @see     u_isdigit
 * @see     u_isIDPart
 * @stable ICU 2.0
 */
U_STABLE UBool U_EXPORT2
u_isJavaIDPart(UChar32 c);



/**
 * Option value for case folding:
 *
 * Use the modified set of mappings provided in CaseFolding.txt to handle dotted I
 * and dotless i appropriately for Turkic languages (tr, az).
 *
 * Before Unicode 3.2, CaseFolding.txt contains mappings marked with 'I' that
 * are to be included for default mappings and
 * excluded for the Turkic-specific mappings.
 *
 * Unicode 3.2 CaseFolding.txt instead contains mappings marked with 'T' that
 * are to be excluded for default mappings and
 * included for the Turkic-specific mappings.
 *
 * @stable ICU 2.0
 */
#define U_FOLD_CASE_EXCLUDE_SPECIAL_I 1

/**
 * Returns the decimal digit value of the code point in the
 * specified radix.
 *
 * If the radix is not in the range <code>2<=radix<=36</code> or if the
 * value of <code>c</code> is not a valid digit in the specified
 * radix, <code>-1</code> is returned. A character is a valid digit
 * if at least one of the following is true:
 * <ul>
 * <li>The character has a decimal digit value.
 *     Such characters have the general category "Nd" (decimal digit numbers)
 *     and a Numeric_Type of Decimal.
 *     In this case the value is the character's decimal digit value.</li>
 * <li>The character is one of the uppercase Latin letters
 *     <code>'A'</code> through <code>'Z'</code>.
 *     In this case the value is <code>c-'A'+10</code>.</li>
 * <li>The character is one of the lowercase Latin letters
 *     <code>'a'</code> through <code>'z'</code>.
 *     In this case the value is <code>ch-'a'+10</code>.</li>
 * <li>Latin letters from both the ASCII range (0061..007A, 0041..005A)
 *     as well as from the Fullwidth ASCII range (FF41..FF5A, FF21..FF3A)
 *     are recognized.</li>
 * </ul>
 *
 * Same as java.lang.Character.digit().
 *
 * @param   ch      the code point to be tested.
 * @param   radix   the radix.
 * @return  the numeric value represented by the character in the
 *          specified radix,
 *          or -1 if there is no value or if the value exceeds the radix.
 *
 * @see     UCHAR_NUMERIC_TYPE
 * @see     u_forDigit
 * @see     u_charDigitValue
 * @see     u_isdigit
 * @stable ICU 2.0
 */
U_STABLE int32_t U_EXPORT2
u_digit(UChar32 ch, int8_t radix);

/**
 * Determines the character representation for a specific digit in
 * the specified radix. If the value of <code>radix</code> is not a
 * valid radix, or the value of <code>digit</code> is not a valid
 * digit in the specified radix, the null character
 * (<code>U+0000</code>) is returned.
 * <p>
 * The <code>radix</code> argument is valid if it is greater than or
 * equal to 2 and less than or equal to 36.
 * The <code>digit</code> argument is valid if
 * <code>0 <= digit < radix</code>.
 * <p>
 * If the digit is less than 10, then
 * <code>'0' + digit</code> is returned. Otherwise, the value
 * <code>'a' + digit - 10</code> is returned.
 *
 * Same as java.lang.Character.forDigit().
 *
 * @param   digit   the number to convert to a character.
 * @param   radix   the radix.
 * @return  the <code>char</code> representation of the specified digit
 *          in the specified radix.
 *
 * @see     u_digit
 * @see     u_charDigitValue
 * @see     u_isdigit
 * @stable ICU 2.0
 */
U_STABLE UChar32 U_EXPORT2
u_forDigit(int32_t digit, int8_t radix);

/**
 * Get the "age" of the code point.
 * The "age" is the Unicode version when the code point was first
 * designated (as a non-character or for Private Use)
 * or assigned a character.
 * This can be useful to avoid emitting code points to receiving
 * processes that do not accept newer characters.
 * The data is from the UCD file DerivedAge.txt.
 *
 * @param c The code point.
 * @param versionArray The Unicode version number array, to be filled in.
 *
 * @stable ICU 2.1
 */
U_STABLE void U_EXPORT2
u_charAge(UChar32 c, UVersionInfo versionArray);

/**
 * Gets the Unicode version information.
 * The version array is filled in with the version information
 * for the Unicode standard that is currently used by ICU.
 * For example, Unicode version 3.1.1 is represented as an array with
 * the values { 3, 1, 1, 0 }.
 *
 * @param versionArray an output array that will be filled in with
 *                     the Unicode version number
 * @stable ICU 2.0
 */
U_STABLE void U_EXPORT2
u_getUnicodeVersion(UVersionInfo versionArray);

/**
 * Get the FC_NFKC_Closure property string for a character.
 * See Unicode Standard Annex #15 for details, search for "FC_NFKC_Closure"
 * or for "FNC": http://www.unicode.org/reports/tr15/
 *
 * @param c The character (code point) for which to get the FC_NFKC_Closure string.
 *             It must be <code>0<=c<=0x10ffff</code>.
 * @param dest Destination address for copying the string.
 *             The string will be zero-terminated if possible.
 *             If there is no FC_NFKC_Closure string,
 *             then the buffer will be set to the empty string.
 * @param destCapacity <code>==sizeof(dest)</code>
 * @param pErrorCode Pointer to a UErrorCode variable.
 * @return The length of the string, or 0 if there is no FC_NFKC_Closure string for this character.
 *         If the destCapacity is less than or equal to the length, then the buffer
 *         contains the truncated name and the returned length indicates the full
 *         length of the name.
 *         The length does not include the zero-termination.
 *
 * @stable ICU 2.2
 */
U_STABLE int32_t U_EXPORT2
u_getFC_NFKC_Closure(UChar32 c, UChar *dest, int32_t destCapacity, UErrorCode *pErrorCode);

U_CDECL_END

#endif /*_UCHAR*/
/*eof*/
