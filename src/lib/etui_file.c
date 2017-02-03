/* Etui - Multi-document rendering library using the EFL
 * Copyright (C) 2013 Vincent Torri
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#include <Eina.h>

#include "etui_file.h"
#include "etui_private.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


/****** TIFF ******/

#ifdef ETUI_BUILD_TIFF

typedef struct
{
    unsigned short magic;
    unsigned short version;
} Tiff_Header;

#endif


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

Eina_Bool
etui_file_is_cb(const char *filename, Eina_File *f)
{
    unsigned char *base;
    size_t size;

    size = eina_file_size_get(f);
    base = eina_file_map_all(f, EINA_FILE_POPULATE);

    /* check the file name extension and the signatures */

    /* CBR */
    if (eina_str_has_extension(filename, ".cbr") &&
        (size >= 4) &&
        (base[0] == 'R') &&
        (base[1] == 'a') &&
        (base[2] == 'r') &&
        (base[3] == '!'))
        return EINA_TRUE;

    /* CBZ */
    if (eina_str_has_extension(filename, ".cbz") &&
        (size >= 4) &&
        (base[0] == 'P') &&
        (base[1] == 'K') &&
        (base[2] == 0x03) &&
        (base[3] == 0x04))
        return EINA_TRUE;

    /* CB7 */
    if (eina_str_has_extension(filename, ".cb7") &&
        (size >= 6) &&
        (base[0] == '7') &&
        (base[1] == 'z') &&
        (base[2] == 0xbc) &&
        (base[3] == 0xaf) &&
        (base[3] == 0x27) &&
        (base[3] == 0x1c))
        return EINA_TRUE;

    /* CBT */
    if (eina_str_has_extension(filename, ".cbt"))
        return EINA_TRUE;

    /* CBT */
    if (eina_str_has_extension(filename, ".cba"))
        return EINA_TRUE;

    return EINA_FALSE;
}

#ifdef ETUI_BUILD_DJVU

Eina_Bool
etui_file_is_djvu(const char *filename EINA_UNUSED, Eina_File *f)
{
    unsigned char *base;
    size_t size;

    size = eina_file_size_get(f);
    base = eina_file_map_all(f, EINA_FILE_POPULATE);
    if ((size >= 4) &&
        (base[0] == 0x41) &&
        (base[1] == 0x54) &&
        (base[2] == 0x26) &&
        (base[3] == 0x54))
        return EINA_TRUE;

    return EINA_FALSE;
}

#else

Eina_Bool
etui_file_is_djvu(const char *filename EINA_UNUSED, Eina_File *f EINA_UNUSED)
{
    return EINA_FALSE;
}

#endif /* ETUI_BUILD_DJVU */

#ifdef ETUI_BUILD_PDF

Eina_Bool
etui_file_is_pdf(const char *filename EINA_UNUSED, Eina_File *f)
{
    unsigned char *base;
    size_t size;

    size = eina_file_size_get(f);
    base = (unsigned char *)eina_file_map_all(f, EINA_FILE_POPULATE);
    if ((size >= 8) &&
        (base[0] == '%') &&
        (base[1] == 'P') &&
        (base[2] == 'D') &&
        (base[3] == 'F') &&
        (base[4] == '-') &&
        (base[5] == '1') &&
        (base[6] == '.') &&
        (base[7] >= '0') &&
        (base[7] <= '7'))
        return EINA_TRUE;

    return EINA_FALSE;
}

#else

Eina_Bool
etui_file_is_pdf(const char *filename EINA_UNUSED, Eina_File *f EINA_UNUSED)
{
    return EINA_FALSE;
}

#endif /* ETUI_BUILD_PDF */

#ifdef ETUI_BUILD_PS

Eina_Bool
etui_file_is_ps(const char *filename EINA_UNUSED, Eina_File *f)
{
    unsigned char *base;
    size_t size;

    /*
     * Available versions are:
     * 1.0
     * 1.1
     * 1.2
     * 2.0
     * 2.1
     * 3.0
     * but we check only intervals {1,2,3} for major version
     * and {0, 1, 2} or minor version
     */

    size = eina_file_size_get(f);
    base = (unsigned char *)eina_file_map_all(f, EINA_FILE_POPULATE);
    if ((size >= 14) &&
        (base[0] == '%') &&
        (base[1] == '!') &&
        (base[2] == 'P') &&
        (base[3] == 'S') &&
        (base[4] == '-') &&
        (base[5] == 'A') &&
        (base[6] == 'd') &&
        (base[7] == 'o') &&
        (base[8] == 'b') &&
        (base[9] == 'e') &&
        (base[10] == '-') &&
        (base[11] >= '1') &&
        (base[11] <= '3') &&
        (base[12] == '.') &&
        (base[13] >= '0') &&
        (base[13] <= '2'))
        return EINA_TRUE;

    return EINA_FALSE;
}

#else

Eina_Bool
etui_file_is_ps(const char *filename EINA_UNUSED, Eina_File *f EINA_UNUSED)
{
    return EINA_FALSE;
}

#endif /* ETUI_BUILD_PS */

#ifdef ETUI_BUILD_TIFF

Eina_Bool
etui_file_is_tiff(const char *filename EINA_UNUSED, Eina_File *f)
{
    Tiff_Header *hdr;
    void *base;
    size_t size;

    size = eina_file_size_get(f);
    if (size < sizeof(Tiff_Header))
        return EINA_FALSE;

    base = eina_file_map_all(f, EINA_FILE_POPULATE);
    hdr = (Tiff_Header *)base;
    if (
        (hdr->magic != 0x4d4d) && /* big endian */
        (hdr->magic != 0x4949) && /* little endian */
# if WORDS_BIGENDIAN == 1
        (hdr->magic != 0x4550) /* MDI big endian */
# else
        (hdr->magic != 0x5045) /* MDI little endian */
# endif
        )
        return EINA_FALSE;

# if WORDS_BIGENDIAN == 1
    {
        unsigned char *s;
        unsigned char tmp;

        s = (unsigned char *)(hdr + 2);
        tmp = *s;
        *s = *(s + 1);
        *(s + 1) = tmp;
    }
# endif

    if (hdr->version == 42)
    {
        if (size < 8)
            return EINA_FALSE;
    }
    else if (hdr->version == 43)
    {
        if (size < 16)
            return EINA_FALSE;
    }
    else
        return EINA_FALSE;

    return EINA_TRUE;
}

#else

Eina_Bool
etui_file_is_tiff(const char *filename EINA_UNUSED, Eina_File *f EINA_UNUSED)
{
    return EINA_FALSE;
}

#endif /* ETUI_BUILD_TIFF */


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
