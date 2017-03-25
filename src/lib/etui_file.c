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
#include <Ecore.h> /* for Ecore_Thread in Etui_Module */

#include "Etui.h"
#include "etui_module.h"
#include "etui_file.h"
#include "etui_private.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


struct Etui_File_s
{
    char *filename;
    Eina_File *file;
    Etui_Module *module;
    void *base;
    size_t size;
};

/****** Comic Book ******/

static Eina_Bool
etui_file_is_cb(const char *filename,
                unsigned char *base, size_t size)
{
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

/****** DJVU ******/

#ifdef ETUI_BUILD_DJVU

static Eina_Bool
etui_file_is_djvu(const char *filename EINA_UNUSED,
                  unsigned char *base, size_t size)
{
    if ((size >= 4) &&
        (base[0] == 0x41) &&
        (base[1] == 0x54) &&
        (base[2] == 0x26) &&
        (base[3] == 0x54))
        return EINA_TRUE;

    return EINA_FALSE;
}

#else

static Eina_Bool
etui_file_is_djvu(const char *filename EINA_UNUSED,
                  unsigned char *base EINA_UNUSED, size_t size EINA_UNUSED)
{
    return EINA_FALSE;
}

#endif /* ETUI_BUILD_DJVU */

/****** EPUB ******/

#ifdef ETUI_BUILD_EPUB

static inline unsigned int
_uint32_get(unsigned char *ptr)
{
  return ptr[0] | (ptr[1] << 8) | (ptr[2] << 16) | (ptr[3] << 24);
}

static inline unsigned short
_uint16_get(unsigned char *ptr)
{
  return ptr[0] | (ptr[1] << 8);
}

static Eina_Bool
etui_file_is_epub(const char *filename EINA_UNUSED,
                  unsigned char *base, size_t size)
{
    if ((sz >= 48) && /* first local file header should be large enough */
        (_uint32_get(base) == 0x04034b50) && /* ZIP file */
        ((_uint16_get(base + 4) == 10) || /* version needed to extract */
         (_uint16_get(base + 4) == 20) || /* version needed to extract */
         (_uint16_get(base + 4) == 45)) && /* version needed to extract */
        (!(_uint16_get(base + 6) & 1)) && /* not encrypted */
        ((_uint16_get(base + 8) == 0) || /* not compressed */
         (_uint16_get(base + 8) == 8)) && /* or deflated */
        (_uint16_get(base + 26) == 8) && /* size of file name "mimetype" */
        (_uint16_get(base + 28) == 0) && /* no extra field */
        (memcmp(base + 30, "mimetype", 8) == 0) && /* file name */
        (memcmp(base + 38, "application/epub+zip", 20) == 0)) /* mimetype */
    return  EINA_TRUE;

    return EINA_FALSE;
}

#else

static Eina_Bool
etui_file_is_epub(const char *filename EINA_UNUSED,
                  unsigned char *base EINA_UNUSED, size_t size EINA_UNUSED)
{
    return EINA_FALSE;
}

#endif /* ETUI_BUILD_EPUB */

/****** PDF ******/

#ifdef ETUI_BUILD_PDF

static Eina_Bool
etui_file_is_pdf(const char *filename EINA_UNUSED,
                 unsigned char *base, size_t size)
{
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

static Eina_Bool
etui_file_is_pdf(const char *filename EINA_UNUSED,
                 unsigned char *base EINA_UNUSED, size_t size EINA_UNUSED)
{
    return EINA_FALSE;
}

#endif /* ETUI_BUILD_PDF */

/****** PostScript ******/

#ifdef ETUI_BUILD_PS

static Eina_Bool
etui_file_is_ps(const char *filename EINA_UNUSED,
                unsigned char *base, size_t size)
{
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

static Eina_Bool
etui_file_is_ps(const char *filename EINA_UNUSED,
                unsigned char *base EINA_UNUSED, size_t size EINA_UNUSED)
{
    return EINA_FALSE;
}

#endif /* ETUI_BUILD_PS */

/****** TIFF ******/

#ifdef ETUI_BUILD_TIFF

typedef struct
{
    unsigned short magic;
    unsigned short version;
} Tiff_Header;

static Eina_Bool
etui_file_is_tiff(const char *filename EINA_UNUSED,
                  unsigned char *base, size_t size)
{
    Tiff_Header *hdr;

    if (size < sizeof(Tiff_Header))
        return EINA_FALSE;

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

static Eina_Bool
etui_file_is_tiff(const char *filename EINA_UNUSED,
                  unsigned char *base EINA_UNUSED, size_t size EINA_UNUSED)
{
    return EINA_FALSE;
}

#endif /* ETUI_BUILD_TIFF */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


const Etui_Module *etui_file_module_get(const Etui_File *ef)
{
    return (ef) ? ef->module : NULL;
}

EAPI const void *
etui_file_base_get(const Etui_File *ef)
{
    return ef ? ef->base : NULL;
}

EAPI size_t
etui_file_size_get(const Etui_File *ef)
{
    return ef ? ef->size : 0;
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


EAPI Etui_File *
etui_file_new (const char *filename)
{
    char file[PATH_MAX];
    Etui_File *ef;
    Etui_Module *module;
    const char *module_name = NULL;
    char *res;

    if (!filename || !*filename)
        return NULL;

    if (eina_str_has_prefix(filename, "file://"))
        res = realpath(filename + 7, file);
    else
        res = realpath(filename, file);

    if (!res)
        return NULL;

    ef = (Etui_File *)calloc(1, sizeof(Etui_File));
    if (!ef)
        return NULL;

    /* TODO: use stringshare ?? */
    ef->filename = strdup(file);
    if (!ef->filename)
        goto free_ef;

    ef->file = eina_file_open(file, EINA_FALSE);
    if (!ef->file)
        goto free_filename;

    ef->base = eina_file_map_all(ef->file, EINA_FILE_POPULATE);
    if (!ef->base)
        goto close_file;

    ef->size = eina_file_size_get(ef->file);

    if (etui_file_is_pdf(file, ef->base, ef->size))
        module_name = "pdf";
    else if (etui_file_is_ps(file, ef->base, ef->size))
        module_name = "ps";
    else if (etui_file_is_djvu(file, ef->base, ef->size))
        module_name = "djvu";
    else if (etui_file_is_cb(file, ef->base, ef->size))
        module_name = "cb";
    else if (etui_file_is_epub(file, ef->base, ef->size))
        module_name = "epub";
    else if (etui_file_is_tiff(file, ef->base, ef->size))
        module_name = "tiff";

    /* FIXME: XPS, txt, DVI */

    INF("module name: %s", module_name);

    module = etui_module_find(module_name);
    if (!module)
    {
        Eina_List *l;
        Eina_List *ll;

        l = etui_module_list();
        EINA_LIST_FOREACH(l, ll, module_name)
        {
            module = etui_module_find(module_name);
            if (module)
                break;
        }
    }

    if (!module)
    {
        ERR("Can not find an appropriate module for file %s", filename);
        goto close_file;
    }

    module->data = module->functions->init(ef);
    if (!module->data)
    {
        ERR("can not initialize file object");
        goto unload_module;
    }

    ef->module = module;

    return ef;

  unload_module:
    etui_module_unload(ef->module);
  close_file:
    eina_file_close(ef->file);
  free_filename:
    free(ef->filename);
  free_ef:
    free(ef);

    return NULL;
}

EAPI void
etui_file_del(Etui_File *ef)
{
    if (!ef)
        return;

    etui_module_unload(ef->module);
    eina_file_close(ef->file);
    free(ef->filename);
    free(ef);
}

EAPI const char *
etui_file_filename_get(const Etui_File *ef)
{
    return ef ? ef->filename : NULL;
}
