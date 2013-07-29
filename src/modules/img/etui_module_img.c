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

#include <Eina.h>
#include <Evas.h>

#ifdef HAVE_ZLIB
# include <zlib.h>
#endif

#include "unrar/raros.hpp"
#include "unrar/dll.hpp"

#include "Etui.h"
#include "etui_module.h"
#include "etui_module_img.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#ifdef ETUI_MODULE_IMG_DEFAULT_LOG_COLOR
# undef ETUI_MODULE_IMG_DEFAULT_LOG_COLOR
#endif
#define ETUI_MODULE_IMG_DEFAULT_LOG_COLOR EINA_COLOR_LIGHTCYAN

#ifdef ERR
# undef ERR
#endif
#define ERR(...)  EINA_LOG_DOM_ERR(_etui_module_img_log_domain, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...)  EINA_LOG_DOM_DBG(_etui_module_img_log_domain, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...)  EINA_LOG_DOM_INFO(_etui_module_img_log_domain, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...)  EINA_LOG_DOM_WARN(_etui_module_img_log_domain, __VA_ARGS__)

#ifdef CRIT
# undef CRIT
#endif
#define CRIT(...) EINA_LOG_DOM_CRIT(_etui_module_img_log_domain, __VA_ARGS__)

#ifdef min
# undef min
#endif
#define min(a, b) ((a) < (b)) ? (a) : (b)

typedef enum
{
    ETUI_IMG_CB_NONE,
    ETUI_IMG_CB_CBZ,
    ETUI_IMG_CB_CBR,
    ETUI_IMG_CB_CBA,
    ETUI_IMG_CB_CB7,
    ETUI_IMG_CB_CBT,
} Etui_Img_Cb_Type;

#ifdef HAVE_ZLIB
typedef struct _Etui_Img_Cbz_Data Etui_Img_Cbz_Data;

struct _Etui_Img_Cbz_Data
{
    char *file_name;
    int file_offset;
};
#endif /* HAVE_ZLIB */

typedef struct _Etui_Img_Cbr_Data Etui_Img_Cbr_Data;

struct _Etui_Img_Cbr_Data
{
    char *file_name;
    unsigned long long file_size;
    unsigned int idx;
};

typedef struct _Etui_Provider_Data Etui_Provider_Data;

struct _Etui_Provider_Data
{
    /* specific EFL stuff for the module */

    struct {
        Evas_Object *obj;
        void *m;
    } efl;

    /* specific IMG stuff for the module */

    /* Document */
    struct
    {
        char *filename;
        int page_nbr;
        Eina_Array toc;
        Etui_Img_Cb_Type cb_type;
#ifdef HAVE_ZLIB
        struct
        {
            Eina_File *file;
            unsigned char *data;
            unsigned char *iter;
            size_t size;
        } zip;
#endif
    } doc;

    /* Current page */
    struct
    {
        int width;
        int height;
        int page_num;
        Etui_Rotation rotation;
        float hscale;
        float vscale;
        struct
        {
            void *data;
            unsigned long long size;
            unsigned long long offset;
            unsigned int getting_data : 1;
        } rar;
    } page;
};

static int _etui_module_img_init_count = 0;
static int _etui_module_img_log_domain = -1;

#ifdef HAVE_ZLIB

static int
_etui_img_cbz_int16_get(Etui_Provider_Data *pd)
{
    int a;
    int b;

    a = *(pd->doc.zip.iter++);
    b = *(pd->doc.zip.iter++);

    return a | (b << 8);
}

static int
_etui_img_cbz_int32_get(Etui_Provider_Data *pd)
{
    int a;
    int b;
    int c;
    int d;

    a = *(pd->doc.zip.iter++);
    b = *(pd->doc.zip.iter++);
    c = *(pd->doc.zip.iter++);
    d = *(pd->doc.zip.iter++);

    return a | (b << 8) | (c << 16) | (d << 24);
}

static int
_etui_img_cbz_sort_cb(const void *d1, const void *d2)
{
  return strcasecmp(((Etui_Img_Cbz_Data *)d1)->file_name,
                    ((Etui_Img_Cbz_Data *)d2)->file_name);
}

