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

#include <iapi.h>
#include <ierrors.h>
#include <gdevdsp.h>

#include "Etui.h"
#include "etui_module.h"
#include "etui_module_ps.h"
#include "ps.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#ifdef ETUI_MODULE_PS_DEFAULT_LOG_COLOR
# undef ETUI_MODULE_PS_DEFAULT_LOG_COLOR
#endif
#define ETUI_MODULE_PS_DEFAULT_LOG_COLOR EINA_COLOR_LIGHTCYAN

#ifdef ERR
# undef ERR
#endif
#define ERR(...)  EINA_LOG_DOM_ERR(_etui_module_ps_log_domain, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...)  EINA_LOG_DOM_DBG(_etui_module_ps_log_domain, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...)  EINA_LOG_DOM_INFO(_etui_module_ps_log_domain, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...)  EINA_LOG_DOM_WARN(_etui_module_ps_log_domain, __VA_ARGS__)

#ifdef CRIT
# undef CRIT
#endif
#define CRIT(...) EINA_LOG_DOM_CRIT(_etui_module_ps_log_domain, __VA_ARGS__)

typedef struct _Etui_Provider_Data Etui_Provider_Data;

struct _Etui_Provider_Data
{
    /* specific EFL stuff for the module */

    struct {
        Evas_Object *obj;
        void *m;
    } efl;

    /* specific PS stuff for the module */

    /* Document */
    struct
    {
        char *filename;
        FILE *f;
        struct document *doc;
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
        float hdpi;
        float vdpi;
        int text_alpha_bits;
        int graphic_alpha_bits;
        unsigned int use_platform_fonts : 1;
    } page;

    struct
    {
        void *instance;
        int width;
        int height;
        int stride;
        unsigned char *image;
    } gs;
};

static int _etui_module_ps_init_count = 0;
static int _etui_module_ps_log_domain = -1;

static int
_etui_ps_stdout_cb(void *caller_handle EINA_UNUSED, const char *buf, int len)
{
    char *str;

    /* according to chris, buf might be not nul terminated */
    str = (char *)malloc((len + 1) * sizeof(char));
    if (!str)
        return len;

    memcpy(str, buf, len);
    str[len] = '\0';
    INF(str);
    free(str);

    return len;
}

static int
_etui_ps_stderr_cb(void *caller_handle EINA_UNUSED, const char *buf, int len)
{
    char *str;

    /* according to chris, buf might be not nul terminated */
    str = (char *)malloc((len + 1) * sizeof(char));
    if (!str)
        return len;

    memcpy(str, buf, len);
    str[len] = '\0';
    ERR(str);
    free(str);

    return len;
}

static int
_etui_ps_display_cb_open(void *d EINA_UNUSED, void *device EINA_UNUSED)
{
    return 0;
}

static int
_etui_ps_display_cb_preclose(void *d EINA_UNUSED, void *device EINA_UNUSED)
{
    return 0;
}

static int
_etui_ps_display_cb_close(void *d EINA_UNUSED, void *device EINA_UNUSED)
{
    return 0;
}

static int
_etui_ps_display_cb_presize(void *d, void *device EINA_UNUSED, int width, int height, int stride, unsigned int format EINA_UNUSED)
{
    Etui_Provider_Data *pd;

    if (!d)
        return 0;

    pd = (Etui_Provider_Data *)d;

    pd->gs.width = width;
    pd->gs.height = height;
    pd->gs.stride = stride;
    pd->gs.image = NULL;

    return 0;
}

static int
_etui_ps_display_cb_size(void *d, void *device EINA_UNUSED, int width EINA_UNUSED, int height EINA_UNUSED, int stride EINA_UNUSED, unsigned int format EINA_UNUSED, unsigned char *image)
{
    Etui_Provider_Data *pd;

    if (!d)
        return 0;

    pd = (Etui_Provider_Data *)d;

    pd->gs.image = image;

    return 0;
}

static int
_etui_ps_display_cb_sync(void *d EINA_UNUSED, void *device EINA_UNUSED)
{
    return 0;
}

