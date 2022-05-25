/*
 **********************************************************************
 *   Copyright (C) 1997-2008, International Business Machines
 *   Corporation and others.  All Rights Reserved.
 **********************************************************************
 *
 * File USCRIPT.H
 *
 * Modification History:
 *
 *   Date        Name        Description
 *   07/06/2001    Ram         Creation.
 ******************************************************************************
 */

#ifndef USCRIPT_H
#define USCRIPT_H
#include "unicode/uconfig.h"
#include "unicode/utypes.h"
#include "unicode/urename.h"

/**
 * \file
 * \brief C API: Unicode Script Information
 */


/**
 * Gets script codes associated with the given locale or ISO 15924 abbreviation or name. 
 * Fills in USCRIPT_MALAYALAM given "Malayam" OR "Mlym".
 * Fills in USCRIPT_LATIN given "en" OR "en_US" 
 * If required capacity is greater than capacity of the destination buffer then the error code
 * is set to U_BUFFER_OVERFLOW_ERROR and the required capacity is returned
 *
 * <p>Note: To search by short or long script alias only, use
 * u_getPropertyValueEnum(UCHAR_SCRIPT, alias) instead.  This does
 * a fast lookup with no access of the locale data.
 * @param nameOrAbbrOrLocale name of the script, as given in
 * PropertyValueAliases.txt, or ISO 15924 code or locale
 * @param fillIn the UScriptCode buffer to fill in the script code
 * @param capacity the capacity (size) fo UScriptCode buffer passed in.
 * @param err the error status code.
 * @return The number of script codes filled in the buffer passed in 
 * @stable ICU 2.4
 */
U_STABLE int32_t  U_EXPORT2 
uscript_getCode(const char* nameOrAbbrOrLocale,UScriptCode* fillIn,int32_t capacity,UErrorCode *err);

/**
 * Gets a script name associated with the given script code. 
 * Returns  "Malayam" given USCRIPT_MALAYALAM
 * @param scriptCode UScriptCode enum
 * @return script long name as given in
 * PropertyValueAliases.txt, or NULL if scriptCode is invalid
 * @stable ICU 2.4
 */
U_STABLE const char*  U_EXPORT2 
uscript_getName(UScriptCode scriptCode);

/**
 * Gets a script name associated with the given script code. 
 * Returns  "Mlym" given USCRIPT_MALAYALAM
 * @param scriptCode UScriptCode enum
 * @return script abbreviated name as given in
 * PropertyValueAliases.txt, or NULL if scriptCode is invalid
 * @stable ICU 2.4
 */
U_STABLE const char*  U_EXPORT2 
uscript_getShortName(UScriptCode scriptCode);

/** 
 * Gets the script code associated with the given codepoint.
 * Returns USCRIPT_MALAYALAM given 0x0D02 
 * @param codepoint UChar32 codepoint
 * @param err the error status code.
 * @return The UScriptCode, or 0 if codepoint is invalid 
 * @stable ICU 2.4
 */
U_STABLE UScriptCode  U_EXPORT2 
uscript_getScript(UChar32 codepoint, UErrorCode *err);

#endif


