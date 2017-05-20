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

#include <tiff.h>
#include <tiffio.h>

#include "Etui.h"
#include "etui_module.h"
#include "etui_file.h"
#include "etui_module_tiff.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#ifdef ETUI_MODULE_TIFF_DEFAULT_LOG_COLOR
# undef ETUI_MODULE_TIFF_DEFAULT_LOG_COLOR
#endif
#define ETUI_MODULE_TIFF_DEFAULT_LOG_COLOR EINA_COLOR_LIGHTCYAN

#ifdef ERR
# undef ERR
#endif
#define ERR(...)  EINA_LOG_DOM_ERR(_etui_module_tiff_log_domain, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...)  EINA_LOG_DOM_DBG(_etui_module_tiff_log_domain, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...)  EINA_LOG_DOM_INFO(_etui_module_tiff_log_domain, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...)  EINA_LOG_DOM_WARN(_etui_module_tiff_log_domain, __VA_ARGS__)

#ifdef CRIT
# undef CRIT
#endif
#define CRIT(...) EINA_LOG_DOM_CRIT(_etui_module_tiff_log_domain, __VA_ARGS__)

typedef struct
{
    /* specific EFL stuff for the module */

    struct {
        Evas_Object *obj;
    } efl;

    /* specific TIFF stuff for the module */

    /* Document */
    struct
    {
        char *filename;
        TIFF *tiff;
        Etui_Module_Tiff_Info *info; /* information specific to the document (creator, ...) */
        int page_nbr;
        Eina_Array toc;
        void *data;
        size_t size;
    } doc;

    /* Current page */
    struct
    {
        TIFFRGBAImage img;
        unsigned int *raster;
        unsigned int width;
        unsigned int height;
        int page_num;
        Etui_Rotation rotation;
        float hscale;
        float vscale;
        unsigned int has_begun : 1;
        unsigned int has_rastered : 1;
    } page;
} Etui_Module_Data;

static int _etui_module_tiff_init_count = 0;
static int _etui_module_tiff_log_domain = -1;


/* private functions */

