/* Etui - Multi-document rendering library using the EFL
 * Copyright (C) 2013-2017 Vincent Torri
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

#include <libdjvu/ddjvuapi.h>

#include "Etui.h"
#include "etui_module.h"
#include "etui_file.h"
#include "etui_module_djvu.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#ifdef ETUI_MODULE_DJVU_DEFAULT_LOG_COLOR
# undef ETUI_MODULE_DJVU_DEFAULT_LOG_COLOR
#endif
#define ETUI_MODULE_DJVU_DEFAULT_LOG_COLOR EINA_COLOR_LIGHTCYAN

#ifdef ERR
# undef ERR
#endif
#define ERR(...)  EINA_LOG_DOM_ERR(_etui_module_djvu_log_domain, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...)  EINA_LOG_DOM_DBG(_etui_module_djvu_log_domain, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...)  EINA_LOG_DOM_INFO(_etui_module_djvu_log_domain, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...)  EINA_LOG_DOM_WARN(_etui_module_djvu_log_domain, __VA_ARGS__)

#ifdef CRIT
# undef CRIT
#endif
#define CRIT(...) EINA_LOG_DOM_CRIT(_etui_module_djvu_log_domain, __VA_ARGS__)

typedef struct
{
    /* specific EFL stuff for the module */

    struct {
        Evas_Object *obj;
        void *m;
    } efl;

    /* specific DJVU stuff for the module */

    /* Document */
    struct
    {
        Etui_Module_Djvu_Info *info; /* information specific to the document (creator, ...) */
        int page_nbr;
        ddjvu_context_t *ctx;
        ddjvu_document_t *doc;
    } doc;

    /* Current page */
    struct
    {
        ddjvu_page_t *page;
        int width;
        int height;
        int page_num;
        Etui_Rotation rotation;
        float hscale;
        float vscale;
        /* information values */
        int dpi;
        double gamma;
        Etui_Djvu_Page_Type type;
    } page;
} Etui_Module_Data;

static int _etui_module_djvu_init_count = 0;
static int _etui_module_djvu_log_domain = -1;

static void
_etui_djvu_messages_cb(ddjvu_context_t *ctx, int wait)
{
    const ddjvu_message_t *msg;

    if (wait)
        msg = ddjvu_message_wait(ctx);

    while ((msg = ddjvu_message_peek(ctx)))
    {
        switch(msg->m_any.tag)
        {
            case DDJVU_ERROR:
                ERR("Can not decode: %s\n", msg->m_error.message);
                if (msg->m_error.filename)
                    ERR("'%s:%d'\n",
                           msg->m_error.filename,
                           msg->m_error.lineno);
                break;
            default:
                break;
        }
        ddjvu_message_pop(ctx);
    }
}

/* Virtual functions */

static void *
_etui_djvu_init(const Etui_File *ef)
{
    Etui_Module_Data *md;

    md = (Etui_Module_Data *)calloc(1, sizeof(Etui_Module_Data));
    if (!md)
        return NULL;

    DBG("init module");

    md->doc.ctx = ddjvu_context_create("etui");
    if (!md->doc.ctx)
    {
        ERR("Could not create context");
        goto free_md;
    }

    md->doc.doc = ddjvu_document_create_by_filename(md->doc.ctx,
                                                    etui_file_filename_get(ef),
                                                    EINA_TRUE);
    if (!md->doc.doc)
    {
        ERR("Could not open document %s", etui_file_filename_get(ef));
        goto release_context;
    }

    while (!ddjvu_document_decoding_done(md->doc.doc))
        _etui_djvu_messages_cb(md->doc.ctx, EINA_TRUE);

    if (ddjvu_document_decoding_error(md->doc.doc))
    {
        ERR("Could not decode document");
        goto release_document;
    }

    md->doc.info = (Etui_Module_Djvu_Info *)calloc(1, sizeof(Etui_Module_Djvu_Info));
    if (!md->doc.info)
    {
        ERR("Could not allocate memory for information structure");
        goto release_document;
    }

    md->doc.page_nbr = ddjvu_document_get_pagenum(md->doc.doc);
    md->page.page_num = -1;
    md->page.rotation = ETUI_ROTATION_0;
    md->page.hscale = 1.0f;
    md->page.vscale = 1.0f;

    printf("%d %d\n", ddjvu_document_get_pagenum(md->doc.doc), ddjvu_document_get_filenum(md->doc.doc));

    {
        ddjvu_fileinfo_t info;
        int n = ddjvu_document_get_filenum(md->doc.doc);
        int i;

        for (i = 0; i < n; i++)
        {
            ddjvu_document_get_fileinfo(md->doc.doc, i, &info);
            printf("%c %d %d %s %s %s\n",
                   info.type, info.pageno, info.size, info.id, info.name, info.title);
        }
    }

    return md;

  release_document:
    ddjvu_document_release(md->doc.doc);
  release_context:
    ddjvu_context_release(md->doc.ctx);
  free_md:
    free(md);

    return NULL;
}