static int
_etui_ps_display_cb_page(void *d, void *device EINA_UNUSED, int copies EINA_UNUSED, int flush EINA_UNUSED)
{
    Etui_Provider_Data *pd;
    int i;

    if (!d)
        return 0;

    pd = (Etui_Provider_Data *)d;

    for (i = 0; i < pd->gs.height; i++)
        memcpy((unsigned char *)pd->efl.m + pd->gs.width * 4 * i,
               pd->gs.image + pd->gs.stride * i,
               pd->gs.width * 4);

    return 0;
}

static int
_etui_ps_display_cb_update(void *d EINA_UNUSED, void *device EINA_UNUSED, int x EINA_UNUSED, int y EINA_UNUSED, int width EINA_UNUSED, int height EINA_UNUSED)
{
    return 0;
}

static const display_callback _etui_ps_display_cb =
{
    sizeof(display_callback),
    DISPLAY_VERSION_MAJOR,
    DISPLAY_VERSION_MINOR,
    _etui_ps_display_cb_open,
    _etui_ps_display_cb_preclose,
    _etui_ps_display_cb_close,
    _etui_ps_display_cb_presize,
    _etui_ps_display_cb_size,
    _etui_ps_display_cb_sync,
    _etui_ps_display_cb_page,
    _etui_ps_display_cb_update
};

static Eina_Bool
_etui_ps_gs_process(Etui_Provider_Data *pd, int x, int y, long begin, long end)
{
#define BUFFER_SIZE 32768
    static char buf[BUFFER_SIZE];
    int err;
    int exit_code;
    unsigned int r;
    size_t left;

    if (fseek(pd->doc.f, begin, SEEK_SET) < 0)
        return EINA_FALSE;

    err = gsapi_run_string_begin(pd->gs.instance, 0, &exit_code);
    if (err < 0)
        return EINA_FALSE;

    if ((x != 0) || (y != 0))
    {
        char set[256];

        snprintf (set, sizeof(set), "%d %d translate\n", -x, -y);
        err = gsapi_run_string_continue(pd->gs.instance, set, strlen (set),
                                        0, &exit_code);
        err = (err == e_NeedInput) ? 0 : err;
        if ((err != e_NeedInput) && err < 0)
                return EINA_FALSE;
    }

    left = end - begin;
    while (left > 0)
    {
        size_t to_read;

        to_read = BUFFER_SIZE;
        if (left < to_read)
            to_read = left;

        r = fread(buf, sizeof(char), to_read, pd->doc.f);
        err = gsapi_run_string_continue(pd->gs.instance, buf, r, 0, &exit_code);
        err = (err == e_NeedInput) ? 0 : err;
        left -= r;
    }

    if (err < 0)
        return EINA_FALSE;

    err = gsapi_run_string_end(pd->gs.instance, 0, &exit_code);
    if (err < 0)
        return EINA_FALSE;

    return EINA_TRUE;
}

/* Virtual functions */

static void *
_etui_ps_init(Evas *evas)
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
_etui_ps_shutdown(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    DBG("shutdown module");

    pd = (Etui_Provider_Data *)d;

    if (pd->doc.doc)
        psdocdestroy(pd->doc.doc);
    if (pd->doc.filename)
        free(pd->doc.filename);
    evas_object_del(pd->efl.obj);
    free(pd);
}

static Evas_Object *
_etui_ps_evas_object_get(void *d)
{
    if (!d)
        return NULL;

    return ((Etui_Provider_Data *)d)->efl.obj;
}

static Eina_Bool
_etui_ps_file_open(void *d, const char *filename)
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

    if (pd->doc.f)
        fclose(pd->doc.f);
    pd->doc.f = fopen(filename, "rb");
    if (!pd->doc.f)
        goto free_filename;

    if (pd->doc.doc)
        psdocdestroy(pd->doc.doc);
    pd->doc.doc = psscan(filename, SCANSTYLE_NORMAL);
    if (!pd->doc.doc)
    {
        ERR("Could not open file %s", filename);
        goto close_f;
    }

    if ((pd->doc.doc->numpages == 0) &&
        (pd->doc.doc->lenprolog == 0))
    {
        ERR("Invalid PostScript file");
        goto destroy_doc;
    }
    else if ((pd->doc.doc->numpages == 0) &&
             (!pd->doc.doc->format))
    {
        /* FIXME: render the first page */
        ERR("Invalid PostScript file");
        goto destroy_doc;
    }

    pd->page.page_num = -1;

    return EINA_TRUE;

  destroy_doc:
    psdocdestroy(pd->doc.doc);
    pd->doc.doc = NULL;
  close_f:
    fclose(pd->doc.f);
  free_filename:
    free(pd->doc.filename);
    pd->doc.filename = NULL;

  return EINA_FALSE;
}

