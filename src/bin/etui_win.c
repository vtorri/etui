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
#include "etui_main.h"
#include "etui_controls.h"
#include "etui_theme.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


static void
_etui_win_focus_in_cb(void *data,
                      Evas_Object *obj EINA_UNUSED,
                      void *event EINA_UNUSED)
{
    Etui *etui = (Etui *)data;

    fprintf(stderr, "focus in\n");
    if (!etui->window.focused)
        elm_win_urgent_set(etui->window.win, EINA_FALSE);
    etui->window.focused = EINA_TRUE;
    elm_layout_signal_emit(etui->window.base, "bg:focus,in", "etui");
}

static void
_etui_win_focus_out_cb(void *data,
                       Evas_Object *obj EINA_UNUSED, void *event EINA_UNUSED)
{
    Etui *etui = (Etui *)data;

    fprintf(stderr, "focus out\n");
    etui->window.focused = EINA_FALSE;
    elm_layout_signal_emit(etui->window.base, "bg:focus,out", "etui");
}

static void
_etui_mouse_down_cb(void *data,
                    Evas *_e EINA_UNUSED,
                    Evas_Object *_obj EINA_UNUSED,
                    void *event)
{
    Evas_Event_Mouse_Down *ev;
    Etui *etui;

    EINA_SAFETY_ON_NULL_RETURN(data);
    EINA_SAFETY_ON_NULL_RETURN(event);

    etui = (Etui *)data;
    ev = (Evas_Event_Mouse_Down *)event;

    if (ev->button == 3)
        etui_controls_toggle(etui->window.win, etui->window.base);
}

static void
_etui_win_del_cb(void *data,
                 Evas *_e EINA_UNUSED,
                 Evas_Object *_obj EINA_UNUSED,
                 void *_event EINA_UNUSED)
{
    etui_win_free(data);
 }


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


Eina_Bool
etui_win_new(Etui *etui, const char *role,
             Eina_Bool fullscreen, Etui_Config *config)
{
    Evas_Object *o;

    /* all pointers and values are valid */

    /* main window */
    o = elm_win_add(NULL, PACKAGE_NAME, ELM_WIN_BASIC);
    elm_win_title_set(o, "Etui");
    /* TODO: icon name */
    if (role)
        elm_win_role_set(o, role);
    if (fullscreen)
        elm_win_fullscreen_set(o, EINA_TRUE);
    elm_win_focus_highlight_enabled_set(o, EINA_TRUE);
    elm_win_autodel_set(o, EINA_TRUE);

    etui->window.win = o;

    evas_object_data_set(etui->window.win, "etui", etui);

    evas_object_event_callback_add(o, EVAS_CALLBACK_DEL, _etui_win_del_cb, etui);

    evas_object_smart_callback_add(o, "focus,in", _etui_win_focus_in_cb, etui);
    evas_object_smart_callback_add(o, "focus,out", _etui_win_focus_out_cb, etui);

    /* conformant */
    o = elm_conformant_add(etui->window.win);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_win_resize_object_add(etui->window.win, o);
    evas_object_show(o);
    etui->window.conform = o;

    /* gui */
    o = elm_layout_add(etui->window.win);
    if (!etui_theme_apply(o, etui, "etui"))
    {
        etui_win_free(etui);
        return EINA_FALSE;
    }
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_content_set(etui->window.conform, o);
    evas_object_repeat_events_set(o, EINA_FALSE);
    evas_object_show(o);
    etui->window.base = o;

    /* event catcher */
    o = evas_object_rectangle_add(evas_object_evas_get(etui->window.win));
    evas_object_color_set(o, 0, 0, 0, 0);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_win_resize_object_add(etui->window.win, o);
    evas_object_repeat_events_set(o, EINA_TRUE);
    evas_object_show(o);
    etui->window.event = o;

    evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_DOWN,
                                   _etui_mouse_down_cb, etui);

    etui->window.config = config;

    return EINA_TRUE;
}

void
etui_win_free(Etui *etui)
{
    if (etui->window.panel)
    {
        evas_object_del(etui->window.panel);
        etui->window.panel = NULL;
    }

    if (etui->window.base)
    {
        evas_object_del(etui->window.base);
        etui->window.base = NULL;
    }

    if (etui->window.conform)
    {
        evas_object_del(etui->window.conform);
        etui->window.conform = NULL;
    }

    if (etui->window.win)
    {
        evas_object_event_callback_del_full(etui->window.win, EVAS_CALLBACK_DEL,
                                            _etui_win_del_cb, etui);
        evas_object_del(etui->window.win);
        etui->window.win = NULL;
    }
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