static void
_etui_djvu_shutdown(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return;

    DBG("shutdown module");

    md = (Etui_Module_Data *)d;

    if (md->page.page)
        ddjvu_page_release(md->page.page);
    free(md->doc.info);
    ddjvu_document_release(md->doc.doc);
    ddjvu_context_release(md->doc.ctx);
    free(md);
}

static Evas_Object *
_etui_djvu_evas_object_add(void *d, Evas *evas)
{
    if (!d)
        return NULL;

    ((Etui_Module_Data *)d)->efl.obj = evas_object_image_add(evas);
    return ((Etui_Module_Data *)d)->efl.obj;
}

static void
_etui_djvu_evas_object_del(void *d)
{
    if (!d)
        return;

    evas_object_del(((Etui_Module_Data *)d)->efl.obj);
}

static const void *
_etui_djvu_info_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;
    md->doc.info->page_dpi = md->page.dpi;
    md->doc.info->page_gamma = md->page.gamma;
    md->doc.info->page_type = md->page.type;

    return md->doc.info;
}

static const char *
_etui_djvu_title_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;

    if (!md->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return NULL;
}

static int
_etui_djvu_pages_count(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return -1;

    md = (Etui_Module_Data *)d;

    if (!md->doc.doc)
    {
        ERR("no opened document");
        return -1;
    }

    return ddjvu_document_get_pagenum(md->doc.doc);
}

static Eina_Bool
_etui_djvu_page_set(void *d, int page_num)
{
    Etui_Module_Data *md;
    ddjvu_page_t *page;

    if (!d)
        return EINA_FALSE;

    md = (Etui_Module_Data *)d;

    if (!md->doc.doc)
    {
        ERR("no opened document");
        return EINA_FALSE;
    }

    if (page_num < 0)
        return EINA_FALSE;

    if (page_num >= ddjvu_document_get_pagenum(md->doc.doc))
        return EINA_FALSE;

    page = ddjvu_page_create_by_pageno(md->doc.doc, page_num);
    if (!page)
    {
        ERR("could not set page %d from the document", page_num);
        return EINA_FALSE;
    }

    while (!ddjvu_page_decoding_done(page))
        _etui_djvu_messages_cb(md->doc.ctx, EINA_TRUE);
    if (ddjvu_page_decoding_error(page))
    {
        _etui_djvu_messages_cb(md->doc.ctx, EINA_FALSE);
        ERR("Could not decode page %d\n", page_num);
        /* FIXME: corrupted file */
        ddjvu_page_release(page);
        return EINA_FALSE;
    }

    if (md->page.page)
        ddjvu_page_release(md->page.page);

    md->page.width = 0;
    md->page.height = 0;

    md->page.page = page;
    md->page.page_num = page_num;
    md->page.rotation = ETUI_ROTATION_0;
    md->page.hscale = 1.0f;
    md->page.vscale = 1.0f;

    md->page.dpi = 72;
    md->page.gamma = 2.2;
    md->page.type = 0;

    return EINA_TRUE;
}

static int
_etui_djvu_page_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return -1;

    md = (Etui_Module_Data *)d;

    return md->page.page_num;
}

static void
_etui_djvu_page_size_get(void *d, int *width, int *height)
{
    Etui_Module_Data *md;

    if (!d)
        goto _err;

    md = (Etui_Module_Data *)d;

    if (!md->doc.doc)
    {
        ERR("no opened document");
        goto _err;
    }

    {
        if (width) *width = md->page.width;
        if (height) *height = md->page.width;
    }

    return;

  _err:
    if (width) *width = 0;
    if (height) *height = 0;
}

static Eina_Bool
_etui_djvu_page_rotation_set(void *d, Etui_Rotation rotation)
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
_etui_djvu_page_rotation_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return ETUI_ROTATION_0;

    md = (Etui_Module_Data *)d;
    return md->page.rotation;
}