static void
_etui_ps_file_close(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    pd = (Etui_Provider_Data *)d;

    DBG("close file %s", pd->doc.filename);

    if (pd->doc.doc)
        psdocdestroy(pd->doc.doc);
    pd->doc.doc = NULL;

    if (pd->doc.f)
        fclose(pd->doc.f);
    pd->doc.f = NULL;

    if (pd->doc.filename)
        free(pd->doc.filename);
    pd->doc.filename = NULL;
}

static void
_etui_ps_version_get(void *d EINA_UNUSED, int *maj, int *min)
{
    if (maj) *maj = -1;
    if (min) *min = -1;
}

static char *
_etui_ps_title_get(void *d)
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

    return strdup(pd->doc.doc->title);
}

static char *
_etui_ps_creator_get(void *d)
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

    return strdup(pd->doc.doc->creator);
}

static char *
_etui_ps_creation_date_get(void *d)
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

    return strdup(pd->doc.doc->date);
}

static int
_etui_ps_pages_count(void *d)
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

    if ((!pd->doc.doc->epsf && pd->doc.doc->numpages > 0) ||
        (pd->doc.doc->epsf && pd->doc.doc->numpages > 1))
        return pd->doc.doc->numpages;
    else
        return 1;
}

static Eina_Bool
_etui_ps_page_set(void *d, int page_num)
{
    Etui_Provider_Data *pd;
    int idx;

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

    idx = ((pd->doc.doc->pageorder == DESCEND) ?
           ((int)pd->doc.doc->numpages - 1) - page_num :
           page_num);

    if (idx >= (int)pd->doc.doc->numpages)
        return EINA_FALSE;

    if (idx == pd->page.page_num)
        return EINA_FALSE;

    pd->page.width = 0;
    pd->page.height = 0;

    pd->page.page_num = idx;
    pd->page.rotation = ETUI_ROTATION_0;
    pd->page.hscale = 1.0f;
    pd->page.vscale = 1.0f;
    pd->page.hdpi = 72.0f;
    pd->page.vdpi = 72.0f;
    pd->page.text_alpha_bits = 4;
    pd->page.graphic_alpha_bits = 2;
    pd->page.use_platform_fonts = EINA_TRUE;

    return EINA_TRUE;
}

static int
_etui_ps_page_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return -1;

    pd = (Etui_Provider_Data *)d;

    return pd->page.page_num;
}

static void
_etui_ps_page_size_get(void *d, int *width, int *height)
{
    Etui_Provider_Data *pd;
    int x0;
    int x1;
    int y0;
    int y1;

    if (!d)
        goto _err;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        goto _err;
    }

    if ((pd->page.width == 0) || (pd->page.width == 0))
    {
        psgetpagebox(pd->doc.doc, pd->page.page_num,
                     &x0, &y0, &x1, &y1);
        if (width) *width = x0 - x1;
        if (height) *height = y0 - y1;
    }
    else
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
_etui_ps_page_rotation_set(void *d, Etui_Rotation rotation)
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
_etui_ps_page_rotation_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return ETUI_ROTATION_0;

    pd = (Etui_Provider_Data *)d;
    return pd->page.rotation;
}

static Eina_Bool
_etui_ps_page_scale_set(void *d, float hscale, float vscale)
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
_etui_ps_page_scale_get(void *d, float *hscale, float *vscale)
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
_etui_ps_page_dpi_set(void *d, float hdpi, float vdpi)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if ((pd->page.hdpi == hdpi) && (pd->page.vdpi == vdpi))
        return EINA_TRUE;

    pd->page.hdpi = hdpi;
    pd->page.vdpi = vdpi;

    return EINA_TRUE;
}