static void *
_etui_img_cbz_malloc(void *p EINA_UNUSED, unsigned int items, unsigned int size)
{
    return malloc(items * size);
}

static void
_etui_img_cbz_free(void *p EINA_UNUSED, void *ptr)
{
    free(ptr);
}

static unsigned char *
_etui_img_cbz_record_get(Etui_Provider_Data *pd, int offset, int *size)
{
    unsigned char *compressed_data;
    int signature;
    int method;
    int compressed_size;
    int uncompressed_size;
    int file_name_length;
    int extra_field_length;
    int dummy;

    pd->doc.zip.iter = pd->doc.zip.data + offset;

    signature = _etui_img_cbz_int32_get(pd);
    if (signature != 0x04034b50 )
        goto _err;

    dummy = _etui_img_cbz_int16_get(pd);
    dummy = _etui_img_cbz_int16_get(pd);
    method = _etui_img_cbz_int16_get(pd);
    dummy = _etui_img_cbz_int16_get(pd);
    dummy = _etui_img_cbz_int16_get(pd);
    dummy = _etui_img_cbz_int32_get(pd);
    compressed_size = _etui_img_cbz_int32_get(pd);
    uncompressed_size = _etui_img_cbz_int32_get(pd);
    file_name_length = _etui_img_cbz_int16_get(pd);
    extra_field_length = _etui_img_cbz_int16_get(pd);

    pd->doc.zip.iter += file_name_length + extra_field_length;

    compressed_data = (unsigned char *)malloc(compressed_size * sizeof(unsigned char));
    if (!compressed_data)
        goto _err;

    printf(" ** method : %d\n", method);

    memcpy(compressed_data, pd->doc.zip.iter, compressed_size);
    pd->doc.zip.iter += compressed_size;

    if (method == 0)
    {
        *size = uncompressed_size;
        return compressed_data;
    }

    if (method == 8)
    {
        unsigned char *uncompressed_data;
        z_stream stream;
        int res;

        uncompressed_data = (unsigned char *)malloc(uncompressed_size * sizeof(unsigned char));
        if (!uncompressed_data)
            goto free_compressed_data;

        memset(&stream, 0, sizeof(stream));
        stream.zalloc = _etui_img_cbz_malloc;
        stream.zfree = _etui_img_cbz_free;
        stream.opaque = pd;
        stream.next_in = compressed_data;
        stream.avail_in = compressed_size;
        stream.next_out = uncompressed_data;
        stream.avail_out = uncompressed_size;

        res = inflateInit2(&stream, -15);
        if (res != Z_OK)
            goto free_uncompressed_data;

        res = inflate(&stream, Z_FINISH);
        if (res != Z_STREAM_END)
            goto end_inflate;

        res = inflateEnd(&stream);
        if (res != Z_OK)
            goto end_inflate;

        free(compressed_data);
        *size = uncompressed_size;
        return uncompressed_data;

      end_inflate:
        inflateEnd(&stream);
      free_uncompressed_data:
        free(uncompressed_data);
      free_compressed_data:
        free(compressed_data);
    }

  _err:
    *size = 0;
    return NULL;

    (void)dummy;
}

