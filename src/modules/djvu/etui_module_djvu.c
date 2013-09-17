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

#include <libdjvu/ddjvuapi.h>

#include "Etui.h"
#include "etui_module.h"
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

typedef struct _Etui_Provider_Data Etui_Provider_Data;

struct _Etui_Provider_Data
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
        char *filename;
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
    } page;
};

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
_etui_djvu_init(Evas *evas)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)calloc(1, sizeof(Etui_Provider_Data));
    if (!pd)
        return NULL;

    DBG("init module");

    pd->efl.obj = evas_object_image_add(evas);
    if (!pd->efl.obj)
        goto free_pd;

    pd->doc.ctx = ddjvu_context_create("etui");
    if (!pd->doc.ctx)
    {
        ERR("Could not create context");
        goto del_obj;
    }

    return pd;

  del_obj:
    evas_object_del(pd->efl.obj);
  free_pd:
    free(pd);

    return NULL;
}

static void
_etui_djvu_shutdown(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    DBG("shutdown module");

    pd = (Etui_Provider_Data *)d;

    if (pd->doc.filename)
        free(pd->doc.filename);
    if (pd->doc.ctx)
        ddjvu_context_release(pd->doc.ctx);
    evas_object_del(pd->efl.obj);
    free(pd);
}

static Evas_Object *
_etui_djvu_evas_object_get(void *d)
{
    if (!d)
        return NULL;

    return ((Etui_Provider_Data *)d)->efl.obj;
}

static Eina_Bool
_etui_djvu_file_open(void *d, const char *filename)
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

    pd->doc.doc = ddjvu_document_create_by_filename(pd->doc.ctx, filename, TRUE);
    if (!pd->doc.doc)
    {
        ERR("Could not open document %s", filename);
        goto free_filename;
    }

    while (!ddjvu_document_decoding_done(pd->doc.doc))
        _etui_djvu_messages_cb(pd->doc.ctx, TRUE);

    if (ddjvu_document_decoding_error(pd->doc.doc))
    {
        ERR("Could not decode document");
        goto release_document;
    }

    pd->page.page_num = -1;

    return EINA_TRUE;

  release_document:
    ddjvu_document_release(pd->doc.doc);
    pd->doc.doc = NULL;
  free_filename:
    free(pd->doc.filename);
    pd->doc.filename = NULL;

  return EINA_FALSE;
}

static void
_etui_djvu_file_close(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    pd = (Etui_Provider_Data *)d;

    DBG("close file %s", pd->doc.filename);

    if (pd->page.page)
        ddjvu_page_release(pd->page.page);
    if (pd->doc.doc)
        ddjvu_document_release(pd->doc.doc);
    pd->doc.doc = NULL;
    if (pd->doc.filename)
        free(pd->doc.filename);
    pd->doc.filename = NULL;
}

static void
_etui_djvu_version_get(void *d EINA_UNUSED, int *maj, int *min)
{
    if (maj) *maj = -1;
    if (min) *min = -1;
}

static char *
_etui_djvu_title_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return NULL;
}

static char *
_etui_djvu_creator_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return NULL;
}

static char *
_etui_djvu_creation_date_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return NULL;
}

static int
_etui_djvu_pages_count(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return -1;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return -1;
    }

    return ddjvu_document_get_pagenum(pd->doc.doc);
}

static Eina_Bool
_etui_djvu_page_set(void *d, int page_num)
{
    Etui_Provider_Data *pd;
    ddjvu_page_t *page;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return EINA_FALSE;
    }

    if (page_num < 0)
        return EINA_FALSE;

    if (page_num >= ddjvu_document_get_pagenum(pd->doc.doc))
        return EINA_FALSE;

    page = ddjvu_page_create_by_pageno(pd->doc.doc, page_num);
    if (!page)
    {
        ERR("could not set page %d from the document", page_num);
        return EINA_FALSE;
    }

    while (!ddjvu_page_decoding_done(page))
        _etui_djvu_messages_cb(pd->doc.ctx, TRUE);
    if (ddjvu_page_decoding_error(page))
    {
        _etui_djvu_messages_cb(pd->doc.ctx, FALSE);
        ERR("Could not decode page %d\n", page_num);
        /* FIXME: corrupted file */
        ddjvu_page_release(page);
        return EINA_FALSE;
    }

    if (pd->page.page)
        ddjvu_page_release(pd->page.page);

    pd->page.width = 0;
    pd->page.height = 0;

    pd->page.page = page;
    pd->page.page_num = page_num;
    pd->page.rotation = ETUI_ROTATION_0;
    pd->page.hscale = 1.0f;
    pd->page.vscale = 1.0f;

    return EINA_TRUE;
}

