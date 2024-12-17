/*
 * This file is a partially changed from the Newlib.
 * The Newlib is governed by a collection of licenses, which are BSD, or BSD-alike.
 * All these licenses are listed in at https://sourceware.org/newlib/COPYING.NEWLIB
 * According to this licensing model’s statement
 * ("Permission to use, copy, modify, and distribute this software for any purpose without fee is hereby granted ..."),
 * the library may also be used in proprietary software without any licensing issues.
 */

/* 
FUNCTION
    <<strnlen>>---character string length

INDEX
    strnlen

SYNOPSIS
    #include <string.h>
    size_t strnlen(const char *<[str]>, size_t <[n]>);

DESCRIPTION
    The <<strnlen>> function works out the length of the string
    starting at <<*<[str]>>> by counting chararacters until it
    reaches a NUL character or the maximum: <[n]> number of
        characters have been inspected.

RETURNS
    <<strnlen>> returns the character count or <[n]>.

PORTABILITY
<<strnlen>> is a GNU extension.

<<strnlen>> requires no supporting OS subroutines.

*/

#undef __STRICT_ANSI__
#include <string.h>

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
size_t strnlen(const char *str, size_t n)
{
    const char *start = str;

    while (n-- > 0 && *str)
        str++;

    return str - start;
}
/*****************************************************************************************
End of function strnlen
****************************************************************************************/