static Eina_Bool
_etui_img_cbz_central_directory_parse(Etui_Provider_Data *pd, size_t start_offset)
{
    Eina_List *list = NULL;
    Eina_List *l = NULL;
    Etui_Img_Cbz_Data *data;
    int signature;
    int records_nbr;
    int offset;
    int dummy;
    int i;

    pd->doc.zip.iter = pd->doc.zip.data + start_offset;

    signature = _etui_img_cbz_int32_get(pd);
    if (signature != 0x06054b50)
        return EINA_FALSE;

    dummy = _etui_img_cbz_int16_get(pd);
    dummy = _etui_img_cbz_int16_get(pd);
    dummy = _etui_img_cbz_int16_get(pd);
    records_nbr = _etui_img_cbz_int16_get(pd);
    dummy = _etui_img_cbz_int32_get(pd);
    offset = _etui_img_cbz_int32_get(pd);

    pd->doc.zip.iter = pd->doc.zip.data + offset;

    for (i = 0; i < records_nbr; i++)
    {
        char *file_name;
        int file_name_length;
        int extra_field_length;
        int file_comment_length;

        signature = _etui_img_cbz_int32_get(pd);
        if (signature != 0x02014b50)
            goto free_list;

        dummy = _etui_img_cbz_int16_get(pd);
        dummy = _etui_img_cbz_int16_get(pd);
        dummy = _etui_img_cbz_int16_get(pd);
        dummy = _etui_img_cbz_int16_get(pd);
        dummy = _etui_img_cbz_int16_get(pd);
        dummy = _etui_img_cbz_int16_get(pd);
        dummy = _etui_img_cbz_int32_get(pd);
        dummy = _etui_img_cbz_int32_get(pd);
        dummy = _etui_img_cbz_int32_get(pd);
        file_name_length = _etui_img_cbz_int16_get(pd);
        extra_field_length = _etui_img_cbz_int16_get(pd);
        file_comment_length = _etui_img_cbz_int16_get(pd);
        dummy = _etui_img_cbz_int16_get(pd);
        dummy = _etui_img_cbz_int16_get(pd);
        dummy = _etui_img_cbz_int32_get(pd);
        offset = _etui_img_cbz_int32_get(pd);

        file_name = (char *)malloc((file_name_length + 1) * sizeof(char));
        if (!file_name)
            goto free_list;

        memcpy(file_name, pd->doc.zip.iter, file_name_length);
        file_name[file_name_length] = '\0';

        data = (Etui_Img_Cbz_Data *)malloc(sizeof(Etui_Img_Cbz_Data));
        if (!data)
            goto free_list;

        data->file_name = file_name;
        data->file_offset = offset;

        list = eina_list_append(list, data);

        pd->doc.zip.iter += file_name_length + extra_field_length + file_comment_length;
    }

    pd->doc.cb_type = ETUI_IMG_CB_CBZ;
    pd->doc.toc = *eina_array_new(eina_list_count(list));
    list = eina_list_sort(list, eina_list_count(list), _etui_img_cbz_sort_cb);
    EINA_LIST_FOREACH(list, l, data)
        eina_array_push(&pd->doc.toc, data);
    eina_list_free(list);

    pd->doc.page_nbr = eina_array_count(&pd->doc.toc);
    pd->page.page_num = 0;

    return EINA_TRUE;

  free_list:
    EINA_LIST_FREE(list, data)
        free(data);
    eina_list_free(list);

    return EINA_FALSE;

    (void)dummy;
}

#endif /* HAVE_ZLIB*/

static int
_etui_img_cbr_sort_cb(const void *d1, const void *d2)
{
  return strcasecmp(((Etui_Img_Cbr_Data *)d1)->file_name,
                    ((Etui_Img_Cbr_Data *)d2)->file_name);
}

static int CALLBACK
_etui_img_cbr_cb(UINT msg, LPARAM user_data, LPARAM p1, LPARAM p2)
{
    switch(msg)
    {
        case UCM_CHANGEVOLUMEW:
            break;
        case UCM_PROCESSDATA:
        {
            Etui_Provider_Data *pd;

            if (!user_data)
                return 0;

            pd = (Etui_Provider_Data *)user_data;
            if (pd->page.rar.getting_data)
            {
              printf("  offset %llu   size %llu\n", pd->page.rar.offset, pd->page.rar.size);
                if (pd->page.rar.offset < pd->page.rar.size)
                {
                  printf("  rar size  %zu\n", (size_t)p2);
                  memcpy((unsigned char *)pd->page.rar.data + pd->page.rar.offset, (void *)p1, (size_t)p2);
                  pd->page.rar.offset += p2;
                }
            }

            return 1;
        }
    case UCM_NEEDPASSWORDW:
      break;
    default:
      return 0;
    }

  return 0;
}