static Eina_Bool
_etui_djvu_page_scale_set(void *d, float hscale, float vscale)
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
_etui_djvu_page_scale_get(void *d, float *hscale, float *vscale)
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
_etui_djvu_page_render_pre(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return;

    DBG("render pre");

    md = (Etui_Module_Data *)d;

    if (!md->doc.doc)
    {
        ERR("no opened document");
        return;
    }

    md->page.width = ddjvu_page_get_width(md->page.page);
    md->page.height = ddjvu_page_get_height(md->page.page);
    md->page.dpi = ddjvu_page_get_resolution(md->page.page);
    md->page.gamma = ddjvu_page_get_gamma(md->page.page);
    md->page.type = ddjvu_page_get_type(md->page.page);

    evas_object_image_size_set(md->efl.obj, md->page.width, md->page.height);
    evas_object_image_fill_set(md->efl.obj, 0, 0, md->page.width, md->page.height);
    md->efl.m = evas_object_image_data_get(md->efl.obj, 1);
    evas_object_resize(md->efl.obj, md->page.width, md->page.height);
}

static void
_etui_djvu_page_render(void *d)
{
    unsigned int masks[4] = { 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };
    ddjvu_rect_t prect;
    ddjvu_rect_t rrect;
    Etui_Module_Data *md;
    ddjvu_format_t *format;
    int stride;

    if (!d)
        return;

    DBG("render");

    md = (Etui_Module_Data *)d;

    prect.x = 0;
    prect.y = 0;
    prect.w = ddjvu_page_get_width(md->page.page);
    prect.h = ddjvu_page_get_height(md->page.page);
    rrect = prect;
    format = ddjvu_format_create(DDJVU_FORMAT_RGBMASK32, 4, masks);
    ddjvu_format_set_row_order(format, 1);
    stride = ddjvu_page_get_width(md->page.page) * 4;

    if (!ddjvu_page_render(md->page.page, DDJVU_RENDER_COLOR, &prect, &rrect, format, stride, md->efl.m))
    {
        ERR("could not render page");
        ddjvu_format_release(format);
    }
}

static void
_etui_djvu_page_render_end(void *d)
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


static Etui_Module_Func _etui_module_func_djvu =
{
    /* .init              */ _etui_djvu_init,
    /* .shutdown          */ _etui_djvu_shutdown,
    /* .evas_object_add   */ _etui_djvu_evas_object_add,
    /* .evas_object_del   */ _etui_djvu_evas_object_del,
    /* .info_get          */ _etui_djvu_info_get,
    /* .title_get         */ _etui_djvu_title_get,
    /* .pages_count       */ _etui_djvu_pages_count,
    /* .toc_get           */ NULL,
    /* .page_set          */ _etui_djvu_page_set,
    /* .page_get          */ _etui_djvu_page_get,
    /* .page_size_get     */ _etui_djvu_page_size_get,
    /* .page_rotation_set */ _etui_djvu_page_rotation_set,
    /* .page_rotation_get */ _etui_djvu_page_rotation_get,
    /* .page_scale_set    */ _etui_djvu_page_scale_set,
    /* .page_scale_get    */ _etui_djvu_page_scale_get,
    /* .page_render_pre   */ _etui_djvu_page_render_pre,
    /* .page_render       */ _etui_djvu_page_render,
    /* .page_render_end   */ _etui_djvu_page_render_end,
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
    if (_etui_module_djvu_init_count > 0)
    {
        _etui_module_djvu_init_count++;
        return EINA_TRUE;
    }

    if (!em)
        return EINA_FALSE;

    _etui_module_djvu_log_domain = eina_log_domain_register("etui-djvu",
                                                           ETUI_MODULE_DJVU_DEFAULT_LOG_COLOR);
    if (_etui_module_djvu_log_domain < 0)
    {
        EINA_LOG_ERR("Can not create a module log domain.");
        return EINA_FALSE;
    }

    /* inititialize external libraries here */

    em->functions = (void *)(&_etui_module_func_djvu);

    _etui_module_djvu_init_count = 1;
    return EINA_TRUE;
}

static void
module_close(Etui_Module *em)
{
    if (_etui_module_djvu_init_count > 1)
    {
        _etui_module_djvu_init_count--;
        return;
    }
    else if (_etui_module_djvu_init_count == 0)
    {
        ERR("Too many etui_module_djvu_shutdown() calls");
        return;
    }

    DBG("shutdown djvu module");

    /* shutdown module here */
    em->functions->shutdown(em->data);

    /* shutdown external libraries here */

    /* shutdown EFL here */

    eina_log_domain_unregister(_etui_module_djvu_log_domain);
    _etui_module_djvu_log_domain = -1;
    _etui_module_djvu_init_count = 0;
}

static Etui_Module_Api _etui_modapi =
{
    "djvu",
    {
        module_open,
        module_close
    }
};

ETUI_MODULE_DEFINE(djvu)

#ifndef ETUI_STATIC_BUILD_DJVU
ETUI_EINA_MODULE_DEFINE(djvu);
#endif

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
