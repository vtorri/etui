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

#include <config.h>

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
    Evas_Object *frame;
    Evas_Object *obj;
    /* mode */
    Etui_Mode mode;
};

static Evas_Smart *_etui_smart = NULL;

static void _etui_smart_page_render(void *data, Ecore_Thread *thread);
static void _etui_smart_page_render_end(void *data, Ecore_Thread *thread);
static void _etui_smart_page_render_cancel(void *data, Ecore_Thread *thread);
static void _etui_smart_page_eval(Etui_Smart_Data *sd);

/* internal smart object routines */

#define ETUI_SMART_OBJ_GET(smart, o, type)             \
{                                                      \
    char *_etui_smart_str;                             \
                                                       \
    if (!o) goto _err;                                 \
    smart = evas_object_smart_data_get(o);             \
    if (!smart) goto _err;                             \
    _etui_smart_str = (char *)evas_object_type_get(o); \
    if (!_etui_smart_str) goto _err;                   \
    if (strcmp(_etui_smart_str, type)) goto _err;      \
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
    Evas_Object *frame;

    sd = calloc(1, sizeof(Etui_Smart_Data));
    EINA_SAFETY_ON_NULL_RETURN(sd);

    EINA_REFCOUNT_INIT(sd);

    frame = evas_object_rectangle_add(evas_object_evas_get(obj));
    evas_object_smart_member_add(frame, obj);
    evas_object_color_set(frame, 255, 255, 255, 255);
    evas_object_show(frame);
    sd->frame = frame;

    sd->mode = ETUI_MODE_FREE;
    evas_object_smart_data_set(obj, sd);
}

static void
_etui_smart_del(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    EINA_SAFETY_ON_NULL_RETURN(sd);

    EINA_REFCOUNT_UNREF(sd)
    {
        if (sd->module->render)
            ecore_thread_cancel(sd->module->render);
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
    EINA_SAFETY_ON_NULL_RETURN(sd);
    /* And here we go to be more smarter */
    evas_object_move(sd->frame, x, y);
    _etui_smart_page_eval(sd);
}

static void
_etui_smart_resize(Evas_Object *obj, Evas_Coord w, Evas_Coord h)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    EINA_SAFETY_ON_NULL_RETURN(sd);

    /* FIXME: should do something here ? */
    /* YEAH now we do something */
    evas_object_resize(sd->frame, w, h);
    _etui_smart_page_eval(sd);

    fprintf(stderr, " %s 2 : %dx%d\n", __FUNCTION__, w, h);
}

static void
_etui_smart_show(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    EINA_SAFETY_ON_NULL_RETURN(sd);
    evas_object_show(sd->obj);

}

static void
_etui_smart_hide(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    EINA_SAFETY_ON_NULL_RETURN(sd);
    evas_object_hide(sd->obj);
}

static void
_etui_smart_color_set(Evas_Object *obj, int r, int g, int b, int a)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    EINA_SAFETY_ON_NULL_RETURN(sd);
    evas_object_color_set(sd->obj, r, g, b, a);
}

static void
_etui_smart_clip_set(Evas_Object *obj, Evas_Object *clip)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    EINA_SAFETY_ON_NULL_RETURN(sd);
    evas_object_clip_set(sd->frame, clip);
}

static void
_etui_smart_clip_unset(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    EINA_SAFETY_ON_NULL_RETURN(sd);
    evas_object_clip_unset(sd->obj);
}


static void
_etui_smart_calculate(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    sd = evas_object_smart_data_get(obj);
    EINA_SAFETY_ON_NULL_RETURN(sd);
    if (!sd->module->render)
      sd->module->render = ecore_thread_run(_etui_smart_page_render,
                                            _etui_smart_page_render_end,
                                            _etui_smart_page_render_cancel,
                                            sd);
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

    sd = data;
    if (!sd)
        return;

    sd->module->functions->page_render(sd->module->data);
}

static void
_etui_smart_page_render_end(void *data, Ecore_Thread *thread EINA_UNUSED)
{
    Etui_Smart_Data *sd;

    sd = data;
    if (!sd)
        return;

    sd->module->functions->page_render_end(sd->module->data);
    sd->module->render = NULL;
    _etui_smart_page_eval(sd);
}

static void
_etui_smart_page_render_cancel(void *data EINA_UNUSED, Ecore_Thread *thread EINA_UNUSED)
{

}
#if 0
static void
_etui_smart_resize_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
    Evas_Coord w;
    Evas_Coord h;

