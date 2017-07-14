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
#include <Ecore.h> /* for Ecore_Thread in Etui_Module */
#include <Evas.h>

#ifdef HAVE_LIBARCHIVE
# include <archive.h>
# include <archive_entry.h>
#endif

#include "Etui.h"
#include "etui_module.h"
#include "etui_file.h"
#include "etui_module_cb.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#ifdef ETUI_MODULE_CB_DEFAULT_LOG_COLOR
# undef ETUI_MODULE_CB_DEFAULT_LOG_COLOR
#endif
#define ETUI_MODULE_CB_DEFAULT_LOG_COLOR EINA_COLOR_LIGHTCYAN

#ifdef ERR
# undef ERR
#endif
#define ERR(...)  EINA_LOG_DOM_ERR(_etui_module_cb_log_domain, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...)  EINA_LOG_DOM_DBG(_etui_module_cb_log_domain, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...)  EINA_LOG_DOM_INFO(_etui_module_cb_log_domain, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...)  EINA_LOG_DOM_WARN(_etui_module_cb_log_domain, __VA_ARGS__)

#ifdef CRIT
# undef CRIT
#endif
#define CRIT(...) EINA_LOG_DOM_CRIT(_etui_module_cb_log_domain, __VA_ARGS__)

typedef struct
{
    /* specific EFL stuff for the module */

    struct {
        Evas_Object *obj;
        void *m;
    } efl;

    /* specific CB stuff for the module */

    /* Document */
    struct
    {
        char *filename;
        Etui_Module_Cb_Info *info; /* information specific to the document (creator, ...) */
        int page_nbr;
        Eina_Array toc;
        Etui_Cb_Type cb_type;
        Eina_File *file;
        const void *data;
        size_t size;
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
    } page;
} Etui_Module_Data;

static int _etui_module_cb_init_count = 0;
static int _etui_module_cb_log_domain = -1;

static int
_etui_cb_sort_cb(const void *d1, const void *d2)
{
    return strcasecmp((const char *)d1, (const char *)d2);
}

static Eina_Bool
_etui_cb_is_valid(Etui_Module_Data *md)
{
    Eina_Bool res = EINA_FALSE;
#ifdef HAVE_LIBARCHIVE
    struct archive *a;
    struct archive_entry *entry;
    char *file_name;
    Eina_List *list = NULL;
    Eina_List *l;
    int r;

    /* md is valid */

    /* cbz, cbr, cb7, cbt */

    a = archive_read_new();
    if (!a)
        return EINA_FALSE;

    if (archive_read_support_filter_all(a) != ARCHIVE_OK)
        goto free_archive;

    if (archive_read_support_format_zip(a) != ARCHIVE_OK)
        goto free_archive;

    if (archive_read_support_format_rar(a) != ARCHIVE_OK)
        goto free_archive;

    if (archive_read_support_format_7zip(a) != ARCHIVE_OK)
        goto free_archive;

    if (archive_read_support_format_tar(a) != ARCHIVE_OK)
        goto free_archive;

    r = archive_read_open_memory(a, (void *)md->doc.data, md->doc.size);
    if (r != ARCHIVE_OK)
        goto free_archive;

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {
        if (archive_format(a) == ARCHIVE_FORMAT_ZIP)
            md->doc.cb_type = ETUI_CB_CBZ;
        else if (archive_format(a) == ARCHIVE_FORMAT_RAR)
            md->doc.cb_type = ETUI_CB_CBR;
        else if (archive_format(a) == ARCHIVE_FORMAT_7ZIP)
            md->doc.cb_type = ETUI_CB_CB7;
        else if ((archive_format(a) & ARCHIVE_FORMAT_TAR) == ARCHIVE_FORMAT_TAR)
            md->doc.cb_type = ETUI_CB_CBT;
        else
            goto skip;

        if (archive_entry_filetype(entry) == AE_IFREG)
        {
            file_name = strdup(archive_entry_pathname(entry));
            if (file_name)
                list = eina_list_append(list, file_name);
        }

      skip:
        archive_read_data_skip(a);
    }

    archive_read_free(a);

    eina_array_step_set(&md->doc.toc, sizeof(Eina_Array), 4);
    list = eina_list_sort(list, eina_list_count(list), _etui_cb_sort_cb);
    EINA_LIST_FOREACH(list, l, file_name)
        eina_array_push(&md->doc.toc, file_name);
    eina_list_free(list);

    return EINA_TRUE;

  free_archive:
    archive_read_free(a);
#endif /* HAVE_LIBARCHIVE */

    /* cba */

    INF("ACE Comic Books not supported yet.");

    return res;
}

