/*
 * base64_decode.c
 * Copyright (C) Renesas Electronics Corporation and/or its affiliates.
 * All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include "base64_decode.h"

#if SECURE_BOOT
#pragma section SECURE_BOOT
#endif

/* base64 table */
const static char base64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/=";

/***********************************************************************************************************************
* Function Name: base64_decode
* Description  : http://apis.jpn.ph/fswiki/wiki.cgi?page=ScrapCode%2FC%2FBASE64
* Arguments    : *source
*                *result
*                size
* Return Value : j
***********************************************************************************************************************/
uint32_t base64_decode(uint8_t *source, uint8_t *result, uint32_t size)
{
    uint8_t ind1;
    uint8_t ind2;
    uint8_t ind3;
    uint8_t ind4;
    uint32_t i;
    uint32_t j;
    uint8_t tmp[3];
    uint8_t *ptr;

    for( i = 0, j = 0 ; i < size; i += 4 )
    {
        /* get the character code */
        ind1 = ('=' == (*((source + i) + 0))) ? 0 : (strchr(base64, *((source + i) + 0)) - base64);
        ind2 = ('=' == (*((source + i) + 1))) ? 0 : (strchr(base64, *((source + i) + 1)) - base64);
        ind3 = ('=' == (*((source + i) + 2))) ? 0 : (strchr(base64, *((source + i) + 2)) - base64);
        ind4 = ('=' == (*((source + i) + 3))) ? 0 : (strchr(base64, *((source + i) + 3)) - base64);

		/* decode */
		tmp[0] = (uint8_t)( ((ind1 & 0x3f) << 2) | ((ind2 & 0x30) >> 4) );
		tmp[1] = (uint8_t)( ((ind2 & 0x0f) << 4) | ((ind3 & 0x3c) >> 2) );
		tmp[2] = (uint8_t)( ((ind3 & 0x03) << 6) | ((ind4 & 0x3f) >> 0) );

		ptr = strstr(source, "==");
		if(ptr == (source + i))
		{
			/* nothing to do */
		}
		else if(ptr == (source + i) + 2)
		{
			result[j+0] = tmp[0];
			j += 1;
		}
		else if(ptr == (source + i) + 3)
		{
			result[j+0] = tmp[0];
			result[j+1] = tmp[1];
			j += 2;
		}
		else
		{
			result[j+0] = tmp[0];
			result[j+1] = tmp[1];
			result[j+2] = tmp[2];
			j += 3;
		}
    }

    return j;
}

#if SECURE_BOOT
#pragma section
#endif