static Eina_Bool
_etui_img_cb_is_valid(Etui_Provider_Data *pd)
{
    Eina_Bool res = EINA_FALSE;

    /* pd is valid */

    /* cbz */

#ifdef HAVE_ZLIB
    {
        char buf[512];
        size_t back;
        size_t maxback;
        int n;
        int i;

        pd->doc.zip.file = eina_file_open(pd->doc.filename, EINA_FALSE);
        if (!pd->doc.zip.file)
            return EINA_FALSE;
        pd->doc.zip.data = eina_file_map_all(pd->doc.zip.file,
                                             EINA_FILE_POPULATE);
        pd->doc.zip.size = eina_file_size_get(pd->doc.zip.file);

        maxback = min(pd->doc.zip.size, 0xffff + sizeof(buf));
        back = min(maxback, sizeof(buf));

        while (back < maxback)
        {
            pd->doc.zip.iter = pd->doc.zip.data + (pd->doc.zip.size - back);
            if ((long long)(pd->doc.zip.iter - pd->doc.zip.data) <= (long long)(pd->doc.zip.size - 512))
                n = 512;
            else
                n = pd->doc.zip.size - (pd->doc.zip.iter - pd->doc.zip.data);
            pd->doc.zip.iter += n;
            for (i = n - 4; i > 0; i--)
            {
                if (!memcmp(pd->doc.zip.iter - n + i, "PK\5\6", 4))
                {
                  printf(" * bon !\n");
                    if (_etui_img_cbz_central_directory_parse(pd, pd->doc.zip.size - back + i))
                        return EINA_TRUE;
                }
            }
            back += sizeof(buf) - 4;
        }
    }
#endif /* HAVE_ZLIB */

    /* cbr */

    {
        char buf[16384];
        struct RAROpenArchiveDataEx rar_archive_data;
        struct RARHeaderDataEx rar_header_data;
        HANDLE rar_archive;
        int rar_header_code;
        int rar_process_file_code;
        Eina_List *list = NULL;
        Eina_List *l;
        Etui_Img_Cbr_Data *data;
        int idx;

        memset(&rar_archive_data, 0, sizeof(rar_archive_data));
        rar_archive_data.ArcName = pd->doc.filename;
        rar_archive_data.CmtBuf = buf;
        rar_archive_data.CmtBufSize = sizeof(buf);
        rar_archive_data.OpenMode = RAR_OM_LIST;
        rar_archive_data.Callback = NULL;
        rar_archive_data.UserData = 0;
        rar_archive = RAROpenArchiveEx(&rar_archive_data);

        if (rar_archive_data.OpenResult != 0)
        {
            INF("Can not open RAR archive %s", pd->doc.filename);
            RARCloseArchive(rar_archive);
            goto try_cba;
        }

        INF("Comic Book %s successfully opened", pd->doc.filename);
        INF("Volume: %s", (rar_archive_data.Flags & 1) ? "yes" : "no");
        INF("Comment: %s", (rar_archive_data.Flags & 2) ? "yes" : "no");
        INF("Locked: %s", (rar_archive_data.Flags & 4) ? "yes" : "no");
        INF("Solid: %s", (rar_archive_data.Flags & 8) ? "yes" : "no");
        INF("New Naming: %s", (rar_archive_data.Flags & 16) ? "yes" : "no");
        INF("Authenticity: %s", (rar_archive_data.Flags & 32) ? "yes" : "no");
        INF("Recovery: %s", (rar_archive_data.Flags & 64) ? "yes" : "no");
        INF("Encrypted headers: %s", (rar_archive_data.Flags & 128) ? "yes" : "no");
        INF("First Volume: %s", (rar_archive_data.Flags & 256) ? "yes" : "no");
        if (rar_archive_data.CmtState == 1)
            INF("Comment Description: %s", buf);

        rar_header_data.CmtBuf = NULL;
        memset(&rar_archive_data.Reserved, 0, sizeof(rar_archive_data.Reserved));

        idx = 0;
        while ((rar_header_code = RARReadHeaderEx(rar_archive, &rar_header_data)) == 0)
        {
            long long size;

            size = rar_header_data.UnpSize + (((long long)rar_header_data.UnpSizeHigh) << 32);
            printf("file name : %s %10Ld\n", rar_header_data.FileName, size);
            data = (Etui_Img_Cbr_Data *)malloc(sizeof(Etui_Img_Cbr_Data));
            if (data)
            {
                data->file_name = strdup(rar_header_data.FileName);
                if (data->file_name)
                {
                    data->file_size = size;
                    data->idx = idx;
                    list = eina_list_append(list, data);
                }
                else
                    free(data);
            }
            rar_process_file_code = RARProcessFile(rar_archive, RAR_SKIP,
                                                   NULL, NULL);
            if (rar_process_file_code != 0)
            {
                RARCloseArchive(rar_archive);
                return EINA_FALSE;
            }
            idx++;
        }

        pd->doc.cb_type = ETUI_IMG_CB_CBR;
        pd->doc.toc = *eina_array_new(eina_list_count(list));
        list = eina_list_sort(list, eina_list_count(list), _etui_img_cbr_sort_cb);
        EINA_LIST_FOREACH(list, l, data)
            eina_array_push(&pd->doc.toc, data);
        eina_list_free(list);

        pd->doc.page_nbr = eina_array_count(&pd->doc.toc);
        pd->page.page_num = 0;

        RARCloseArchive(rar_archive);

        return EINA_TRUE;
    }

    /* cba */

  try_cba:

    /* cb7 */

/*     try_cb7: */

    {
    }

    /* cbt */

    return res;
}