/* Virtual functions */

static void *
_etui_cb_init(const Etui_File *ef)
{
    Etui_Module_Data *md;

    md = (Etui_Module_Data *)calloc(1, sizeof(Etui_Module_Data));
    if (!md)
        return NULL;

    DBG("init module");

    md->doc.data = etui_file_base_get(ef);
    md->doc.size = etui_file_size_get(ef);

    if (!_etui_cb_is_valid(md))
    {
        ERR("Can not open Comic Book archive %s", md->doc.filename);
        goto free_md;
    }

    md->doc.info = (Etui_Module_Cb_Info *)calloc(1, sizeof(Etui_Module_Cb_Info));
    if (!md->doc.info)
    {
        ERR("Could not allocate memory for information structure");
        goto free_md;
    }

    md->doc.page_nbr = eina_array_count(&md->doc.toc);
    md->page.page_num = -1;
    md->page.rotation = ETUI_ROTATION_0;
    md->page.hscale = 1.0f;
    md->page.vscale = 1.0f;

    return md;

  free_md:
    free(md);

    return NULL;
}

static void
_etui_cb_shutdown(void *d)
{
    Etui_Module_Data *md;

    DBG("shutdown module");

    if (!d)
        return;

    md = (Etui_Module_Data *)d;

    switch (md->doc.cb_type)
    {
        case ETUI_CB_CBZ:
        case ETUI_CB_CBR:
        case ETUI_CB_CB7:
        case ETUI_CB_CBT:
        {
            char *file_name;
            Eina_Array_Iterator iterator;
            unsigned int i;

            EINA_ARRAY_ITER_NEXT(&md->doc.toc, i, file_name, iterator)
            {
                free(file_name);
            }
            eina_array_flush(&md->doc.toc);
            break;
        }
        case ETUI_CB_CBA:
            break;
        default:
            break;
    }

    free(md->doc.info);
    free(md);
}

static Evas_Object *
_etui_cb_evas_object_add(void *d, Evas *evas)
{
    if (!d)
        return NULL;

    ((Etui_Module_Data *)d)->efl.obj = evas_object_image_add(evas);
    return ((Etui_Module_Data *)d)->efl.obj;
}

static void
_etui_cb_evas_object_del(void *d)
{
    if (!d)
        return;

    evas_object_del(((Etui_Module_Data *)d)->efl.obj);
}

static const void *
_etui_cb_info_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;
    md->doc.info->type = md->doc.cb_type;

    return md->doc.info;
}

static const char *
_etui_cb_title_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;

    return md->doc.filename;
}

static int
_etui_cb_pages_count(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return -1;

    md = (Etui_Module_Data *)d;

    return md->doc.page_nbr;
}

static const Eina_Array *
_etui_cb_toc_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;

    return &md->doc.toc;
}

static Eina_Bool
_etui_cb_page_set(void *d, int page_num)
{
    Etui_Module_Data *md;

    if (!d)
        return EINA_FALSE;

    md = (Etui_Module_Data *)d;

    if ((page_num < 0) || (page_num >= md->doc.page_nbr))
        return EINA_FALSE;

    if (page_num == md->page.page_num)
        return EINA_FALSE;

    md->page.page_num = page_num;
    md->page.rotation = ETUI_ROTATION_0;
    md->page.hscale = 1.0f;
    md->page.vscale = 1.0f;

    return EINA_TRUE;
}

static int
_etui_cb_page_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return -1;

    md = (Etui_Module_Data *)d;

    return md->page.page_num;
}

