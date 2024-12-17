/*
 * This file is a partially changed from the Newlib.
 * The Newlib is governed by a collection of licenses, which are BSD, or BSD-alike.
 * All these licenses are listed in at https://sourceware.org/newlib/COPYING.NEWLIB
 * According to this licensing model’s statement
 * ("Permission to use, copy, modify, and distribute this software for any purpose without fee is hereby granted ..."),
 * the library may also be used in proprietary software without any licensing issues.
 */

/*
 * strnlen.h
 *
 * Definitions for memory and strnlen functions.
 */

#ifndef _STRNLEN_H_
#define _STRNLEN_H_

/**********************************************************************************************************************
 * Function Name: strnlen
 * Description  : The <<strnlen>> function works out the length of the string
 *                starting at <<*<[str]>>> by counting chararacters until it
 *                reaches a NUL character or the maximum: <[n]> number of
 *                characters have been inspected.
 * Arguments    : str
 *              : n
 * Return Value : returns the character count or <[n]>.
 *********************************************************************************************************************/
size_t strnlen (const char *, size_t);

#endif /* _STRNLEN_H_ */