/* Virtual functions */

static void *
_etui_img_init(Evas *evas)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)calloc(1, sizeof(Etui_Provider_Data));
    if (!pd)
        return NULL;

    DBG("init module");

    pd->efl.obj = evas_object_image_add(evas);
    if (!pd->efl.obj)
        goto free_pd;

    return pd;

  free_pd:
    free(pd);

    return NULL;
}

static void
_etui_img_shutdown(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    DBG("shutdown module");

    pd = (Etui_Provider_Data *)d;

    if (pd->doc.filename)
      free(pd->doc.filename);
    evas_object_del(pd->efl.obj);
    free(pd);
}

static Evas_Object *
_etui_img_evas_object_get(void *d)
{
    if (!d)
        return NULL;

    return ((Etui_Provider_Data *)d)->efl.obj;
}

static Eina_Bool
_etui_img_file_open(void *d, const char *filename)
{
    Etui_Provider_Data *pd;

    if (!d || !filename || !*filename)
        return EINA_FALSE;

    DBG("open file %s", filename);

    pd = (Etui_Provider_Data *)d;

    if (pd->doc.filename && (strcmp(filename, pd->doc.filename) == 0))
      return EINA_TRUE;

    if (pd->doc.filename)
        free(pd->doc.filename);
    pd->doc.filename = strdup(filename);
    if (!pd->doc.filename)
        return EINA_FALSE;

    evas_object_image_file_set(pd->efl.obj, filename, NULL);
    if (evas_object_image_load_error_get(pd->efl.obj) == EVAS_LOAD_ERROR_NONE)
    {
        pd->doc.page_nbr = 1;
        pd->page.page_num = -1;
        return EINA_TRUE;
    }

    /* TODO: Comic Book */

    if (!_etui_img_cb_is_valid(pd))
    {
        ERR("Can not open Comic Book archive %s", pd->doc.filename);
        goto free_filename;
    }

    pd->page.page_num = -1;

    return EINA_TRUE;

  free_filename:
    free(pd->doc.filename);
    pd->doc.filename = NULL;

  return EINA_FALSE;
}

static void
_etui_img_file_close(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    pd = (Etui_Provider_Data *)d;

    DBG("close file %s", pd->doc.filename);

    if (pd->doc.filename)
        free(pd->doc.filename);
    pd->doc.filename = NULL;
}

static void
_etui_img_version_get(void *d EINA_UNUSED, int *maj, int *min)
{
    if (maj) *maj = 0;
    if (min) *min = 0;
}

static char *
_etui_img_title_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    return strdup(pd->doc.filename);
}

static int
_etui_img_pages_count(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return -1;

    pd = (Etui_Provider_Data *)d;

    return pd->doc.page_nbr;
}

static const Eina_Array *
_etui_img_toc_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    return &pd->doc.toc;
}

