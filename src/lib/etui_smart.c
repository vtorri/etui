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
#include <Ecore.h>
#include <Efreet_Mime.h>

#include "Etui.h"
#include "etui_private.h"
#include "etui_module.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

typedef struct Etui_Smart_Data_ Etui_Smart_Data;

struct Etui_Smart_Data_
{
    EINA_REFCOUNT;

    /* properties */
    Etui_Provider_Instance *provider_instance;
    char *filename;

    /* private */
    Evas_Object *obj;
    Ecore_Thread *render;
};

static Evas_Smart *_etui_smart = NULL;

static void _etui_smart_page_render(void *data, Ecore_Thread *thread);
static void _etui_smart_page_render_end(void *data, Ecore_Thread *thread);
static void _etui_smart_page_render_cancel(void *data, Ecore_Thread *thread);

/* internal smart object routines */

#define ETUI_SMART_OBJ_GET(smart, o, type)             \
{                                                      \
    char *_etui_smart_str;                             \
                                                       \
    if (!o) return;                                    \
    smart = evas_object_smart_data_get(o);             \
    if (!smart) return;                                \
    _etui_smart_str = (char *)evas_object_type_get(o); \
    if (!_etui_smart_str) return;                      \
    if (strcmp(_etui_smart_str, type)) return;         \
}

#define ETUI_SMART_OBJ_GET_RETURN(smart, o, type, ret) \
{                                                      \
    char *_etui_smart_str;                             \
                                                       \
    if (!o) return ret;                                \
    smart = evas_object_smart_data_get(o);             \
    if (!smart) return ret;                            \
    _etui_smart_str = (char *)evas_object_type_get(o); \
    if (!_etui_smart_str) return ret;                  \
    if (strcmp(_etui_smart_str, type)) return ret;     \
}

#define ETUI_OBJ_NAME "etui_object"

/* static void */
/* _mouse_move(void *data, Evas *ev EINA_UNUSED, Evas_Object *obj, void *event_info) */
/* { */
/*     Evas_Event_Mouse_Move *e; */
/*     Etui_Smart_Data *sd; */
/*     int x, y, iw, ih; */
/*     Evas_Coord ox, oy, ow, oh; */

/*     e = event_info; */
/*     sd = data; */
/*     evas_object_geometry_get(obj, &ox, &oy, &ow, &oh); */
/*     evas_object_image_size_get(obj, &iw, &ih); */
/*     if ((iw < 1) || (ih < 1)) return; */
/*     x = (((int)e->cur.canvas.x - ox) * iw) / ow; */
/*     y = (((int)e->cur.canvas.y - oy) * ih) / oh; */
/*     emotion_engine_instance_event_mouse_move_feed(sd->engine_instance, x, y); */
/* } */

/* static void */
/* _mouse_down(void *data, Evas *ev EINA_UNUSED, Evas_Object *obj, void *event_info) */
/* { */
/*     Evas_Event_Mouse_Down *e; */
/*     Etui_Smart_Data *sd; */
/*     int x, y, iw, ih; */
/*     Evas_Coord ox, oy, ow, oh; */

/*     e = event_info; */
/*     sd = data; */
/*     evas_object_geometry_get(obj, &ox, &oy, &ow, &oh); */
/*     evas_object_image_size_get(obj, &iw, &ih); */
/*     if ((iw < 1) || (ih < 1)) return; */
/*     x = (((int)e->canvas.x - ox) * iw) / ow; */
/*     y = (((int)e->canvas.y - oy) * ih) / oh; */
/*     emotion_engine_instance_event_mouse_button_feed(sd->engine_instance, 1, x, y); */
/* } */

static void
_etui_smart_add(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = calloc(1, sizeof(Etui_Smart_Data));
    if (!sd) return;

    EINA_REFCOUNT_INIT(sd);

/*     evas_object_event_callback_add(sd->obj, EVAS_CALLBACK_MOUSE_MOVE, _etui_mouse_move, sd); */
/*     evas_object_event_callback_add(sd->obj, EVAS_CALLBACK_MOUSE_DOWN, _etui_mouse_down, sd); */
    evas_object_smart_data_set(obj, sd);
}

static void
_etui_smart_del(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    if (!sd) return;

    EINA_REFCOUNT_UNREF(sd)
    {
        if (sd->provider_instance)
        {
            etui_provider_instance_file_close(sd->provider_instance);
            etui_provider_instance_del(sd->provider_instance);
            sd->provider_instance = NULL;
        }

        if (sd->obj)
            evas_object_del(sd->obj);
        if (sd->filename)
            free(sd->filename);
        free(sd);
    }
}