static void
_etui_tiff_info_set(Etui_Module_Data *md)
{

    {
        /* string tags */

#define TAG_STRING_SET(memb_, tag_) \
        md->doc.info->memb_ = TIFFGetField(md->doc.tiff, TIFFTAG_ ## tag_, &val) ? strdup(val) : NULL

        char *val;

        TAG_STRING_SET(artist, ARTIST);
        TAG_STRING_SET(copyright, COPYRIGHT);
        TAG_STRING_SET(date_time, DATETIME);
        TAG_STRING_SET(document_name, DOCUMENTNAME);
        TAG_STRING_SET(image_description, IMAGEDESCRIPTION);
        TAG_STRING_SET(make, MAKE);
        TAG_STRING_SET(model, MODEL);
        TAG_STRING_SET(software, SOFTWARE);

#undef TAG_STRING_SET
    }

    {
        /* float tags */

#define TAG_FLOAT_SET(memb_, tag_) \
        md->doc.info->memb_ = TIFFGetField(md->doc.tiff, TIFFTAG_ ## tag_, &val) ? val : -1.0

        float val;

        TAG_FLOAT_SET(resolution_x, XRESOLUTION);
        TAG_FLOAT_SET(resolution_y, YRESOLUTION);

#undef TAG_FLOAT_SET
    }

    {
        /* unsigned int tags */

#define TAG_UINT32_SET(memb_, tag_) \
        md->doc.info->memb_ = TIFFGetField(md->doc.tiff, TIFFTAG_ ## tag_, &val) ? val : 0xffffffff

        unsigned int val;

        TAG_UINT32_SET(rows_per_strip, ROWSPERSTRIP);

#undef TAG_UINT32_SET
    }

    {
        /* unsigned short tags */

#define TAG_UINT16_SET(memb_, tag_) \
        md->doc.info->memb_ = TIFFGetField(md->doc.tiff, TIFFTAG_ ## tag_, &val) ? val : 0xffff

        unsigned short val;

        TAG_UINT16_SET(bits_per_sample, BITSPERSAMPLE);
        TAG_UINT16_SET(compression, COMPRESSION);
        TAG_UINT16_SET(orientation, ORIENTATION);
        TAG_UINT16_SET(photometric, PHOTOMETRIC);
        TAG_UINT16_SET(planar_configuration, PLANARCONFIG);
        TAG_UINT16_SET(resolution_unit, RESOLUTIONUNIT);
        TAG_UINT16_SET(sample_format, SAMPLEFORMAT);
        TAG_UINT16_SET(samples_per_pixel, SAMPLESPERPIXEL);

#undef TAG_UINT16_SET
    }

    {
        /* miscelaneous */

        unsigned short v1;
        unsigned short v2;

        if (TIFFGetField(md->doc.tiff, TIFFTAG_YCBCRSUBSAMPLING, &v1, &v2))
        {
            md->doc.info->ycbcr_subssampling[0] = v1;
            md->doc.info->ycbcr_subssampling[1] = v2;
        }
        else
        {
            md->doc.info->ycbcr_subssampling[0] = 0xffff;
            md->doc.info->ycbcr_subssampling[1] = 0xffff;
        }
    }
}

/* Virtual functions */

static void *
_etui_tiff_init(const Etui_File *ef)
{
    Etui_Module_Data *md;

    md = (Etui_Module_Data *)calloc(1, sizeof(Etui_Module_Data));
    if (!md)
        return NULL;

    DBG("init module");

    md->doc.tiff = TIFFOpen(etui_file_filename_get(ef), "r");
    if (!md->doc.tiff)
        goto free_md;

    md->doc.info = (Etui_Module_Tiff_Info *)calloc(1, sizeof(Etui_Module_Tiff_Info));
    if (!md->doc.info)
    {
        ERR("Could not allocate memory for information structure");;
        goto close_tiff;
    }

    md->doc.page_nbr = TIFFNumberOfDirectories(md->doc.tiff);
    md->page.page_num = -1;
    md->page.rotation = ETUI_ROTATION_0;
    md->page.hscale = 1.0f;
    md->page.vscale = 1.0f;

    return md;

  close_tiff:
    if (md->page.raster)
        _TIFFfree(md->page.raster);
    if (md->doc.tiff)
        TIFFClose(md->doc.tiff);
  free_md:
    free(md);

    return NULL;
}

static void
_etui_tiff_shutdown(void *d)
{
    Etui_Module_Data *md;

    DBG("shutdown module");

    if (!d)
        return;

    md = (Etui_Module_Data *)d;

    if (md->page.raster)
        _TIFFfree(md->page.raster);
    free(md->doc.info);
    TIFFClose(md->doc.tiff);
    free(md);
}

static Evas_Object *
_etui_tiff_evas_object_add(void *d, Evas *evas)
{
    if (!d)
        return NULL;

    ((Etui_Module_Data *)d)->efl.obj = evas_object_image_add(evas);
    evas_object_move(((Etui_Module_Data *)d)->efl.obj, 0, 0);

    return ((Etui_Module_Data *)d)->efl.obj;
}

static void
_etui_tiff_evas_object_del(void *d)
{
    if (!d)
        return;

    evas_object_del(((Etui_Module_Data *)d)->efl.obj);
}

static const void *
_etui_tiff_info_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;

    return md->doc.info;
}

static const char *
_etui_tiff_title_get(void *d)
{
    Etui_Module_Data *md;
    char *txt;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;

    if (TIFFGetField(md->doc.tiff, TIFFTAG_DOCUMENTNAME, &txt))
        return txt;

    if (TIFFGetField(md->doc.tiff, TIFFTAG_IMAGEDESCRIPTION, &txt))
        return txt;

    return md->doc.filename;
}

static int
_etui_tiff_pages_count(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return -1;

    md = (Etui_Module_Data *)d;

    return md->doc.page_nbr;
}

static const Eina_Array *
_etui_tiff_toc_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;

    return &md->doc.toc;
}

static Eina_Bool
_etui_tiff_page_set(void *d, int page_num)
{
    Etui_Module_Data *md;

    if (!d)
        return EINA_FALSE;

    md = (Etui_Module_Data *)d;

    if ((page_num < 0) || (page_num >= md->doc.page_nbr))
        return EINA_FALSE;

    if (page_num == md->page.page_num)
        return EINA_FALSE;

    if (!TIFFSetDirectory(md->doc.tiff, page_num))
        return EINA_FALSE;

    md->page.page_num = page_num;
    md->page.rotation = ETUI_ROTATION_0;
    md->page.hscale = 1.0f;
    md->page.vscale = 1.0f;

    /* info */
    _etui_tiff_info_set(md);

    return EINA_TRUE;
}

static int
_etui_tiff_page_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return -1;

    md = (Etui_Module_Data *)d;

    return md->page.page_num;
}

static void
_etui_tiff_page_size_get(void *d, int *width, int *height)
{
    Etui_Module_Data *md;
    unsigned int w;
    unsigned int h;

    if (!d)
        goto _err;

    md = (Etui_Module_Data *)d;

    if (!TIFFGetField(md->doc.tiff, TIFFTAG_IMAGEWIDTH, &w) ||
        !TIFFGetField(md->doc.tiff, TIFFTAG_IMAGELENGTH, &h))
        goto _err;

    if (width) *width = w;
    if (height) *height = h;

    return;

  _err:
    if (width) *width = 0;
    if (height) *height = 0;
}

static Eina_Bool
_etui_tiff_page_rotation_set(void *d, Etui_Rotation rotation)
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
_etui_tiff_page_rotation_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return ETUI_ROTATION_0;

    md = (Etui_Module_Data *)d;
    return md->page.rotation;
}