static void
_etui_cb_page_size_get(void *d, int *width, int *height)
{
    Etui_Module_Data *md;

    if (!d)
        goto _err;

    md = (Etui_Module_Data *)d;

    evas_object_image_size_get(md->efl.obj, width, height);

    return;

  _err:
    if (width) *width = 0;
    if (height) *height = 0;
}

static Eina_Bool
_etui_cb_page_rotation_set(void *d, Etui_Rotation rotation)
{
    Etui_Module_Data *md;

    if (!d)
        return EINA_FALSE;

    md = (Etui_Module_Data *)d;

    if (md->page.rotation == rotation)
        return EINA_TRUE;

    md->page.rotation = rotation;

    return EINA_TRUE;
}

static Etui_Rotation
_etui_cb_page_rotation_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return ETUI_ROTATION_0;

    md = (Etui_Module_Data *)d;
    return md->page.rotation;
}

static Eina_Bool
_etui_cb_page_scale_set(void *d, float hscale, float vscale)
{
    Etui_Module_Data *md;

    if (!d)
        return EINA_FALSE;

    md = (Etui_Module_Data *)d;

    if ((md->page.hscale == hscale) && (md->page.vscale == vscale))
        return EINA_TRUE;

    md->page.hscale = hscale;
    md->page.vscale = vscale;

    return EINA_TRUE;
}

static void
_etui_cb_page_scale_get(void *d, float *hscale, float *vscale)
{
    Etui_Module_Data *md;

    if (!d)
    {
        if (hscale) *hscale = 1.0f;
        if (vscale) *vscale = 1.0f;
        return;
    }

    md = (Etui_Module_Data *)d;

    if (hscale) *hscale = md->page.hscale;
    if (vscale) *vscale = md->page.vscale;
}

static void
_etui_cb_page_render_pre(void *d)
{
    Etui_Module_Data *md;
    int width;
    int height;

    if (!d)
        return;

    DBG("render pre");

    md = (Etui_Module_Data *)d;
    DBG("render pre %d", md->doc.cb_type);

    switch (md->doc.cb_type)
    {
        case ETUI_CB_CBZ:
        case ETUI_CB_CBR:
        case ETUI_CB_CB7:
        case ETUI_CB_CBT:
        {
#ifdef HAVE_LIBARCHIVE
            struct archive *a;
            struct archive_entry *entry;
            char *file_name;
            void *data = NULL;
            size_t size = 0;
            int r;

            a = archive_read_new();
            if (!a)
                break;

            if (archive_read_support_filter_all(a) != ARCHIVE_OK)
                goto free_archive;

            if (archive_read_support_format_zip(a) != ARCHIVE_OK)
                goto free_archive;

            if (archive_read_support_format_rar(a) != ARCHIVE_OK)
                goto free_archive;

            if (archive_read_support_format_7zip(a) != ARCHIVE_OK)
                goto free_archive;

            if (archive_read_support_format_tar(a) != ARCHIVE_OK)
                goto free_archive;

            r = archive_read_open_memory(a, (void *)md->doc.data, md->doc.size);
            if (r != ARCHIVE_OK)
                goto free_archive;

            file_name = eina_array_data_get(&md->doc.toc, md->page.page_num);

            while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
            {
                if (archive_entry_filetype(entry) == AE_IFREG)
                {
                    if (strcasecmp(file_name, archive_entry_pathname(entry)) == 0)
                    {
                        ssize_t res;

                        size = archive_entry_size(entry);
                        data = malloc(size);
                        if (!data)
                            goto free_archive;

                        res = archive_read_data(a, data, size);
                        if ((res != ARCHIVE_OK) && (res != ARCHIVE_WARN))
                            break;
                    }
                }

                archive_read_data_skip(a);
            }

            archive_read_free(a);

            if (!data)
                break;
            evas_object_image_memfile_set(md->efl.obj,
                                          data,
                                          size,
                                          NULL, NULL);
            if (evas_object_image_load_error_get(md->efl.obj) != EVAS_LOAD_ERROR_NONE)
            {
                ERR("Comic Book image format not supported");
                return;
            }

            evas_object_image_size_get(md->efl.obj, &width, &height);

            evas_object_image_size_set(md->efl.obj, width, height);
            evas_object_image_fill_set(md->efl.obj, 0, 0, width, height);
            md->efl.m = evas_object_image_data_get(md->efl.obj, 1);
            md->page.width = width;
            md->page.height = height;

            evas_object_resize(md->efl.obj, width, height);
            break;

          free_archive:
            archive_read_free(a);
#endif
            break;
        }
        case ETUI_CB_CBA:
            break;
    }
}