static void
_etui_smart_move(Evas_Object *obj, Evas_Coord x, Evas_Coord y)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    if (!sd) return;

    evas_object_move(sd->obj, x, y);
}

static void
_etui_smart_resize(Evas_Object *obj, Evas_Coord w, Evas_Coord h)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    if (!sd) return;

    /* FIXME: not always image object */

    printf(" %s : %dx%d\n", __FUNCTION__, w, h);
    printf(" 도도 _etui_smart_resize : %p\n", obj);
    evas_object_image_fill_set(sd->obj, 0, 0, w, h);
    evas_object_resize(sd->obj, w, h);
}

static void
_etui_smart_show(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    if (!sd) return;

    evas_object_show(sd->obj);

}

static void
_etui_smart_hide(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    if (!sd) return;

    evas_object_hide(sd->obj);
}

static void
_etui_smart_color_set(Evas_Object *obj, int r, int g, int b, int a)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    if (!sd) return;

    evas_object_color_set(sd->obj, r, g, b, a);
}

static void
_etui_smart_clip_set(Evas_Object *obj, Evas_Object *clip)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    if (!sd) return;

    evas_object_clip_set(sd->obj, clip);
}

static void
_etui_smart_clip_unset(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    if (!sd) return;

    evas_object_clip_unset(sd->obj);
}


static void
_etui_smart_calculate(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    if (!sd) return;

    if (sd->render) ecore_thread_cancel(sd->render);
    sd->render = ecore_thread_run(_etui_smart_page_render,
                                    _etui_smart_page_render_end,
                                    _etui_smart_page_render_cancel,
                                    obj);
}

static void
_etui_smart_init(void)
{
    static Evas_Smart_Class sc = EVAS_SMART_CLASS_INIT_NAME_VERSION(ETUI_OBJ_NAME);

    if (_etui_smart) return;

    if (!sc.add)
    {
        sc.add = _etui_smart_add;
        sc.del = _etui_smart_del;
        sc.move = _etui_smart_move;
        sc.resize = _etui_smart_resize;
        sc.show = _etui_smart_show;
        sc.hide = _etui_smart_hide;
        sc.color_set = _etui_smart_color_set;
        sc.clip_set = _etui_smart_clip_set;
        sc.clip_unset = _etui_smart_clip_unset;
/*         sc.callbacks = _etui_smart_callbacks; */
        sc.calculate = _etui_smart_calculate;
    }
    _etui_smart = evas_smart_class_new(&sc);
}

/* private calls */

static void
_etui_smart_page_render(void *data, Ecore_Thread *thread EINA_UNUSED)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(data);
    if (!sd)
        return;

    etui_provider_instance_page_render(sd->provider_instance);
}

static void
_etui_smart_page_render_end(void *data, Ecore_Thread *thread EINA_UNUSED)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(data);
    if (!sd)
        return;

    etui_provider_instance_page_render_end(sd->provider_instance);
    sd->render = NULL;
}

static void
_etui_smart_page_render_cancel(void *data EINA_UNUSED, Ecore_Thread *thread EINA_UNUSED)
{

}

static void
_etui_smart_resize_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
    int w;
    int h;

    evas_object_geometry_get(obj, NULL, NULL, &w, &h);
    evas_object_resize((Evas_Object *)data, w, h);
    printf(" 도도 _etui_smart_resize_cb : data : %p   obj : %p\n", data, obj);
    printf(" %s : %dx%d\n", __FUNCTION__, w, h);
}

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

EAPI Evas_Object *
etui_object_add(Evas *evas)
{
  Evas_Object *o;
    _etui_smart_init();
    o= evas_object_smart_add(evas, _etui_smart);
    printf(" 도도 smart object : %p\n", o);
    return o;
}

EAPI const char *
etui_object_module_name_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_module_name_get(sd->provider_instance);
}