static void
_etui_ps_page_dpi_get(void *d, float *hdpi, float *vdpi)
{
    Etui_Provider_Data *pd;

    if (!d)
    {
        if (hdpi) *hdpi = 72.0f;
        if (vdpi) *vdpi = 72.0f;
        return;
    }

    pd = (Etui_Provider_Data *)d;

    if (hdpi) *hdpi = pd->page.hdpi;
    if (vdpi) *vdpi = pd->page.vdpi;
}

static void
_etui_ps_page_render_pre(void *d)
{
    Etui_Provider_Data *pd;
    int x0;
    int x1;
    int y0;
    int y1;
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

    psgetpagebox(pd->doc.doc, pd->page.page_num,
                 &x0, &y0, &x1, &y1);
    width = (int)(((x0 - x1) * pd->page.hscale) + 0.5);
    height = (int)(((y0 - y1) * pd->page.vscale) + 0.5);

    evas_object_image_size_set(pd->efl.obj, width, height);
    evas_object_image_fill_set(pd->efl.obj, 0, 0, width, height);
    pd->efl.m = evas_object_image_data_get(pd->efl.obj, 1);
    pd->page.width = width;
    pd->page.height = height;

    evas_object_resize(pd->efl.obj, width, height);
}

static void
_etui_ps_page_render(void *d)
{
    char text_alpha[256];
    char graphic_alpha[256];
    char size[256];
    char resolution[256];
    char display_format[256];
    char display_handle[256];
    char fmt[256];
    char str[256];
    char **args;
    int n_args;
    int arg;
    int err;
    int exit_code;
    int page_urx;
    int page_ury;
    int page_llx;
    int page_lly;
    int bbox_urx;
    int bbox_ury;
    int bbox_llx;
    int bbox_lly;
    int hoffset;
    int voffset;
    int page_hoffset;
    int page_voffset;
    int doc_hoffset;
    int doc_voffset;
    Etui_Provider_Data *pd;

    if (!d)
        return;

    DBG("render");

    pd = (Etui_Provider_Data *)d;

    err = gsapi_new_instance(&pd->gs.instance, pd);
    /* TODO: should I call some functions here ? */
    if (err < 0)
    {
        ERR("can not create ghostscript instance");
        pd->gs.instance = NULL;
        return;
    }

    err = gsapi_set_stdio(pd->gs.instance,
                          NULL,
                          _etui_ps_stdout_cb,
                          _etui_ps_stderr_cb);
    if (err < 0)
        goto delete_gs_instance;

    err = gsapi_set_display_callback(pd->gs.instance,
                                     (display_callback *)&_etui_ps_display_cb);
    if (err < 0)
        goto delete_gs_instance;

    n_args = 14;
    arg = 0;

    args = (char **)calloc(n_args, sizeof(char *));
    if (!args)
        goto delete_gs_instance;

    args[arg++] = "etui";
    args[arg++] = "-dMaxBitmap=10000000";
    args[arg++] = "-dSAFER";
    args[arg++] = "-dNOPAUSE";
    args[arg++] = "-dNOPAGEPROMPT";
    args[arg++] = "-P-";
    args[arg++] = "-sDEVICE=display";
    snprintf(text_alpha, sizeof(text_alpha),
             "-dTextAlphaBits=%d",
             pd->page.text_alpha_bits);
    args[arg++] = text_alpha;
    snprintf(graphic_alpha, sizeof(graphic_alpha),
             "-dGraphicsAlphaBits=%d",
             pd->page.graphic_alpha_bits);
    args[arg++] = graphic_alpha;
    printf("%dx%d\n", pd->page.width, pd->page.height);
    snprintf(size, sizeof(size), "-g%dx%d", pd->page.width, pd->page.height);
    args[arg++] = size;
    snprintf(resolution, sizeof(resolution), "-r%fx%f",
             pd->page.hscale * pd->page.hdpi,
             pd->page.vscale * pd->page.vdpi);
    args[arg++] = resolution;
    snprintf(display_format, sizeof(display_format),
             "-dDisplayFormat=%d",
             DISPLAY_COLORS_RGB |
             DISPLAY_DEPTH_8 |
             DISPLAY_ROW_ALIGN_32 |
#ifdef WORDS_BIGENDIAN
             DISPLAY_UNUSED_FIRST |
             DISPLAY_BIGENDIAN |
#else
             DISPLAY_UNUSED_LAST |
             DISPLAY_LITTLEENDIAN |
#endif
             DISPLAY_TOPFIRST);
    args[arg++] = display_format;
    snprintf(fmt, sizeof(fmt),
              "-sDisplayHandle=16#%s",
              sizeof(d) == 4 ? "%lx" : "%llx");
    snprintf(display_handle, sizeof(display_handle), fmt, d);
    args[arg++] = display_handle;
    /* FIXME: platform fonts */
    args[arg++] = "-dNOPLATFONTS";

    err = gsapi_init_with_args(pd->gs.instance, n_args, args);
    free(args);
    if (err < 0)
        goto delete_gs_instance;

    snprintf(str, sizeof(str),
             "<< /Orientation %d >> setpagedevice .locksafe",
             pd->page.rotation);
    err = gsapi_run_string_with_length(pd->gs.instance,
                                       str, strlen(str), 0, &exit_code);

    if (err < 0)
        goto delete_gs_instance;

    /* offsets */
    hoffset = 0;
    voffset = 0;
    page_hoffset = 0;
    page_voffset = 0;
    doc_hoffset = 0;
    doc_voffset = 0;
    if (psgetpagebbox(pd->doc.doc, pd->page.page_num,
                      &bbox_urx, &bbox_ury, &bbox_llx, &bbox_lly))
    {
        psgetpagebox(pd->doc.doc, pd->page.page_num,
                     &page_urx, &page_ury, &page_llx, &page_lly);
        if ((bbox_urx - bbox_llx) == (page_urx - page_llx) ||
            (bbox_ury - bbox_lly) == (page_ury - page_lly))
        {
            hoffset = page_llx;
            voffset = page_lly;
        }
    }

    if (pd->doc.doc->numpages > 0)
    {
        page_hoffset = hoffset;
        page_voffset = voffset;
    }
    else
    {
        doc_hoffset = hoffset;
        doc_voffset = voffset;
    }

    if (!_etui_ps_gs_process(pd, doc_hoffset, doc_voffset, pd->doc.doc->beginprolog, pd->doc.doc->endprolog))
        goto delete_gs_instance;

    if (!_etui_ps_gs_process(pd, 0, 0, pd->doc.doc->beginsetup, pd->doc.doc->endsetup))
        goto delete_gs_instance;

    if (pd->doc.doc->numpages > 0)
    {
        if (pd->doc.doc->pageorder == SPECIAL)
        {
            int i;

            /* Pages cannot be re-ordered */

            for (i = 0; i < pd->page.page_num; i++)
            {
                if (!_etui_ps_gs_process(pd,
                                         page_hoffset,
                                         page_voffset,
                                         pd->doc.doc->pages[i].begin,
                                         pd->doc.doc->pages[i].end))
                    return;
            }
        }

        if (!_etui_ps_gs_process(pd,
                                 page_hoffset,
                                 page_voffset,
                                 pd->doc.doc->pages[pd->page.page_num].begin,
                                 pd->doc.doc->pages[pd->page.page_num].end))
            return;
    }

    if (!_etui_ps_gs_process(pd, 0, 0, pd->doc.doc->begintrailer, pd->doc.doc->endtrailer))
    {
        gsapi_exit(pd->gs.instance);
        gsapi_delete_instance(pd->gs.instance);
        pd->gs.instance = NULL;
        return;
    }

    return;

  delete_gs_instance:
    gsapi_exit(pd->gs.instance);
    gsapi_delete_instance(pd->gs.instance);
    pd->gs.instance = NULL;
}