static Eina_Bool
_etui_img_page_set(void *d, int page_num)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if ((page_num < 0) || (page_num >= pd->doc.page_nbr))
        return EINA_FALSE;

    if (page_num == pd->page.page_num)
        return EINA_FALSE;

    pd->page.page_num = page_num;
    pd->page.rotation = ETUI_ROTATION_0;
    pd->page.hscale = 1.0f;
    pd->page.vscale = 1.0f;

    return EINA_TRUE;
}

static int
_etui_img_page_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return -1;

    pd = (Etui_Provider_Data *)d;

    return pd->page.page_num;
}

static void
_etui_img_page_size_get(void *d, int *width, int *height)
{
    Etui_Provider_Data *pd;

    if (!d)
        goto _err;

    pd = (Etui_Provider_Data *)d;

    evas_object_image_size_get(pd->efl.obj, width, height);

    return;

  _err:
    if (width) *width = 0;
    if (height) *height = 0;
}

static Eina_Bool
_etui_img_page_rotation_set(void *d, Etui_Rotation rotation)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (pd->page.rotation == rotation)
        return EINA_TRUE;

    pd->page.rotation = rotation;

    return EINA_TRUE;
}

static Etui_Rotation
_etui_img_page_rotation_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return ETUI_ROTATION_0;

    pd = (Etui_Provider_Data *)d;
    return pd->page.rotation;
}

static Eina_Bool
_etui_img_page_scale_set(void *d, float hscale, float vscale)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if ((pd->page.hscale == hscale) && (pd->page.vscale == vscale))
        return EINA_TRUE;

    pd->page.hscale = hscale;
    pd->page.vscale = vscale;

    return EINA_TRUE;
}

static void
_etui_img_page_scale_get(void *d, float *hscale, float *vscale)
{
    Etui_Provider_Data *pd;

    if (!d)
    {
        if (hscale) *hscale = 1.0f;
        if (vscale) *vscale = 1.0f;
        return;
    }

    pd = (Etui_Provider_Data *)d;

    if (hscale) *hscale = pd->page.hscale;
    if (vscale) *vscale = pd->page.vscale;
}