EAPI Eina_Bool
etui_object_file_set(Evas_Object *obj, const char *filename)
{
    char file[PATH_MAX];
    char *res;
    const char *mime;
    const char *module_name = NULL;
    Etui_Smart_Data *sd;
    printf(" 도도 etui_object_file_set : %p\n", obj);

    if (!filename || !*filename)
        return EINA_FALSE;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, EINA_FALSE);

    res = realpath(filename, file);
    if (!res)
        return EINA_FALSE;
    if (sd->filename && (!strcmp(file, sd->filename)))
        return EINA_TRUE;

    mime = efreet_mime_type_get(file);
    INF("mime type: %s", mime);
    if (mime)
    {
        if ((strcmp(mime, "application/pdf") == 0) ||
            (strcmp(mime, "application/vnd.ms-xpsdocument") == 0))
            module_name = "pdf";
        else if (strcmp(mime, "application/postscript") == 0)
            module_name = "ps";
        else if (strcmp(mime, "text/plain") == 0)
            module_name = "txt";
        else if ((strcmp(mime, "application/x-cba") == 0) ||
                 (strcmp(mime, "application/x-cbr") == 0) ||
                 (strcmp(mime, "application/x-cbt") == 0) ||
                 (strcmp(mime, "application/x-cbz") == 0) ||
                 (strcmp(mime, "application/x-cb7") == 0) ||
                 (strcmp(mime, "image/bmp") == 0) ||
                 (strcmp(mime, "image/gif") == 0) ||
                 (strcmp(mime, "image/vnd.microsoft.icon") == 0) ||
                 (strcmp(mime, "image/x-icon") == 0) ||
                 (strcmp(mime, "image/ico") == 0) ||
                 (strcmp(mime, "image/icon") == 0) ||
                 (strcmp(mime, "txt/ico") == 0) ||
                 (strcmp(mime, "application/ico") == 0) ||
                 (strcmp(mime, "image/jpeg") == 0) ||
                 (strcmp(mime, "image/pjpeg") == 0) ||
                 (strcmp(mime, "image/x-portable-pixmap") == 0) ||
                 (strcmp(mime, "image/x-p") == 0) ||
                 (strcmp(mime, "image/x-ppm") == 0) ||
                 (strcmp(mime, "application/ppm") == 0) ||
                 (strcmp(mime, "application/x-ppm") == 0) ||
                 (strcmp(mime, "image/x-portable-greymap") == 0) ||
                 (strcmp(mime, "image/x-pgm") == 0) ||
                 (strcmp(mime, "image/x-portable-bitmap") == 0) ||
                 (strcmp(mime, "image/portable-bitmap") == 0) ||
                 (strcmp(mime, "image/pbm") == 0) ||
                 (strcmp(mime, "image/x-pbm") == 0) ||
                 (strcmp(mime, "image/x-portable-anymap") == 0) ||
                 (strcmp(mime, "image/x-portable/anymap") == 0) ||
                 (strcmp(mime, "image/pbm") == 0) ||
                 (strcmp(mime, "image/png") == 0) ||
                 (strcmp(mime, "image/x-png") == 0) ||
                 (strcmp(mime, "image/x-photoshop") == 0) ||
                 (strcmp(mime, "image/photoshop") == 0) ||
                 (strcmp(mime, "image/psd") == 0) ||
                 (strcmp(mime, "application/photoshop") == 0) ||
                 (strcmp(mime, "application/psd") == 0) ||
                 (strcmp(mime, "zz-application/zz-winassoc-psd") == 0) ||
                 (strcmp(mime, "image/svg+xml") == 0) ||
                 (strcmp(mime, "image/x-tga") == 0) ||
                 (strcmp(mime, "image/tiff") == 0) ||
                 (strcmp(mime, "image/vnd.wap.wbmp") == 0) ||
                 (strcmp(mime, "image/webp") == 0) ||
                 (strcmp(mime, "image/xcf") == 0) ||
                 (strcmp(mime, "image/x-xpixmap") == 0)) /* xpm */
            module_name = "img";
        else if ((strcmp(mime, "image/vnd.djvu") == 0) ||
                 (strcmp(mime, "image/x-djvu") == 0))
            module_name = "djvu";
        else if (strcmp(mime, "application/epub+zip") == 0)
            module_name = "epub";
    }
    INF("module name: %s", module_name);

    /* TODO : iterate over all modules if failure */

    if (etui_provider_instance_name_equal(sd->provider_instance, module_name))
    {
        DBG("no need to reset module, already set: %s", module_name);
    }
    else
    {
        if (sd->provider_instance)
            etui_provider_instance_del(sd->provider_instance);
        sd->provider_instance = etui_provider_instance_new(module_name, evas_object_evas_get(obj));
    }

    if (sd->obj)
        evas_object_del(sd->obj);
    sd->obj = etui_provider_instance_evas_object_get(sd->provider_instance);
    evas_object_smart_member_add(sd->obj, obj);
    evas_object_event_callback_add(sd->obj, EVAS_CALLBACK_RESIZE, _etui_smart_resize_cb, obj);

    if (!sd->provider_instance)
    {
        ERR("can not find a suitable instace (requested: %s)", module_name);
        return EINA_FALSE;
    }

    /* TODO: use stringshare ?? */
    sd->filename = strdup(file);
    if (!etui_provider_instance_file_open(sd->provider_instance, sd->filename))
    {
        WRN("Couldn't open file=%s", sd->filename);
        return EINA_FALSE;
    }

    return EINA_TRUE;
}

