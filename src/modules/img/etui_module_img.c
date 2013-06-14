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
        unsigned int is_archive : 1;
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
};

static int _etui_module_img_init_count = 0;
static int _etui_module_img_log_domain = -1;

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

    pd->doc.is_archive = 1;

    pd->page.page_num = -1;

    return EINA_TRUE;

  /* free_filename: */
  /*   free(pd->doc.filename); */
  /*   pd->doc.filename = NULL; */

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
_etui_img_version_get(void *d, int *maj, int *min)
{
    Etui_Provider_Data *pd;

    if (!d)
        goto _err;

    pd = (Etui_Provider_Data *)d;

    return;

  _err:
    if (maj) *maj = -1;
    if (min) *min = -1;
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
        }
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

    /* evas_object_image_size_get(pd->efl.obj, &width, &height); */
    /* evas_object_image_data_set(pd->efl.obj, pd->efl.m); */
    /* evas_object_image_data_update_add(pd->efl.obj, 0, 0, width, height); */
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
