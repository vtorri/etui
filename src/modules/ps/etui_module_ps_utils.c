/* Etui - Multi-document rendering library using the EFL
 * Copyright (C) 2013 Vincent Torri
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "etui_module_ps_utils.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#define ETUI_TO_LOWERCASE(c) (((c) >= 'A') && ((c) <= 'Z')) ? (c) + 32  : (c)

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

char *etui_strdup(const char *s)
{
    char *res;
    size_t len;

    if (!s)
        return NULL;

    len = strlen(s) + 1;
    res = (char *)malloc(len * sizeof(char));

    return res ? memcpy(res, s, len) : NULL;
}

int
etui_strncasecmp (const char *s1,
                  const char *s2,
                  size_t n)
{
    int c1, c2;

    while (n && *s1 && *s2)
    {
        n -= 1;
        c1 = (int)(unsigned char)ETUI_TO_LOWERCASE(*s1);
        c2 = (int)(unsigned char)ETUI_TO_LOWERCASE(*s2);
        if (c1 != c2)
            return (c1 - c2);
        s1++;
        s2++;
    }

    return (n) ? (((int)(unsigned char)*s1) - ((int)(unsigned char)*s2)) : 0;
}

int
etui_strcasecmp (const char *s1,
                 const char *s2)
{
    int c1, c2;

    while (*s1 && *s2)
    {
        c1 = (int)(unsigned char)ETUI_TO_LOWERCASE(*s1);
        c2 = (int)(unsigned char)ETUI_TO_LOWERCASE(*s2);
        if (c1 != c2)
            return (c1 - c2);
        s1++;
        s2++;
    }

    return (((int)(unsigned char)*s1) - ((int)(unsigned char)*s2));
}

/* code copied from http://www.ryanjuckett.com/programming/c-cplusplus/25-optimizing-atof-and-strtod */
/* c++ --> c */

/******************************************************************************
  Copyright (c) 2008-2010 Ryan Juckett
  http://www.ryanjuckett.com/

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

  3. This notice may not be removed or altered from any source
     distribution.
******************************************************************************/
double
etui_strtod(const char * pString, char ** ppEnd)
{
    char buffer[32];
    const char * pNumberStart;
    const char * pNumberEnd;
    unsigned int numberLen;
    double result;

    /* Find the start of the string */
    pNumberStart = pString;

    /* skip whitespace */
    while (isspace(*pNumberStart))
        ++pNumberStart;

    /* Find the end of the string */
    pNumberEnd = pNumberStart;

    /* skip optional sign */
    if ((*pNumberEnd == '-') || (*pNumberEnd == '+'))
        ++pNumberEnd;

    /* skip optional digits */
    while (isdigit(*pNumberEnd))
        ++pNumberEnd;

    /* skip optional decimal and digits */
    if (*pNumberEnd == '.')
    {
        ++pNumberEnd;
        while (isdigit(*pNumberEnd))
            ++pNumberEnd;
    }

    /* skip optional exponent */
    if ((*pNumberEnd == 'd') ||
        (*pNumberEnd == 'D') ||
        (*pNumberEnd == 'e') ||
        (*pNumberEnd == 'E'))
    {
        ++pNumberEnd;

        if ((*pNumberEnd == '-') || (*pNumberEnd == '+'))
            ++pNumberEnd;

        while (isdigit(*pNumberEnd))
            ++pNumberEnd;
    }

    /* Process the string */
    numberLen = pNumberEnd - pNumberStart;
    if ((numberLen + 1) < sizeof(buffer))
    {
        /* copy into buffer and terminate with NUL before calling the */
        /* standard function */
        memcpy(buffer, pNumberStart, numberLen * sizeof(buffer[0]));
        buffer[numberLen] = '\0';
        result = strtod( buffer, ppEnd );

        /* translate end of string back from the buffer to the input string */
        if (ppEnd)
        {
            *ppEnd = (char *)(pNumberStart + (*ppEnd-buffer));
        }

        return result;
    }
    else
    {
        /* buffer was too small so just call the standard function on the */
        /* source input to get a proper result */
        return strtod(pString, ppEnd);
    }
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