static Eina_Bool
_etui_tiff_page_scale_set(void *d, float hscale, float vscale)
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
_etui_tiff_page_scale_get(void *d, float *hscale, float *vscale)
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
_etui_tiff_page_render_pre(void *d)
{
    char emsg[1024];
    Etui_Module_Data *md;

    if (!d)
        return;

    DBG("render pre");

    md = (Etui_Module_Data *)d;

    md->page.has_begun = 0;
    md->page.has_rastered = 0;
    if (md->page.raster)
        _TIFFfree(md->page.raster);

    if (!TIFFRGBAImageOK(md->doc.tiff, emsg))
        return;

    if (TIFFRGBAImageBegin(&md->page.img, md->doc.tiff, 0, emsg))
    {
        unsigned int *raster;
        unsigned int width;
        unsigned int height;

        md->page.img.req_orientation = ORIENTATION_TOPLEFT;

        /* scale first */
        width = (unsigned int)(md->page.img.width * md->page.hscale);
        height = (unsigned int)(md->page.img.height * md->page.vscale);
        raster = (unsigned int *)_TIFFmalloc(width * height * sizeof(unsigned int));
        if (!raster)
            return;

        evas_object_image_size_set(md->efl.obj, width, height);
        evas_object_image_fill_set(md->efl.obj, 0, 0, width, height);
        evas_object_resize(md->efl.obj, width, height);
        md->page.width = width;
        md->page.height = height;
        md->page.raster = raster;
        md->page.has_begun = 1;
    }
    else
        TIFFError("Etui", "%s", emsg);
}

static void
_etui_tiff_page_render(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return;

    DBG("render");

    md = (Etui_Module_Data *)d;
    if (!md->page.has_begun)
        return;

    if (TIFFRGBAImageGet(&md->page.img, md->page.raster,
                         md->page.width, md->page.height))
        md->page.has_rastered = 1;
}

static void
_etui_tiff_page_render_end(void *d)
{
    Etui_Module_Data *md;
    int width;
    int height;

    if (!d)
        return;

    DBG("render end");

    md = (Etui_Module_Data *)d;
    if (!md->page.has_begun)
        return;

    evas_object_image_size_get(md->efl.obj, &width, &height);
    evas_object_image_data_set(md->efl.obj, md->page.raster);
    evas_object_image_data_update_add(md->efl.obj, 0, 0, width, height);
    TIFFRGBAImageEnd(&md->page.img);
}


static Etui_Module_Func _etui_module_func_tiff =
{
    /* .init              */ _etui_tiff_init,
    /* .shutdown          */ _etui_tiff_shutdown,
    /* .evas_object_add   */ _etui_tiff_evas_object_add,
    /* .evas_object_del   */ _etui_tiff_evas_object_del,
    /* .info_get          */ _etui_tiff_info_get,
    /* .title_get         */ _etui_tiff_title_get,
    /* .pages_count       */ _etui_tiff_pages_count,
    /* .toc_get           */ _etui_tiff_toc_get,
    /* .page_set          */ _etui_tiff_page_set,
    /* .page_get          */ _etui_tiff_page_get,
    /* .page_size_get     */ _etui_tiff_page_size_get,
    /* .page_rotation_set */ _etui_tiff_page_rotation_set,
    /* .page_rotation_get */ _etui_tiff_page_rotation_get,
    /* .page_scale_set    */ _etui_tiff_page_scale_set,
    /* .page_scale_get    */ _etui_tiff_page_scale_get,
    /* .page_render_pre   */ _etui_tiff_page_render_pre,
    /* .page_render       */ _etui_tiff_page_render,
    /* .page_render_end   */ _etui_tiff_page_render_end
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
    if (_etui_module_tiff_init_count > 0)
    {
        _etui_module_tiff_init_count++;
        return EINA_TRUE;
    }

    if (!em)
        return EINA_FALSE;

    _etui_module_tiff_log_domain = eina_log_domain_register("etui-tiff",
                                                            ETUI_MODULE_TIFF_DEFAULT_LOG_COLOR);
    if (_etui_module_tiff_log_domain < 0)
    {
        EINA_LOG_ERR("Can not create a module log domain.");
        return EINA_FALSE;
    }

    /* inititialize external libraries here */

    em->functions = (void *)(&_etui_module_func_tiff);

    _etui_module_tiff_init_count = 1;
    return EINA_TRUE;
}

static void
module_close(Etui_Module *em)
{
    if (_etui_module_tiff_init_count > 1)
    {
        _etui_module_tiff_init_count--;
        return;
    }
    else if (_etui_module_tiff_init_count == 0)
    {
        ERR("Too many etui_module_tiff_shutdown() calls");
        return;
    }

    DBG("shutdown tiff module");

    /* shutdown module here */
    em->functions->shutdown(em->data);

    /* shutdown external libraries here */

    /* shutdown EFL here */

    eina_log_domain_unregister(_etui_module_tiff_log_domain);
    _etui_module_tiff_log_domain = -1;
    _etui_module_tiff_init_count = 0;
}

static Etui_Module_Api _etui_modapi =
{
    "tiff",
    {
        module_open,
        module_close
    }
};

ETUI_MODULE_DEFINE(tiff)

#ifndef ETUI_STATIC_BUILD_TIFF
ETUI_EINA_MODULE_DEFINE(tiff);
#endif

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