static void
_etui_img_page_render_pre(void *d)
{
    Etui_Provider_Data *pd;
    int width;
    int height;

    if (!d)
        return;

    DBG("render pre");

    pd = (Etui_Provider_Data *)d;
    DBG("render pre %d\n", pd->doc.cb_type);

    switch (pd->doc.cb_type)
    {
        case ETUI_IMG_CB_NONE:
            evas_object_image_size_get(pd->efl.obj, &width, &height);

            /* rotation first */
            switch (pd->page.rotation)
            {
                case ETUI_ROTATION_90:
                case ETUI_ROTATION_270:
                {
                    int tmp;

                    tmp = width;
                    width = height;
                    height = tmp;
                    break;
                }
                default:
                    break;
            }

            /* then scale */
            width = (int)(width * pd->page.hscale);
            height = (int)(height * pd->page.vscale);

            evas_object_image_size_set(pd->efl.obj, width, height);
            evas_object_image_fill_set(pd->efl.obj, 0, 0, width, height);
            pd->efl.m = evas_object_image_data_get(pd->efl.obj, 1);
            pd->page.width = width;
            pd->page.height = height;

            evas_object_resize(pd->efl.obj, width, height);
            break;
        case ETUI_IMG_CB_CBZ:
#ifdef HAVE_ZLIB
        {
            Etui_Img_Cbz_Data *cbz_data;
            void *data;
            int size;

            cbz_data = eina_array_data_get(&pd->doc.toc, pd->page.page_num);
            data = _etui_img_cbz_record_get(pd, cbz_data->file_offset, &size);
            printf(" page num : %d  data : %p size %d\n", pd->page.page_num, data, size);
            if (!data)
                return;

            evas_object_image_memfile_set(pd->efl.obj,
                                          data,
                                          size,
                                          NULL, NULL);
            if (evas_object_image_load_error_get(pd->efl.obj) == EVAS_LOAD_ERROR_NONE)
            {
                ERR("CBZ image format not supported");
                return;
            }

            evas_object_image_size_get(pd->efl.obj, &width, &height);
            printf(" * (%dx%d)\n", width, height);

            evas_object_image_size_set(pd->efl.obj, width, height);
            evas_object_image_fill_set(pd->efl.obj, 0, 0, width, height);
            pd->efl.m = evas_object_image_data_get(pd->efl.obj, 1);
            pd->page.width = width;
            pd->page.height = height;

            evas_object_resize(pd->efl.obj, width, height);
        }
#endif
            break;
        case ETUI_IMG_CB_CBR:
        {
            char buf[16384];
            struct RAROpenArchiveDataEx rar_archive_data;
            struct RARHeaderDataEx rar_header_data;
            HANDLE rar_archive;
            int rar_header_code;
            int rar_process_file_code;
            Etui_Img_Cbr_Data *data;
            unsigned int idx;

            memset(&rar_archive_data, 0, sizeof(rar_archive_data));
            rar_archive_data.ArcName = pd->doc.filename;
            rar_archive_data.CmtBuf = buf;
            rar_archive_data.CmtBufSize = sizeof(buf);
            rar_archive_data.OpenMode = RAR_OM_EXTRACT;
            rar_archive_data.Callback = _etui_img_cbr_cb;
            rar_archive_data.UserData = (LPARAM)pd;
            rar_archive = RAROpenArchiveEx(&rar_archive_data);

            rar_header_data.CmtBuf = NULL;
            memset(&rar_archive_data.Reserved, 0, sizeof(rar_archive_data.Reserved));

            idx = 0;
            data = eina_array_data_get(&pd->doc.toc, pd->page.page_num);
            while ((rar_header_code = RARReadHeaderEx(rar_archive, &rar_header_data)) == 0)
            {
                if (idx == data->idx)
                {
                  printf(" * idx : %d %llu\n", idx, data->file_size);
                    pd->page.rar.getting_data = 1;
                    if (!pd->page.rar.data)
                    {
                        pd->page.rar.size = data->file_size;
                        pd->page.rar.data = malloc(pd->page.rar.size);
                    }
                }
                else
                    pd->page.rar.getting_data = 0;

                rar_process_file_code = RARProcessFile(rar_archive, RAR_TEST,
                                                       NULL, NULL);
                if (rar_process_file_code != 0)
                {
                    RARCloseArchive(rar_archive);
                    return;
                }
                idx++;
            }

            RARCloseArchive(rar_archive);

            printf(" * %p %p  %llu %llu\n", pd->efl.obj, pd->page.rar.data, pd->page.rar.size, pd->page.rar.offset);
            evas_object_image_memfile_set(pd->efl.obj,
                                          pd->page.rar.data,
                                          pd->page.rar.size,
                                          "jpeg", NULL);
            printf(" * 10\n");
            if (evas_object_image_load_error_get(pd->efl.obj) == EVAS_LOAD_ERROR_NONE)
            {
                ERR("CBR image format not supported");
                return;
            }

            evas_object_image_size_get(pd->efl.obj, &width, &height);

            evas_object_image_size_set(pd->efl.obj, width, height);
            evas_object_image_fill_set(pd->efl.obj, 0, 0, width, height);
            pd->efl.m = evas_object_image_data_get(pd->efl.obj, 1);
            pd->page.width = width;
            pd->page.height = height;

            evas_object_resize(pd->efl.obj, width, height);

            break;
        }
        case ETUI_IMG_CB_CBA:
            break;
        case ETUI_IMG_CB_CB7:
            break;
        case ETUI_IMG_CB_CBT:
            break;
    }
}

static void
_etui_img_page_render(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    DBG("render");

    pd = (Etui_Provider_Data *)d;
}

static void
_etui_img_page_render_end(void *d)
{
    Etui_Provider_Data *pd;
    int width;
    int height;

    if (!d)
        return;

    DBG("render end");

    pd = (Etui_Provider_Data *)d;

    evas_object_image_size_get(pd->efl.obj, &width, &height);
    evas_object_image_data_set(pd->efl.obj, pd->efl.m);
    evas_object_image_data_update_add(pd->efl.obj, 0, 0, width, height);
}

