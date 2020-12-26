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

#include <config.h>

#include <Elementary.h>

#include "etui_private.h"
#include "etui_settings.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


typedef enum
{
    SETTINGS_NONE,
    SETTINGS_THEME,
    SETTINGS_BEHAVIOR,
    SETTINGS_KEYS,
    SETTINGS_ELM
} Settings_Mode;

static Evas_Object *saved_win = NULL;
static Evas_Object *saved_bg = NULL;
static Evas_Object *st_frame = NULL; // the frame containing the toolbar
static Evas_Object *st_box = NULL; // box for layout in the frame
static Evas_Object *st_tbox = NULL; // box for layout in the frame
static Evas_Object *st_stbox = NULL; // box containing the selected setting
static Evas_Object *st_toolbar = NULL; // for the layout of the buttons
static Evas_Object *st_over = NULL; // rectangle over panel to avoid hiding it
static Ecore_Timer *st_del_timer = NULL; // delete objects after timeout
static Settings_Mode st_mode = SETTINGS_NONE;
static Eina_Bool st_out = EINA_FALSE; // is settings displayed ?

static void
_cb_op(void *data,
       Evas_Object *_obj EINA_UNUSED,
       void *_event EINA_UNUSED)
{
   Settings_Mode mode = (intptr_t)data;

   if (st_mode == mode)
       return;

   st_mode = mode;
   elm_object_signal_emit(saved_bg, "fg:stdetails,hide", "etui");
}

static void
_etui_st_cb(void *data,
            Evas_Object *_obj EINA_UNUSED,
            void *_event EINA_UNUSED)
{
    Settings_Mode mode;

    mode = (int)(intptr_t)data;

    if (st_mode == mode)
        return;

    st_mode = mode;
    elm_object_signal_emit(saved_bg, "fg:stdetails,hide", "etui");
}

static void
_etui_mouse_down_cb(void *data EINA_UNUSED,
                    Evas *_e EINA_UNUSED,
                    Evas_Object *_obj EINA_UNUSED,
                    void *_ev EINA_UNUSED)
{
    etui_settings_toggle(saved_win, saved_bg);
}

static Eina_Bool
_etui_st_del_delay_cb(void *_data EINA_UNUSED)
{
    evas_object_del(st_stbox);
    evas_object_del(st_frame);
    /* options_theme_clear(); */
    /* FIXME: clear if more */
    st_stbox = NULL;
    st_frame = NULL;
    st_del_timer = NULL;
    elm_cache_all_flush();

    return EINA_FALSE;
}

static void
_etui_stdt_hide_done_cb(void *data,
                        Evas_Object *_obj EINA_UNUSED,
                        const char *_sig EINA_UNUSED,
                        const char *_src EINA_UNUSED)
{
    elm_box_clear(st_stbox);
    switch (st_mode)
    {
        case SETTINGS_NONE: break;
        case SETTINGS_THEME: /* etui_settings_elm(st_stbox, data); */ break;
        case SETTINGS_BEHAVIOR:  etui_settings_behavior(st_stbox, data); break;
        case SETTINGS_KEYS: /* options_keys(st_stbox, data); */ break;
        case SETTINGS_ELM: etui_settings_elm(st_stbox, data); break;
    }

    elm_object_signal_emit(saved_bg, "fg:stdetails,show", "etui");
}