EAPI const char *
etui_object_filename_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return sd->filename;
}

EAPI void
etui_object_version_get(Evas_Object *obj, int *maj, int *min)
{
    Etui_Smart_Data *sd;
    char *_etui_smart_str;

    if (!obj)
        goto _err;
    sd = evas_object_smart_data_get(obj);
    if (!sd)
        goto _err;
    _etui_smart_str = (char *)evas_object_type_get(obj);
    if (!_etui_smart_str)
        goto _err;
    if (strcmp(_etui_smart_str, ETUI_OBJ_NAME))
        goto _err;

    etui_provider_instance_version_get(sd->provider_instance, maj, min);

    return;

  _err:
    if (maj) *maj = -1;
    if (min) *min = -1;
}

EAPI char *
etui_object_title_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_title_get(sd->provider_instance);
}

EAPI char *
etui_object_author_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_author_get(sd->provider_instance);
}

EAPI char *
etui_object_subject_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_subject_get(sd->provider_instance);
}

EAPI char *
etui_object_keywords_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_keywords_get(sd->provider_instance);
}

EAPI char *
etui_object_creator_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_creator_get(sd->provider_instance);
}

EAPI char *
etui_object_producer_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_producer_get(sd->provider_instance);
}

EAPI char *
etui_object_creation_date_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_creation_date_get(sd->provider_instance);
}

EAPI char *
etui_object_modification_date_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_modification_date_get(sd->provider_instance);
}

EAPI Eina_Bool
etui_object_is_printable(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, EINA_FALSE);

    return etui_provider_instance_is_printable(sd->provider_instance);
}

EAPI Eina_Bool
etui_object_is_changeable(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, EINA_FALSE);

    return etui_provider_instance_is_changeable(sd->provider_instance);
}

EAPI Eina_Bool
etui_object_is_copyable(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, EINA_FALSE);

    return etui_provider_instance_is_copyable(sd->provider_instance);
}

EAPI Eina_Bool
etui_object_is_notable(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, EINA_FALSE);

    return etui_provider_instance_is_notable(sd->provider_instance);
}

EAPI Eina_Bool
etui_object_document_password_needed(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, EINA_FALSE);

    return etui_provider_instance_password_needed(sd->provider_instance);
}

EAPI Eina_Bool
etui_object_document_password_set(Evas_Object *obj, const char *password)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, EINA_FALSE);

    return etui_provider_instance_password_set(sd->provider_instance, password);
}

EAPI int
etui_object_document_pages_count(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, -1);

    return etui_provider_instance_pages_count(sd->provider_instance);
}

EAPI const Eina_Array *
etui_object_toc_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_toc_get(sd->provider_instance);
}

EAPI void
etui_object_page_use_display_list_set(Evas_Object *obj, Eina_Bool on)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET(sd, obj, ETUI_OBJ_NAME);

    etui_provider_instance_page_use_display_list_set(sd->provider_instance, on);
}

EAPI Eina_Bool
etui_object_page_use_display_list_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, EINA_FALSE);

    return etui_provider_instance_page_use_display_list_get(sd->provider_instance);
}

EAPI void
etui_object_page_set(Evas_Object *obj, int page_num)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET(sd, obj, ETUI_OBJ_NAME);

    INF("page set %d", page_num);
    if (etui_provider_instance_page_set(sd->provider_instance, page_num))
    {
        etui_provider_instance_page_render_pre(sd->provider_instance);
        evas_object_smart_changed(obj);
    }
}

EAPI int
etui_object_page_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, -1);
    INF("page get");

    return etui_provider_instance_page_get(sd->provider_instance);
}

EAPI void
etui_object_page_size_get(Evas_Object *obj, int *width, int *height)
{
    Etui_Smart_Data *sd;
    char *_etui_smart_str;

    if (!obj)
        goto _err;
    sd = evas_object_smart_data_get(obj);
    if (!sd)
        goto _err;
    _etui_smart_str = (char *)evas_object_type_get(obj);
    if (!_etui_smart_str)
        goto _err;
    if (strcmp(_etui_smart_str, ETUI_OBJ_NAME))
        goto _err;

    etui_provider_instance_page_size_get(sd->provider_instance, width, height);

    return;

  _err:
    if (width) *width = 0;
    if (height) *height = 0;
}