static void
_etui_ps_page_render_end(void *d)
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


static Etui_Provider_Descriptor _etui_provider_descriptor_ps =
{
    /* .name                          */ "ps",
    /* .version                       */ ETUI_PROVIDER_DESCRIPTOR_VERSION,
    /* .priority                      */ ETUI_PROVIDER_DESCRIPTOR_PRIORITY_LOW,
    /* .init                          */ _etui_ps_init,
    /* .shutdown                      */ _etui_ps_shutdown,
    /* .evas_object_get               */ _etui_ps_evas_object_get,
    /* .file_open                     */ _etui_ps_file_open,
    /* .file_close                    */ _etui_ps_file_close,
    /* .version_get                   */ _etui_ps_version_get,
    /* .title_get                     */ _etui_ps_title_get,
    /* .author_get                    */ NULL,
    /* .subject_get                   */ NULL,
    /* .keywords_get                  */ NULL,
    /* .creator_get                   */ _etui_ps_creator_get,
    /* .producer_get                  */ NULL,
    /* .creation_date_get             */ _etui_ps_creation_date_get,
    /* .modification_date_get         */ NULL,
    /* .is_printable                  */ NULL,
    /* .is_changeable                 */ NULL,
    /* .is_copyable                   */ NULL,
    /* .is_notable                    */ NULL,
    /* .password_needed               */ NULL,
    /* .password_set                  */ NULL,
    /* .pages_count                   */ _etui_ps_pages_count,
    /* .toc_get                       */ NULL,
    /* .page_use_display_list_set     */ NULL,
    /* .page_use_display_list_get     */ NULL,
    /* .page_set                      */ _etui_ps_page_set,
    /* .page_get                      */ _etui_ps_page_get,
    /* .page_size_get                 */ _etui_ps_page_size_get,
    /* .page_rotation_set             */ _etui_ps_page_rotation_set,
    /* .page_rotation_get             */ _etui_ps_page_rotation_get,
    /* .page_scale_set                */ _etui_ps_page_scale_set,
    /* .page_scale_get                */ _etui_ps_page_scale_get,
    /* .page_dpi_set                  */ _etui_ps_page_dpi_set,
    /* .page_dpi_get                  */ _etui_ps_page_dpi_get,
    /* .page_links_get                */ NULL,
    /* .page_render_pre               */ _etui_ps_page_render_pre,
    /* .page_render                   */ _etui_ps_page_render,
    /* .page_render_end               */ _etui_ps_page_render_end,
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
etui_module_ps_init(void)
{
    if (_etui_module_ps_init_count > 0)
    {
        _etui_module_ps_init_count++;
        return EINA_TRUE;
    }

    _etui_module_ps_log_domain = eina_log_domain_register("etui-ps",
                                                          ETUI_MODULE_PS_DEFAULT_LOG_COLOR);
    if (_etui_module_ps_log_domain < 0)
    {
        EINA_LOG_CRIT("Could not register log domain 'etui-ps'");
        return EINA_FALSE;
    }

    if (!etui_module_register(&_etui_provider_descriptor_ps))
    {
        ERR("Could not register module %p", &_etui_provider_descriptor_ps);
        goto unregister_log;
    }

    _etui_module_ps_init_count = 1;
    return EINA_TRUE;

  unregister_log:
    eina_log_domain_unregister(_etui_module_ps_log_domain);
    _etui_module_ps_log_domain = -1;

    return EINA_FALSE;
}

void
etui_module_ps_shutdown(void)
{
    if (_etui_module_ps_init_count > 1)
    {
        _etui_module_ps_init_count--;
        return;
    }
    else if (_etui_module_ps_init_count == 0)
    {
        ERR("Too many etui_module_ps_shutdown() calls");
        return;
    }

    DBG("shutdown ps module");

    _etui_module_ps_init_count = 0;

    etui_module_unregister(&_etui_provider_descriptor_ps);

    eina_log_domain_unregister(_etui_module_ps_log_domain);
    _etui_module_ps_log_domain = -1;
}

#ifndef ETUI_BUILD_STATIC_PS

EINA_MODULE_INIT(etui_module_ps_init);
EINA_MODULE_SHUTDOWN(etui_module_ps_shutdown);

#endif

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