static void
_etui_cb_page_render(void *d)
{
    if (!d)
        return;

    DBG("render");
}

static void
_etui_cb_page_render_end(void *d)
{
    Etui_Module_Data *md;
    int width;
    int height;

    if (!d)
        return;

    DBG("render end");

    md = (Etui_Module_Data *)d;

    evas_object_image_size_get(md->efl.obj, &width, &height);
    evas_object_image_data_set(md->efl.obj, md->efl.m);
    evas_object_image_data_update_add(md->efl.obj, 0, 0, width, height);
}


static Etui_Module_Func _etui_module_func_cb =
{
    /* .init              */ _etui_cb_init,
    /* .shutdown          */ _etui_cb_shutdown,
    /* .evas_object_add   */ _etui_cb_evas_object_add,
    /* .evas_object_del   */ _etui_cb_evas_object_del,
    /* .info_get          */ _etui_cb_info_get,
    /* .title_get         */ _etui_cb_title_get,
    /* .pages_count       */ _etui_cb_pages_count,
    /* .toc_get           */ _etui_cb_toc_get,
    /* .page_set          */ _etui_cb_page_set,
    /* .page_get          */ _etui_cb_page_get,
    /* .page_size_get     */ _etui_cb_page_size_get,
    /* .page_rotation_set */ _etui_cb_page_rotation_set,
    /* .page_rotation_get */ _etui_cb_page_rotation_get,
    /* .page_scale_set    */ _etui_cb_page_scale_set,
    /* .page_scale_get    */ _etui_cb_page_scale_get,
    /* .page_render_pre   */ _etui_cb_page_render_pre,
    /* .page_render       */ _etui_cb_page_render,
    /* .page_render_end   */ _etui_cb_page_render_end,
    /* .api_get           */ NULL
};

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/**** module API access ****/

static Eina_Bool
module_open(Etui_Module *em)
{
    if (_etui_module_cb_init_count > 0)
    {
        _etui_module_cb_init_count++;
        return EINA_TRUE;
    }

    if (!em)
        return EINA_FALSE;

    _etui_module_cb_log_domain = eina_log_domain_register("etui-cb",
                                                          ETUI_MODULE_CB_DEFAULT_LOG_COLOR);
    if (_etui_module_cb_log_domain < 0)
    {
        EINA_LOG_ERR("Can not create a module log domain.");
        return EINA_FALSE;
    }

    /* inititialize external libraries here */

    em->functions = (void *)(&_etui_module_func_cb);

    _etui_module_cb_init_count = 1;
    return EINA_TRUE;
}

static void
module_close(Etui_Module *em)
{
    if (_etui_module_cb_init_count > 1)
    {
        _etui_module_cb_init_count--;
        return;
    }
    else if (_etui_module_cb_init_count == 0)
    {
        ERR("Too many etui_module_cb_shutdown() calls");
        return;
    }

    DBG("shutdown cb module");

    /* shutdown module here */
    em->functions->shutdown(em->data);

    /* shutdown external libraries here */

    /* shutdown EFL here */

    eina_log_domain_unregister(_etui_module_cb_log_domain);
    _etui_module_cb_log_domain = -1;
    _etui_module_cb_init_count = 0;
}

static Etui_Module_Api _etui_modapi =
{
    "cb",
    {
        module_open,
        module_close
    }
};

ETUI_MODULE_DEFINE(cb)

#ifndef ETUI_STATIC_BUILD_CB
ETUI_EINA_MODULE_DEFINE(cb);
#endif

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