//    evas_object_geometry_get(obj, NULL, NULL, &w, &h);
//    evas_object_resize((Evas_Object *)data, w, h);
    _etui_smart_page_eval(data);
    fprintf(stderr, " $$$$ %s : data: %p   obj: %p  %dx%d\n", __FUNCTION__, data, obj, w, h);
}
#endif


static void
_etui_smart_page_eval(Etui_Smart_Data *sd)
{
   Evas_Coord x, y, w, h;
   Evas_Coord ox, oy, ow, oh;
   double scale = 1.0;

   evas_object_geometry_get(sd->frame, &x, &y, &w, &h);
   sd->module->functions->page_size_get(sd->module->data, &ow, &oh);
   switch (sd->mode)
     {
      case ETUI_MODE_FIT_WIDTH:
         if (ow) scale = w / (double)ow;
         ow = w;
         oh = scale * oh;
         break;
      case ETUI_MODE_FIT_HEIGHT:
         if (oh) scale = h / (double)oh;
         oh = h;
         ow = scale * ow;
         break;
      case ETUI_MODE_FIT_AUTO:
         if (((float)w / (float)h) > ((float)ow/(float)oh))
           {
              printf("VVVVVVVVVVVVV\n");
              if (oh) scale = h / (double)oh;
              oh = h;
              ow = scale * ow;
           }
         else
           {
              printf("HHHHHHHHHHHH\n");
              if (ow) scale = w / (double)ow;
              ow = w;
              oh = scale * oh;
           }
         break;
      default:
      case ETUI_MODE_FREE:
         scale = sd->module->functions->page_scale_get(sd->module->data);
         ow = scale * ow;
         oh = scale * oh;
         break;
     }
   ox = ((w - ow) / 2.0) + (double)x + 0.5;
   oy = ((h - oh) / 2.0) + (double)y + 0.5;
   fprintf(stderr, "EVAL FRAME(%d, %d, %d, %d), OBJ(%d, %d, %d, %d) scaling %f\n",
           x, y, w, h, ox, oy, ow, oh, scale);
   // sd->module->functions->page_scale_set(sd->module->data, scale);
   evas_object_move(sd->obj, ox, oy);
   evas_object_resize(sd->obj, ow, oh);
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
    evas_object_smart_member_add(sd->obj, obj);
    evas_object_clip_set(sd->obj, sd->frame);
    /*
    evas_object_event_callback_add(sd->obj, EVAS_CALLBACK_RESIZE,
                                   _etui_smart_resize_cb, sd);
                                   */
_err:
    return;

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
etui_object_page_scale_set(Evas_Object *obj, double scale)
{
    Etui_Smart_Data *sd;
    Evas_Coord w, h;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    fprintf(stderr, " %s 1\n", __FUNCTION__);
    if (sd->module->functions->page_scale_set(sd->module->data, scale))
    {
        sd->module->functions->page_render_pre(sd->module->data);
        _etui_smart_page_eval(sd);
        evas_object_geometry_get(sd->obj, NULL, NULL, &w, &h);
        switch (sd->mode)
        {
           case ETUI_MODE_FREE:
              evas_object_size_hint_min_set(obj, w, h);
              break;
           case ETUI_MODE_FIT_WIDTH:
              evas_object_size_hint_min_set(obj, 0, h);
              break;
           case ETUI_MODE_FIT_HEIGHT:
              evas_object_size_hint_min_set(obj, w, 0);
              break;
           case ETUI_MODE_FIT_AUTO:
           case ETUI_MODE_UNKNOWN:
              evas_object_size_hint_min_set(obj, 0, 0);
              break;
        }
        evas_object_smart_changed(obj);
    }
    fprintf(stderr, " %s 2\n", __FUNCTION__);

  _err:
    return;
}

EAPI double
etui_object_page_scale_get(const Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    return sd->module->functions->page_scale_get(sd->module->data);

  _err:
    return -1.0;
}

EAPI void
etui_object_page_mode_set(Evas_Object *obj, Etui_Mode mode)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET(sd, obj, ETUI_OBJ_NAME);

    fprintf(stderr, "Mode set %d\n", mode);
    if (sd->mode == mode) return;
    sd->mode = mode;

  _err:
    return;
}

EAPI Etui_Mode
etui_object_page_mode_get(const Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET(sd, obj, ETUI_OBJ_NAME);

    return sd->mode;

  _err:
    return ETUI_MODE_UNKNOWN;
}


EAPI const void *
etui_object_api_get(Evas_Object *obj)
{
    Etui_Smart_Data *sd;

    ETUI_SMART_OBJ_GET_ERROR(sd, obj, ETUI_OBJ_NAME);

    return sd->module->functions->api_get(sd->module->data);

  _err:
    return NULL;
}