static Etui_Provider_Descriptor _etui_provider_descriptor_img =
{
    /* .name                          */ "img",
    /* .version                       */ ETUI_PROVIDER_DESCRIPTOR_VERSION,
    /* .priority                      */ ETUI_PROVIDER_DESCRIPTOR_PRIORITY_DEFAULT,
    /* .init                          */ _etui_img_init,
    /* .shutdown                      */ _etui_img_shutdown,
    /* .evas_object_get               */ _etui_img_evas_object_get,
    /* .file_open                     */ _etui_img_file_open,
    /* .file_close                    */ _etui_img_file_close,
    /* .version_get                   */ _etui_img_version_get,
    /* .title_get                     */ _etui_img_title_get,
    /* .author_get                    */ NULL,
    /* .subject_get                   */ NULL,
    /* .keywords_get                  */ NULL,
    /* .creator_get                   */ NULL,
    /* .producer_get                  */ NULL,
    /* .creation_date_get             */ NULL,
    /* .modification_date_get         */ NULL,
    /* .is_printable                  */ NULL,
    /* .is_changeable                 */ NULL,
    /* .is_copyable                   */ NULL,
    /* .is_notable                    */ NULL,
    /* .password_needed               */ NULL,
    /* .password_set                  */ NULL,
    /* .pages_count                   */ _etui_img_pages_count,
    /* .toc_get                       */ _etui_img_toc_get,
    /* .page_use_display_list_set     */ NULL,
    /* .page_use_display_list_get     */ NULL,
    /* .page_set                      */ _etui_img_page_set,
    /* .page_get                      */ _etui_img_page_get,
    /* .page_size_get                 */ _etui_img_page_size_get,
    /* .page_rotation_set             */ _etui_img_page_rotation_set,
    /* .page_rotation_get             */ _etui_img_page_rotation_get,
    /* .page_scale_set                */ _etui_img_page_scale_set,
    /* .page_scale_get                */ _etui_img_page_scale_get,
    /* .page_dpi_set                  */ NULL,
    /* .page_dpi_get                  */ NULL,
    /* .page_links_get                */ NULL,
    /* .page_render_pre               */ _etui_img_page_render_pre,
    /* .page_render                   */ _etui_img_page_render,
    /* .page_render_end               */ _etui_img_page_render_end,
    /* .page_text_extract             */ NULL,
    /* .page_text_find                */ NULL,
    /* .page_duration_get             */ NULL,
    /* .page_transition_type_get      */ NULL,
    /* .page_transition_duration_get  */ NULL,
    /* .page_transition_vertical_get  */ NULL,
    /* .page_transition_outwards_get  */ NULL,
    /* .page_transition_direction_get */ NULL
};

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

Eina_Bool
etui_module_img_init(void)
{
    if (_etui_module_img_init_count > 0)
    {
        _etui_module_img_init_count++;
        return EINA_TRUE;
    }

    _etui_module_img_log_domain = eina_log_domain_register("etui-img",
                                                           ETUI_MODULE_IMG_DEFAULT_LOG_COLOR);
    if (_etui_module_img_log_domain < 0)
    {
        EINA_LOG_CRIT("Could not register log domain 'etui-img'");
        return EINA_FALSE;
    }

    if (!etui_module_register(&_etui_provider_descriptor_img))
    {
        ERR("Could not register module %p", &_etui_provider_descriptor_img);
        goto unregister_log;
    }

    _etui_module_img_init_count = 1;
    return EINA_TRUE;

  unregister_log:
    eina_log_domain_unregister(_etui_module_img_log_domain);
    _etui_module_img_log_domain = -1;

    return EINA_FALSE;
}

void
etui_module_img_shutdown(void)
{
    if (_etui_module_img_init_count > 1)
    {
        _etui_module_img_init_count--;
        return;
    }
    else if (_etui_module_img_init_count == 0)
    {
        ERR("Too many etui_module_img_shutdown() calls");
        return;
    }

    DBG("shutdown img module");

    _etui_module_img_init_count = 0;

    etui_module_unregister(&_etui_provider_descriptor_img);

    eina_log_domain_unregister(_etui_module_img_log_domain);
    _etui_module_img_log_domain = -1;
}

#ifndef ETUI_BUILD_STATIC_IMG

EINA_MODULE_INIT(etui_module_img_init);
EINA_MODULE_SHUTDOWN(etui_module_img_shutdown);

#endif

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
