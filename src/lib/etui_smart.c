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

#include "Etui.h"
#include "etui_module.h"
#include "etui_file.h"
#include "etui_private.h"

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
    Etui_Module *module;

    /* private */
    Evas_Object *obj;
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

#define ETUI_SMART_OBJ_GET_ERROR(smart, o, type)          \
{                                                         \
    char *_etui_smart_str;                                \
                                                          \
    if (!o) goto _err;                                    \
    smart = evas_object_smart_data_get(o);                \
    if (!smart) goto _err;                                \
    if (!smart->module)                                   \
    {                                                     \
        ERR("Module unvailable: "                         \
            "etui_objectfile_set() has not been called"); \
        goto _err;                                        \
    }                                                     \
    _etui_smart_str = (char *)evas_object_type_get(o);    \
    if (!_etui_smart_str) goto _err;                      \
    if (strcmp(_etui_smart_str, type)) goto _err;         \
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
        if (sd->obj)
            sd->module->functions->evas_object_del(sd->module->data);
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

    fprintf(stderr, " %s : %dx%d\n", __FUNCTION__, w, h);
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

    if (sd->module->render) ecore_thread_cancel(sd->module->render);
    sd->module->render = ecore_thread_run(_etui_smart_page_render,
                                          _etui_smart_page_render_end,
                                          _etui_smart_page_render_cancel,
                                          sd->module);
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
    Etui_Module *mod;

    mod = data;
    if (!mod)
        return;

    mod->functions->page_render(mod->data);
}

static void
_etui_smart_page_render_end(void *data, Ecore_Thread *thread EINA_UNUSED)
{
    Etui_Module *mod;

    mod = data;
    if (!mod)
        return;

    mod->functions->page_render_end(mod->data);
    mod->render = NULL;
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
    fprintf(stderr, " $$$$ %s : data: %p   obj: %p\n", __FUNCTION__, data, obj);
    fprintf(stderr, " %s : %dx%d\n", __FUNCTION__, w, h);
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
    Evas_Object *obj;

    _etui_smart_init();
    obj = evas_object_smart_add(evas, _etui_smart);
    fprintf(stderr, " $$$$ %s : %p\n", __FUNCTION__, obj);

    return obj;
}

EAPI void
etui_object_file_set(Evas_Object *obj, const Etui_File *ef)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET(sd, obj, ETUI_OBJ_NAME);
    INF("file set");

    sd->module = (Etui_Module *)etui_file_module_get(ef);
    sd->obj = sd->module->functions->evas_object_add(sd->module->data,
                                                     evas_object_evas_get(obj));
    evas_object_repeat_events_set(sd->obj, EINA_TRUE);
    /* evas_object_pass_events_set(sd->obj, EINA_TRUE); */
    /* evas_object_propagate_events_set(sd->obj, EINA_FALSE); */
    evas_object_smart_member_add(sd->obj, obj);
    evas_object_event_callback_add(sd->obj, EVAS_CALLBACK_RESIZE,
                                   _etui_smart_resize_cb, obj);

}

EAPI const char *
etui_object_module_name_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    return sd->module->definition->name;

  _err:
    return NULL;
}

EAPI const void *
etui_object_info_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    return sd->module->functions->info_get(sd->module->data);

  _err:
    return NULL;
}

EAPI const char *
etui_object_title_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    return sd->module->functions->title_get(sd->module->data);

  _err:
    return NULL;
}

EAPI int
etui_object_document_pages_count(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    return sd->module->functions->pages_count(sd->module->data);

  _err:
    return -1;
}

EAPI const Eina_Array *
etui_object_toc_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    return sd->module->functions->toc_get(sd->module->data);

  _err:
    return NULL;
}

EAPI void
etui_object_page_set(Evas_Object *obj, int page_num)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    INF("page set %d", page_num);
    if (sd->module->functions->page_set(sd->module->data, page_num))
    {
        sd->module->functions->page_render_pre(sd->module->data);
        evas_object_smart_changed(obj);
    }

  _err:
    return;
}

EAPI int
etui_object_page_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);
    INF("page get");

    return sd->module->functions->page_get(sd->module->data);

  _err:
    return -1;
}

EAPI void
etui_object_page_size_get(Evas_Object *obj, int *width, int *height)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);
    INF("page size get");

    sd->module->functions->page_size_get(sd->module->data, width, height);

    return;

  _err:
    if (width) *width = 0;
    if (height) *height = 0;
}

EAPI void
etui_object_page_rotation_set(Evas_Object *obj, Etui_Rotation rotation)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    if (sd->module->functions->page_rotation_set(sd->module->data, rotation))
    {
        sd->module->functions->page_render_pre(sd->module->data);
        evas_object_smart_changed(obj);
    }

  _err:
    return;
}

EAPI Etui_Rotation
etui_object_page_rotation_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    return sd->module->functions->page_rotation_get(sd->module->data);

  _err:
    return ETUI_ROTATION_0;
}

EAPI void
etui_object_page_scale_set(Evas_Object *obj, float hscale, float vscale)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    fprintf(stderr, " %s 1\n", __FUNCTION__);
    if (sd->module->functions->page_scale_set(sd->module->data, hscale, vscale))
    {
        sd->module->functions->page_render_pre(sd->module->data);
        evas_object_smart_changed(obj);
    }
    fprintf(stderr, " %s 2\n", __FUNCTION__);

  _err:
    return;
}

EAPI void
etui_object_page_scale_get(Evas_Object *obj, float *hscale, float *vscale)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    sd->module->functions->page_scale_get(sd->module->data, hscale, vscale);

    return;

  _err:
    if (hscale) *hscale = 1.0;
    if (vscale) *vscale = 1.0;
}