EAPI void
etui_object_page_rotation_set(Evas_Object *obj, Etui_Rotation rotation)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET(sd, obj, ETUI_OBJ_NAME);

    if (etui_provider_instance_page_rotation_set(sd->provider_instance, rotation))
        evas_object_smart_changed(obj);
}

EAPI Etui_Rotation
etui_object_page_rotation_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, ETUI_ROTATION_0);

    return etui_provider_instance_page_rotation_get(sd->provider_instance);
}

EAPI void
etui_object_page_scale_set(Evas_Object *obj, float hscale, float vscale)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET(sd, obj, ETUI_OBJ_NAME);

    printf(" %s 2\n", __FUNCTION__);
    if (etui_provider_instance_page_scale_set(sd->provider_instance, hscale, vscale))
    {
        etui_provider_instance_page_render_pre(sd->provider_instance);
        evas_object_smart_changed(obj);
    }
    printf(" %s 1\n", __FUNCTION__);
}

EAPI void
etui_object_page_scale_get(Evas_Object *obj, float *hscale, float *vscale)
{
    Etui_Smart_Data *sd;
    char *_etui_smart_str;

    if (!obj)
        goto _err;
    sd = evas_object_smart_data_get(obj);
    if (!sd)
        goto _err;
    _etui_smart_str = (char *)evas_object_type_get(obj);
    if (!_etui_smart_str)
        goto _err;
    if (strcmp(_etui_smart_str, ETUI_OBJ_NAME))
        goto _err;

    etui_provider_instance_page_scale_get(sd->provider_instance, hscale, vscale);

    return;

  _err:
    if (hscale) *hscale = 1.0;
    if (vscale) *vscale = 1.0;
}

EAPI void
etui_object_page_dpi_set(Evas_Object *obj, float hdpi, float vdpi)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET(sd, obj, ETUI_OBJ_NAME);

    if (etui_provider_instance_page_dpi_set(sd->provider_instance, hdpi, vdpi))
        evas_object_smart_need_recalculate_set(obj, 1);
}

EAPI void
etui_object_page_dpi_get(Evas_Object *obj, float *hdpi, float *vdpi)
{
    Etui_Smart_Data *sd;
    char *_etui_smart_str;

    if (!obj)
        goto _err;
    sd = evas_object_smart_data_get(obj);
    if (!sd)
        goto _err;
    _etui_smart_str = (char *)evas_object_type_get(obj);
    if (!_etui_smart_str)
        goto _err;
    if (strcmp(_etui_smart_str, ETUI_OBJ_NAME))
        goto _err;

    etui_provider_instance_page_dpi_get(sd->provider_instance, hdpi, vdpi);

    return;

  _err:
    if (hdpi) *hdpi = 72.0;
    if (vdpi) *vdpi = 72.0;
}

EAPI const Eina_Array *
etui_object_page_links_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_page_links_get(sd->provider_instance);
}

EAPI char *
etui_object_page_text_extract(Evas_Object *obj, const Eina_Rectangle *rect)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_page_text_extract(sd->provider_instance, rect);
}

EAPI Eina_Array *
etui_object_page_text_find(Evas_Object *obj, const char *needle)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, NULL);

    return etui_provider_instance_page_text_find(sd->provider_instance, needle);
}

EAPI float
etui_object_page_duration_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, 0.0);

    return etui_provider_instance_page_duration_get(sd->provider_instance);
}

EAPI Etui_Transition
etui_object_page_transition_type_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, ETUI_TRANSITION_NONE);

    return etui_provider_instance_page_transition_type_get(sd->provider_instance);
}

EAPI float
etui_object_page_transition_duration_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, 0.0);

    return etui_provider_instance_page_transition_duration_get(sd->provider_instance);
}

EAPI Eina_Bool
etui_object_page_transition_vertical_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, 0.0);

    return etui_provider_instance_page_transition_vertical_get(sd->provider_instance);
}

EAPI Eina_Bool
etui_object_page_transition_outwards_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, 0.0);

    return etui_provider_instance_page_transition_outwards_get(sd->provider_instance);
}

EAPI int
etui_object_page_transition_direction_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_RETURN(sd, obj, ETUI_OBJ_NAME, 0.0);

    return etui_provider_instance_page_transition_direction_get(sd->provider_instance);
}