static int
_etui_djvu_page_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return -1;

    pd = (Etui_Provider_Data *)d;

    return pd->page.page_num;
}

static void
_etui_djvu_page_size_get(void *d, int *width, int *height)
{
    Etui_Provider_Data *pd;

    if (!d)
        goto _err;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        goto _err;
    }

    {
        if (width) *width = pd->page.width;
        if (height) *height = pd->page.width;
    }

    return;

  _err:
    if (width) *width = 0;
    if (height) *height = 0;
}

static Eina_Bool
_etui_djvu_page_rotation_set(void *d, Etui_Rotation rotation)
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
_etui_djvu_page_rotation_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return ETUI_ROTATION_0;

    pd = (Etui_Provider_Data *)d;
    return pd->page.rotation;
}

static Eina_Bool
_etui_djvu_page_scale_set(void *d, float hscale, float vscale)
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
_etui_djvu_page_scale_get(void *d, float *hscale, float *vscale)
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

static Eina_Bool
_etui_djvu_page_dpi_set(void *d EINA_UNUSED, float hdpi EINA_UNUSED, float vdpi EINA_UNUSED)
{
    return EINA_TRUE;
}

static void
_etui_djvu_page_dpi_get(void *d, float *hdpi, float *vdpi)
{
    Etui_Provider_Data *pd;

    if (!d)
    {
        if (hdpi) *hdpi = 72.0f;
        if (vdpi) *vdpi = 72.0f;
        return;
    }

    pd = (Etui_Provider_Data *)d;

    if (hdpi) *hdpi = ddjvu_page_get_resolution(pd->page.page);
    if (vdpi) *vdpi = ddjvu_page_get_resolution(pd->page.page);
}

static void
_etui_djvu_page_render_pre(void *d)
{
    Etui_Provider_Data *pd;
    int width;
    int height;

    if (!d)
        return;

    DBG("render pre");

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return;
    }

    width = ddjvu_page_get_width(pd->page.page);
    height = ddjvu_page_get_height(pd->page.page);

    evas_object_image_size_set(pd->efl.obj, width, height);
    evas_object_image_fill_set(pd->efl.obj, 0, 0, width, height);
    pd->efl.m = evas_object_image_data_get(pd->efl.obj, 1);
    pd->page.width = width;
    pd->page.height = height;

    evas_object_resize(pd->efl.obj, width, height);
}

static void
_etui_djvu_page_render(void *d)
{
    unsigned int masks[4] = { 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };
    ddjvu_rect_t prect;
    ddjvu_rect_t rrect;
    Etui_Provider_Data *pd;
    ddjvu_format_t *format;
    int stride;

    if (!d)
        return;

    DBG("render");

    pd = (Etui_Provider_Data *)d;

    prect.x = 0;
    prect.y = 0;
    prect.w = ddjvu_page_get_width(pd->page.page);
    prect.h = ddjvu_page_get_height(pd->page.page);
    rrect = prect;
    format = ddjvu_format_create(DDJVU_FORMAT_RGBMASK32, 4, masks);
    ddjvu_format_set_row_order(format, 1);
    stride = ddjvu_page_get_width(pd->page.page) * 4;

    if (!ddjvu_page_render(pd->page.page, DDJVU_RENDER_COLOR, &prect, &rrect, format, stride, pd->efl.m))
    {
        ERR("could not render page");
        ddjvu_format_release(format);
    }
}

