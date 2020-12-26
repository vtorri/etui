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
#include "etui_config.h"
#include "etui_controls.h"
#include "etui_theme.h"
#include "etui_win.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


static void
_cb_mouse_down(void *win,
               Evas *_e EINA_UNUSED,
               Evas_Object *_obj EINA_UNUSED,
               void *event)
{
    Evas_Event_Mouse_Down *ev;
    Etui *etui;

    EINA_SAFETY_ON_NULL_RETURN(win);
    EINA_SAFETY_ON_NULL_RETURN(event);

    etui = evas_object_data_get(win, "etui");
    ev = (Evas_Event_Mouse_Down *)event;

    if (ev->button == 3)
        etui_controls_toggle(win, etui->layout);
}

static void
_cb_win_focus_in(void *data EINA_UNUSED,
                 Evas_Object *win,
                 void *event EINA_UNUSED)
{
    Etui *etui;

    EINA_SAFETY_ON_NULL_RETURN(win);

    fprintf(stderr, "focus in\n");
    fflush(stderr);

    etui = evas_object_data_get(win, "etui");
    if (!etui->focused)
        elm_win_urgent_set(win, EINA_FALSE);
    etui->focused = EINA_TRUE;
    elm_layout_signal_emit(etui->layout, "bg:focus,in", "etui");
}

static void
_cb_win_focus_out(void *data EINA_UNUSED,
                  Evas_Object *win,
                  void *event EINA_UNUSED)
{
    Etui *etui;

    EINA_SAFETY_ON_NULL_RETURN(win);

    fprintf(stderr, "focus out\n");
    fflush(stderr);

    etui = evas_object_data_get(win, "etui");
    etui->focused = EINA_FALSE;
    elm_layout_signal_emit(etui->layout, "bg:focus,out", "etui");
}

static void
_cb_key_down(void *data,
             Evas *evas EINA_UNUSED,
             Evas_Object *obj EINA_UNUSED,
             void *event_info)
{
    Evas_Event_Key_Down *ev;
    Eina_Bool ctrl, alt, shift, win, meta, hyper;

    EINA_SAFETY_ON_NULL_RETURN(event_info);

    ev = (Evas_Event_Key_Down *)event_info;

    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");
    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    win = evas_key_modifier_is_set(ev->modifiers, "Super");
    meta =
        evas_key_modifier_is_set(ev->modifiers, "Meta") ||
        evas_key_modifier_is_set(ev->modifiers, "AltGr") ||
        evas_key_modifier_is_set(ev->modifiers, "ISO_Level3_Shift");
    hyper = evas_key_modifier_is_set(ev->modifiers, "Hyper");

    if (ctrl && !alt && !shift && !win && !meta && !hyper)
    {
        if (!strcmp(ev->keyname, "q"))
        {
            evas_object_del(data);
        }
    }
}

static void
_cb_win_del(void *data EINA_UNUSED,
            Evas *_e EINA_UNUSED,
            Evas_Object *win,
            void *_event EINA_UNUSED)
{
    Etui *etui;

    etui = evas_object_data_get(win, "etui");
    etui_config_del(etui->config);
    evas_object_data_del(win, "etui");
    free(etui);
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

Evas_Object *
etui_win_add(void)
{
    Evas_Object *win;
    Evas_Object *o;
    Etui *etui;

    etui = (Etui *)calloc(1, sizeof(Etui));
    if (!etui)
    {
        ERR(_("could not allocate memory for Etui struct"));
        return NULL;
    }

    etui->config = etui_config_load("config");
    if (!etui->config)
    {
        ERR(_("could not load \"config\" configuration."));
        free(etui);
        return NULL;
    }
    elm_theme_overlay_add(NULL, etui_config_theme_path_default_get(etui->config));
    elm_theme_overlay_add(NULL, etui_config_theme_path_get(etui->config));

    /* main window */
    win = elm_win_add(NULL, PACKAGE_NAME, ELM_WIN_BASIC);
    if (!win)
    {
        ERR(_("could not create elm window."));
        etui_config_del(etui->config);
        free(etui);
        return NULL;
    }

    elm_win_title_set(win, "Etui");
    /* TODO: icon name */
    elm_win_autodel_set(win, EINA_TRUE);
    elm_win_focus_highlight_enabled_set(win, EINA_TRUE);

    evas_object_data_set(win, "etui", etui);

    evas_object_event_callback_add(win, EVAS_CALLBACK_DEL, _cb_win_del, etui);
    evas_object_smart_callback_add(win, "focus,in", _cb_win_focus_in, etui);
    evas_object_smart_callback_add(win, "focus,out", _cb_win_focus_out, etui);

    /* conformant */
    o = elm_conformant_add(win);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_win_resize_object_add(win, o);
    evas_object_show(o);
    etui->conform = o;

    /* gui layout */
    o = elm_layout_add(win);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_content_set(etui->conform, o);
    evas_object_repeat_events_set(o, EINA_FALSE);
    evas_object_show(o);
    etui->layout = o;
    if (!etui_theme_apply(win, "etui"))
    {
        ERR(_("could not apply the theme."));
        _cb_win_del(NULL, NULL, win, NULL);
        return NULL;
    }

    /* dummy button to catch mouse events */
    o = elm_button_add(win);
    elm_object_focus_allow_set(o, EINA_FALSE);
    elm_object_focus_move_policy_set(o, ELM_FOCUS_MOVE_POLICY_CLICK);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(win, o);
    evas_object_color_set(o, 0, 0, 0, 0);
    evas_object_repeat_events_set(o, EINA_TRUE);
    evas_object_show(o);
    evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_DOWN,
                                   _cb_mouse_down, win);
    etui->event_mouse = o;

    /* dummy button to catch keyboard events */
    o = elm_button_add(win);
    elm_object_focus_highlight_style_set(o, "blank");
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(win, o);
    evas_object_lower(o);
    evas_object_show(o);
    elm_object_focus_set(o, EINA_TRUE);
    evas_object_event_callback_add(o, EVAS_CALLBACK_KEY_DOWN,
                                   _cb_key_down, win);
    etui->event_kbd = o;

    return win;
}