static void
_etui_stdt_hide_done2_cb(void *_data EINA_UNUSED,
                         Evas_Object *_obj EINA_UNUSED,
                         const char *_sig EINA_UNUSED,
                         const char *_src EINA_UNUSED)
{
    if (st_del_timer)
    {
        ecore_timer_del(st_del_timer);
        st_del_timer = NULL;
    }
    _etui_st_del_delay_cb(NULL);
    elm_object_signal_callback_del(saved_bg, "stdetails,hide,done",
                                   "fg:etui",
                                   _etui_stdt_hide_done2_cb);
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


Eina_Bool
etui_settings_active_get(void)
{
    return st_out;
}

void
etui_settings_toggle(Evas_Object *win, Evas_Object *bg)
{
    Evas_Object *o;

    st_mode = SETTINGS_NONE;

    if (!st_frame)
    {
        Elm_Object_Item *item;

        /* FIXME: config */

        saved_win = win;
        saved_bg = bg;

        o = elm_box_add(win);
        evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
        elm_object_part_content_set(bg, "fg:etui.stdetails", o);
        evas_object_show(o);
        st_stbox = o;

        o = elm_frame_add(win);
        evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
        elm_object_text_set(o, "Settings");
        st_frame = o;

        o = elm_box_add(win);
        elm_box_horizontal_set(o, EINA_TRUE);
        elm_object_content_set(st_frame, o);
        evas_object_show(o);
        st_box = o;

        o = elm_box_add(win);
        evas_object_size_hint_weight_set(o, 0.0, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(o, 1.0, EVAS_HINT_FILL);
        elm_box_pack_end(st_box, o);
        evas_object_show(o);
        st_tbox = o;

        o = elm_toolbar_add(win);
        evas_object_size_hint_weight_set(o, 0.0, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(o, 0.5, EVAS_HINT_FILL);
        elm_toolbar_horizontal_set(o, EINA_FALSE);
        elm_object_style_set(o, "item_horizontal");
        elm_toolbar_icon_size_set(o, 16 * elm_config_scale_get());
        elm_toolbar_shrink_mode_set(o, ELM_TOOLBAR_SHRINK_SCROLL);
        elm_toolbar_select_mode_set(o, ELM_OBJECT_SELECT_MODE_ALWAYS);
        elm_toolbar_menu_parent_set(o, win);
        elm_toolbar_homogeneous_set(o, EINA_FALSE);
        st_toolbar = o;

#define ITEM_APPEND(_icon_name, _name, _option_mode) \
        elm_toolbar_item_append(o, _icon_name, _name, _cb_op, \
                                (void*) SETTINGS_##_option_mode)

        item = ITEM_APPEND("preferences-desktop-theme", _("Theme"), THEME);
        ITEM_APPEND("preferences-system", _("Behavior"), BEHAVIOR);
        ITEM_APPEND("preferences-desktop-keyboard-shortcuts", _("Keys"), KEYS);
        ITEM_APPEND("preferences-color", _("Toolkit"), ELM);
#undef ITEM_APPEND

        elm_box_pack_end(st_tbox, o);
        evas_object_show(o);

        elm_toolbar_item_selected_set(item, EINA_TRUE);

        elm_object_part_content_set(bg, "fg:etui.settings", st_frame);
        evas_object_show(st_frame);
    }
    else if ((st_stbox) && (!st_out))
    {
        elm_object_part_content_set(bg, "fg:etui.stdetails", st_stbox);
        elm_object_part_content_set(bg, "fg:etui.settings", st_frame);
        elm_object_signal_emit(bg, "fg:stdetails,show", "etui");
        elm_object_signal_emit(bg, "fg:settings,show", "etui");
    }

    if (!st_out)
    {
        elm_object_signal_callback_add(bg, "stdetails,hide,done",
                                       "fg:etui",
                                       _etui_stdt_hide_done_cb,
                                       win);
        o = evas_object_rectangle_add(evas_object_evas_get(win));
        evas_object_color_set(o, 0, 0, 0, 0);
        elm_object_part_content_set(bg, "fg:etui.dismiss", o);
        evas_object_show(o);
        evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_DOWN,
                                       _etui_mouse_down_cb, NULL);
        st_over = o;

        elm_object_signal_emit(bg, "fg:settings,show", "etui");
        st_out = EINA_TRUE;
        /* FIXME: callback */
        elm_object_focus_set(st_toolbar, EINA_TRUE);
        if (st_del_timer)
        {
            ecore_timer_del(st_del_timer);
            st_del_timer = NULL;
        }
    }
    else
    {
        elm_object_signal_callback_del(bg, "stdetails,hide,done",
                                       "fg:etui",
                                       _etui_stdt_hide_done_cb);
        elm_object_signal_callback_add(bg, "stdetails,hide,done",
                                       "fg:etui",
                                       _etui_stdt_hide_done2_cb, NULL);
        elm_object_focus_set(st_frame, EINA_FALSE);
        elm_object_focus_set(st_stbox, EINA_FALSE);
        elm_object_focus_set(st_toolbar, EINA_FALSE);
        /* FIXME: callback */
        evas_object_del(st_over);
        st_over = NULL;
        elm_object_signal_emit(bg, "fg:settings,hide", "etui");
        elm_object_signal_emit(bg, "fg:stdetails,hide", "etui");
        st_out = EINA_FALSE;
        if (st_del_timer) ecore_timer_del(st_del_timer);
        st_del_timer = ecore_timer_add(10.0, _etui_st_del_delay_cb, NULL);
    }
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