static void
_etui_djvu_page_render_end(void *d)
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


static Etui_Provider_Descriptor _etui_provider_descriptor_djvu =
{
    /* .name                          */ "djvu",
    /* .version                       */ ETUI_PROVIDER_DESCRIPTOR_VERSION,
    /* .priority                      */ ETUI_PROVIDER_DESCRIPTOR_PRIORITY_LOW,
    /* .init                          */ _etui_djvu_init,
    /* .shutdown                      */ _etui_djvu_shutdown,
    /* .evas_object_get               */ _etui_djvu_evas_object_get,
    /* .file_open                     */ _etui_djvu_file_open,
    /* .file_close                    */ _etui_djvu_file_close,
    /* .version_get                   */ _etui_djvu_version_get,
    /* .title_get                     */ _etui_djvu_title_get,
    /* .author_get                    */ NULL,
    /* .subject_get                   */ NULL,
    /* .keywords_get                  */ NULL,
    /* .creator_get                   */ _etui_djvu_creator_get,
    /* .producer_get                  */ NULL,
    /* .creation_date_get             */ _etui_djvu_creation_date_get,
    /* .modification_date_get         */ NULL,
    /* .is_printable                  */ NULL,
    /* .is_changeable                 */ NULL,
    /* .is_copyable                   */ NULL,
    /* .is_notable                    */ NULL,
    /* .password_needed               */ NULL,
    /* .password_set                  */ NULL,
    /* .pages_count                   */ _etui_djvu_pages_count,
    /* .toc_get                       */ NULL,
    /* .page_use_display_list_set     */ NULL,
    /* .page_use_display_list_get     */ NULL,
    /* .page_set                      */ _etui_djvu_page_set,
    /* .page_get                      */ _etui_djvu_page_get,
    /* .page_size_get                 */ _etui_djvu_page_size_get,
    /* .page_rotation_set             */ _etui_djvu_page_rotation_set,
    /* .page_rotation_get             */ _etui_djvu_page_rotation_get,
    /* .page_scale_set                */ _etui_djvu_page_scale_set,
    /* .page_scale_get                */ _etui_djvu_page_scale_get,
    /* .page_dpi_set                  */ _etui_djvu_page_dpi_set,
    /* .page_dpi_get                  */ _etui_djvu_page_dpi_get,
    /* .page_links_get                */ NULL,
    /* .page_render_pre               */ _etui_djvu_page_render_pre,
    /* .page_render                   */ _etui_djvu_page_render,
    /* .page_render_end               */ _etui_djvu_page_render_end,
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
etui_module_djvu_init(void)
{
    if (_etui_module_djvu_init_count > 0)
    {
        _etui_module_djvu_init_count++;
        return EINA_TRUE;
    }

    _etui_module_djvu_log_domain = eina_log_domain_register("etui-djvu",
                                                          ETUI_MODULE_DJVU_DEFAULT_LOG_COLOR);
    if (_etui_module_djvu_log_domain < 0)
    {
        EINA_LOG_CRIT("Could not register log domain 'etui-djvu'");
        return EINA_FALSE;
    }

    if (!etui_module_register(&_etui_provider_descriptor_djvu))
    {
        ERR("Could not register module %p", &_etui_provider_descriptor_djvu);
        goto unregister_log;
    }

    _etui_module_djvu_init_count = 1;
    return EINA_TRUE;

  unregister_log:
    eina_log_domain_unregister(_etui_module_djvu_log_domain);
    _etui_module_djvu_log_domain = -1;

    return EINA_FALSE;
}

void
etui_module_djvu_shutdown(void)
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

    _etui_module_djvu_init_count = 0;

    etui_module_unregister(&_etui_provider_descriptor_djvu);

    eina_log_domain_unregister(_etui_module_djvu_log_domain);
    _etui_module_djvu_log_domain = -1;
}

#ifndef ETUI_BUILD_STATIC_DJVU

EINA_MODULE_INIT(etui_module_djvu_init);
EINA_MODULE_SHUTDOWN(etui_module_djvu_shutdown);

#endif

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
