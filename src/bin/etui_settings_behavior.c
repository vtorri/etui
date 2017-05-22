/* Etui - Multi-document rendering application using the EFL
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

#include <Elementary.h>

#include "etui_private.h"
#include "etui_config.h"
#include "etui_main.h"
#include "etui_settings.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


static Evas_Object *op_w;
static Evas_Object *op_h;

#define CB(_cfg_name, _inv) \
static void \
_etui_settings_behavior_##_cfg_name##_cb(void *data, Evas_Object *obj, \
                                         void *_event EINA_UNUSED) \
{ \
   Etui_Config *config = ((Etui *)data)->window.config; \
   if (_inv) \
     config->_cfg_name = !elm_check_state_get(obj); \
   else \
     config->_cfg_name = elm_check_state_get(obj); \
   /* termio_config_update(term); */ \
   /* windows_update(); */ \
   etui_config_save(config, NULL); \
}

CB(disable_visual_bell, 1);
CB(bell_rings, 0);

static void
_etui_settings_behavior_custom_geometry_cb(void *data,
                                           Evas_Object *obj,
                                           void *_event EINA_UNUSED)
{
    Etui_Config *config;

    config = ((Etui *)data)->window.config;
    config->custom_geometry = elm_check_state_get(obj);
    if (config->custom_geometry)
    {
        config->cg_width = (int)elm_spinner_value_get(op_w);
        config->cg_height = (int)elm_spinner_value_get(op_h);
    }
    etui_config_save(config, NULL);

    elm_object_disabled_set(op_w, !config->custom_geometry);
    elm_object_disabled_set(op_h, !config->custom_geometry);
}

static void
_etui_settings_behavior_cg_width_cb(void *data,
                                    Evas_Object *obj,
                                    void *_event EINA_UNUSED)
{
    Etui_Config *config;

    config = ((Etui *)data)->window.config;
    if (config->custom_geometry)
    {
        config->cg_width = (int) elm_spinner_value_get(obj);
        etui_config_save(config, NULL);
    }
}

static void
_etui_settings_behavior_cg_height_cb(void *data,
                                     Evas_Object *obj,
                                     void *_event EINA_UNUSED)
{
    Etui_Config *config;

    config = ((Etui *)data)->window.config;
    if (config->custom_geometry)
    {
        config->cg_height = (int) elm_spinner_value_get(obj);
        etui_config_save(config, NULL);
    }
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


void
etui_settings_behavior(Evas_Object *stbox, Etui *etui)
{
    Etui_Config *config;
    Evas_Object *fr;
    Evas_Object *sc;
    Evas_Object *bx;
    Evas_Object *o;
    Evas_Coord w;
    Evas_Coord h;

    config = etui->window.config;

    /* frame */
    o = elm_frame_add(stbox);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_text_set(o, "Behavior");
    elm_box_pack_end(stbox, o);
    evas_object_show(o);
    fr = o;

    /* scroller */
    o = elm_scroller_add(stbox);
    elm_scroller_content_min_limit(o, EINA_TRUE, EINA_FALSE);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_content_set(fr, o);
    evas_object_show(o);
    sc = o;

    /* box */
    o = elm_box_add(stbox);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.0);
    elm_object_content_set(sc, o);
    evas_object_show(o);
    bx = o;

#define CX(_lbl, _cfg_name, _inv) \
    o = elm_check_add(bx); \
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0); \
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.5); \
    elm_object_text_set(o, _lbl); \
    elm_check_state_set(o, _inv ? !config->_cfg_name : config->_cfg_name); \
    elm_box_pack_end(bx, o); \
    evas_object_show(o); \
    evas_object_smart_callback_add(o, "changed", \
                                   _etui_settings_behavior_##_cfg_name##_cb, etui)

    CX(("Visual Bell"), disable_visual_bell, 1);
    CX(("Bell rings"), bell_rings, 0);

#undef CX

    evas_object_geometry_get(etui->window.win, NULL, NULL, &w, &h);

   o = elm_check_add(bx);
   evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.5);
   elm_object_text_set(o, ("Always open at size:"));
   elm_check_state_set(o, config->custom_geometry);
   elm_box_pack_end(bx, o);
   evas_object_show(o);
   evas_object_smart_callback_add(o, "changed",
                                  _etui_settings_behavior_custom_geometry_cb, etui);

   o = elm_label_add(bx);
   evas_object_size_hint_weight_set(o, 0.0, 0.0);
   evas_object_size_hint_align_set(o, 0.0, 0.5);
   elm_object_text_set(o, ("Width:"));
   elm_box_pack_end(bx, o);
   evas_object_show(o);

   op_w = o = elm_spinner_add(bx);
   elm_spinner_editable_set(o, EINA_TRUE);
   elm_spinner_min_max_set(o, 2.0, 350.0);
   evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.5);
   if (config->custom_geometry)
       elm_spinner_value_set(o, config->cg_width);
   else
       elm_spinner_value_set(o, w);
   elm_object_disabled_set(o, !config->custom_geometry);
   elm_box_pack_end(bx, o);
   evas_object_show(o);
   evas_object_smart_callback_add(o, "changed",
                                  _etui_settings_behavior_cg_width_cb, etui);

   o = elm_label_add(bx);
   evas_object_size_hint_weight_set(o, 0.0, 0.0);
   evas_object_size_hint_align_set(o, 0.0, 0.5);
   elm_object_text_set(o, ("Height:"));
   elm_box_pack_end(bx, o);
   evas_object_show(o);

   op_h = o = elm_spinner_add(bx);
   elm_spinner_editable_set(o, EINA_TRUE);
   elm_spinner_min_max_set(o, 1.0, 150.0);
   evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.5);
   if (config->custom_geometry)
       elm_spinner_value_set(o, config->cg_height);
   else
       elm_spinner_value_set(o, h);
   elm_object_disabled_set(o, !config->custom_geometry);
   elm_box_pack_end(bx, o);
   evas_object_show(o);
   evas_object_smart_callback_add(o, "changed",
                                  _etui_settings_behavior_cg_height_cb, etui);

   evas_object_size_hint_weight_set(stbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(stbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
