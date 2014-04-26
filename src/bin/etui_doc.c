/* Etui - Multi-document rendering application using the EFL
 * Copyright (C) 2013-2014 Vincent Torri
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
#include <Etui.h>

#include "etui_private.h"
#include "etui_theme.h"
#include "etui_win.h"
#include "etui_doc.h"

static void
_etui_doc_zoom(const Etui *etui)
{
    int w;
    int h;

    etui_object_page_scale_set(etui->doc.doc, etui->doc.scale, etui->doc.scale);
    evas_object_geometry_get(etui->doc.doc, NULL, NULL, &w, &h);
    evas_object_size_hint_min_set(etui->doc.doc, w, h);
    evas_object_size_hint_max_set(etui->doc.doc, w, h);
}

static void
_etui_doc_fullscreen_set(Etui *etui, Eina_Bool on)
{
    Evas_Coord win_w;
    Evas_Coord win_h;
    int screen_w;
    int screen_h;
    int w;
    int h;

    if (on)
    {
        float scale;

        evas_object_geometry_get(etui->window.win,
                                 NULL, NULL, &win_w, &win_h);
        etui->window.win_w = win_w;
        etui->window.win_h = win_h;

        elm_win_screen_size_get(etui->window.win, NULL, NULL,
                                &screen_w, &screen_h);
        etui_object_page_size_get(etui->doc.doc, &w, &h);

        if (screen_w > screen_h)
        {
            if (w < h)
            {
                scale = (float)(screen_h) / (float)h;
                etui_object_page_scale_set(etui->doc.doc, scale, scale);
                evas_object_geometry_get(etui->doc.doc, NULL, NULL, &w, &h);
                evas_object_size_hint_min_set(etui->doc.doc, w, h);
                evas_object_size_hint_max_set(etui->doc.doc, w, h);
            }
            else
            {
                scale = (float)(screen_w) / (float)w;
                etui_object_page_scale_set(etui->doc.doc, scale, scale);
                evas_object_geometry_get(etui->doc.doc, NULL, NULL, &w, &h);
                evas_object_size_hint_min_set(etui->doc.doc, w, h);
                evas_object_size_hint_max_set(etui->doc.doc, w, h);
            }
        }
        else
        {
            /* something to do ? */
        }

        elm_win_fullscreen_set(etui->window.win, EINA_TRUE);

        evas_object_size_hint_min_set(etui->window.bg, screen_w, screen_h);
        evas_object_resize(etui->window.win, screen_w, screen_h);
        elm_scroller_policy_set(etui->doc.sc, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
    }
    else
    {
        elm_win_fullscreen_set(etui->window.win, EINA_FALSE);
        evas_object_size_hint_min_set(etui->window.bg,
                                      etui->window.win_w, etui->window.win_h);
        evas_object_resize(etui->window.win,
                           etui->window.win_w, etui->window.win_h);
        elm_scroller_policy_set(etui->doc.sc,
                                ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
        etui_object_page_scale_set(etui->doc.doc,
                                   etui->doc.scale, etui->doc.scale);
        evas_object_geometry_get(etui->doc.doc, NULL, NULL, &w, &h);
        evas_object_size_hint_min_set(etui->doc.doc, w, h);
        evas_object_size_hint_max_set(etui->doc.doc, w, h);
    }
}

static Eina_Bool
_etui_doc_key_down_global_cb(void *data, int type EINA_UNUSED, void *event)
{
    Ecore_Event_Key *ev;
    Etui *etui;
    unsigned int alt;
    unsigned int shift;
    unsigned int ctrl;

    ev = (Ecore_Event_Key *)event;
    etui = (Etui *)data;

    alt = ev->modifiers & ECORE_EVENT_MODIFIER_ALT;
    shift = ev->modifiers & ECORE_EVENT_MODIFIER_SHIFT;
    ctrl = ev->modifiers & ECORE_EVENT_MODIFIER_CTRL;

    if ((!alt) && (ctrl) && (!shift))
    {
        if (!strcmp(ev->key, "q"))
        {
            etui_win_free(etui);
        }
        else if (!strcmp(ev->key, "o"))
        {
            /* FIXME: open file shortcut */
        }
        else if (!strcmp(ev->key, "F1"))
        {
            /* FIXME: help shortcut */
        }
    }

    return ECORE_CALLBACK_PASS_ON;
}

static void
_etui_doc_key_down_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event)
{
    Evas_Event_Key_Down *ev;
    Etui *etui;
    int alt;
    int shift;
    int ctrl;

    ev = (Evas_Event_Key_Down *)event;
    etui = (Etui *)data;

    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");

    printf("keyname: %s  key: %s  string: %s\n",
           ev->keyname, ev->key, ev->string);

    if ((!alt) && (!ctrl) && (!shift))
    {
        if (!strcmp(ev->key, "Right") || !strcmp(ev->key, "Down") || !strcmp(ev->key, "space"))
        {
            etui_object_page_set(etui->doc.doc, etui_object_page_get(etui->doc.doc) + 1);
        }
        else if (!strcmp(ev->key, "Left") || !strcmp(ev->key, "Up") || !strcmp(ev->key, "BackSpace"))
        {
            etui_object_page_set(etui->doc.doc, etui_object_page_get(etui->doc.doc) - 1);
        }
        else if (!strcmp(ev->key, "Next"))
        {
            etui_object_page_set(etui->doc.doc, etui_object_page_get(etui->doc.doc) + 10);
        }
        else if (!strcmp(ev->key, "Prior"))
        {
            etui_object_page_set(etui->doc.doc, etui_object_page_get(etui->doc.doc) - 10);
        }
        else if (!strcmp(ev->key, "F11"))
        {
            _etui_doc_fullscreen_set(etui,
                                     !elm_win_fullscreen_get(etui->window.win));

        }
        else if (!strcmp(ev->key, "Escape"))
        {
            if (elm_win_fullscreen_get(etui->window.win))
                _etui_doc_fullscreen_set(etui, EINA_FALSE);
        }
    }

    if ((!alt) && (ctrl) && (!shift))
    {
        if (!strcmp(ev->key, "KP_Add"))
        {
            etui->doc.scale *= M_SQRT2;
            _etui_doc_zoom(etui);
        }
        else if (!strcmp(ev->key, "KP_Subtract"))
        {
            etui->doc.scale *= M_SQRT1_2;
            _etui_doc_zoom(etui);
        }
        else if (!strcmp(ev->key, "KP_0"))
        {
            Evas_Coord win_h;
            int h;

            /* zoom to fit height */
            evas_object_geometry_get(etui->window.win, NULL, NULL, NULL, &win_h);
            etui_object_page_size_get(etui->doc.doc, NULL, &h);
            etui->doc.scale = (float)win_h / (float)h;
            _etui_doc_zoom(etui);
        }
        else if (!strcmp(ev->key, "KP_1"))
        {
            /* zoom to 100% */
            etui->doc.scale = 1.0f;
            _etui_doc_zoom(etui);
        }
        else if (!strcmp(ev->key, "KP_2"))
        {
            Evas_Coord win_w;
            int w;

            /* zoom to fit width */
            evas_object_geometry_get(etui->window.win, NULL, NULL, &win_w, NULL);
            etui_object_page_size_get(etui->doc.doc, &w, NULL);
            etui->doc.scale = (float)win_w / (float)w;
            _etui_doc_zoom(etui);
        }
        else if (!strcmp(ev->key, "Home"))
        {
            etui_object_page_set(etui->doc.doc, 0);
        }
        else if (!strcmp(ev->key, "End"))
        {
            etui_object_page_set(etui->doc.doc, etui_object_document_pages_count(etui->doc.doc) - 1);
        }
    }
}

static void
_etui_doc_mouse_down_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event)
{
    Evas_Event_Mouse_Down *ev;
    Etui *etui;
    int alt;
    int shift;
    int ctrl;

    ev = (Evas_Event_Mouse_Down *)event;
    etui = (Etui *)data;

    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");

    if ((!alt) && (!ctrl) && (!shift))
    {
        if (ev->flags & (EVAS_BUTTON_DOUBLE_CLICK | EVAS_BUTTON_TRIPLE_CLICK))
        {
            _etui_doc_fullscreen_set(etui,
                                     !elm_win_fullscreen_get(etui->window.win));
        }
    }
}

static void
_etui_doc_mouse_wheel_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event)
{
    Evas_Event_Mouse_Wheel *ev;
    Etui *etui;
    int alt;
    int shift;
    int ctrl;

    ev = (Evas_Event_Mouse_Wheel *)event;
    etui = (Etui *)data;

    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");

    if ((!alt) && (ctrl) && (!shift))
    {
        if (ev->z == 1)
            etui->doc.scale *= M_SQRT2;
        else
            etui->doc.scale /= M_SQRT2;
        _etui_doc_zoom(etui);
    }
}

static Eina_Bool
_etui_doc_input_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, Evas_Object *src EINA_UNUSED, Evas_Callback_Type type, void *event_info EINA_UNUSED)
{
   return ((type == EVAS_CALLBACK_KEY_UP) ||
           (type == EVAS_CALLBACK_KEY_DOWN));
}

static void
_etui_doc_splash_show(Etui *etui)
{
    Evas_Object *o;

    if ((o = elm_layout_content_get(etui->window.base, "base.content")))
    {
        if (o == etui->doc.sc)
        {
            evas_object_hide(etui->doc.sc);
            elm_layout_content_unset(etui->window.base, "base.content");
            elm_layout_content_set(etui->window.base, "base.content",
                                   etui->doc.splash);
            evas_object_show(etui->doc.splash);
        }
        /* else : splash is already shown */
    }
    else
    {
        /* etui is launched without file or unsupported file */
        elm_layout_content_set(etui->window.base, "base.content",
                               etui->doc.splash);
        evas_object_show(etui->doc.splash);
    }
}

Eina_Bool
etui_doc_init(Etui *etui)
{
    Ecore_Event_Handler *h;
    Evas_Object *o;

    h = ecore_event_handler_add(ECORE_EVENT_KEY_DOWN,
                                _etui_doc_key_down_global_cb,
                                etui);
    if (!h)
        return EINA_FALSE;

    h = etui->doc.handle_key_down;

    /* splash object */
    o = elm_layout_add(etui->window.win);
    if (!etui_theme_apply(o, etui, "etui/splash"))
      goto del_handler;

    etui->doc.splash = o;

    /* scroller object */
    o = elm_scroller_add(etui->window.win);
    elm_scroller_policy_set(o,
                            ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
    elm_scroller_bounce_set(o, EINA_TRUE, EINA_TRUE);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    etui->doc.sc = o;

    /* box object */
    o = elm_box_add(etui->window.win);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_content_set(etui->doc.sc, o);
    evas_object_show(o);
    etui->doc.box = o;

    /* doc object */
    o = etui_object_add(evas_object_evas_get(etui->window.win));
    if (!o)
        goto del_objects;

    etui->doc.doc = o;
    etui->doc.scale = 1.0f;

    elm_object_event_callback_add(etui->doc.sc, _etui_doc_input_cb, NULL);
    evas_object_event_callback_add(etui->doc.sc, EVAS_CALLBACK_KEY_DOWN,
                                   _etui_doc_key_down_cb, etui);
    evas_object_event_callback_add(etui->doc.sc, EVAS_CALLBACK_MOUSE_DOWN,
                                   _etui_doc_mouse_down_cb, etui);
    evas_object_event_callback_add(etui->doc.sc, EVAS_CALLBACK_MOUSE_WHEEL,
                                   _etui_doc_mouse_wheel_cb, etui);

    return EINA_TRUE;

  del_objects:
    evas_object_del(etui->doc.box);
    evas_object_del(etui->doc.sc);
    evas_object_del(etui->doc.splash);
  del_handler:
    ecore_event_handler_del(etui->doc.handle_key_down);

    return EINA_FALSE;
}

void
etui_doc_shutdown(Etui *etui)
{
    evas_object_del(etui->doc.doc);
    evas_object_del(etui->doc.box);
    evas_object_del(etui->doc.sc);
    evas_object_del(etui->doc.splash);
    ecore_event_handler_del(etui->doc.handle_key_down);
}

void etui_doc_show(Etui *etui)
{
    Evas_Object *o;
    int w;
    int h;

    if (!etui_object_file_set(etui->doc.doc, etui->filename))
    {
        /* unsupported file, we show splash */
        _etui_doc_splash_show(etui);
        return;
    }

    etui_object_page_set(etui->doc.doc, 0);
    evas_object_geometry_get(etui->doc.doc, NULL, NULL, &w, &h);
    evas_object_size_hint_min_set(etui->doc.doc, w, h);
    evas_object_size_hint_max_set(etui->doc.doc, w, h);
    evas_object_focus_set(etui->doc.doc, EINA_TRUE);
    etui_object_page_use_display_list_set(etui->doc.doc, EINA_FALSE);
    elm_box_pack_end(etui->doc.box, etui->doc.doc);

    if ((o = elm_layout_content_get(etui->window.base, "base.content")))
    {
        if (o == etui->doc.splash)
        {
            evas_object_hide(etui->doc.splash);
            elm_layout_content_unset(etui->window.base, "base.content");
            elm_layout_content_set(etui->window.base, "base.content",
                                   etui->doc.sc);
            evas_object_show(etui->doc.doc);
            evas_object_show(etui->doc.sc);
        }
        /* else : sc is already shown */
    }
    else
    {
        /* etui is launched with a supported file */
        elm_layout_content_set(etui->window.base, "base.content",
                               etui->doc.sc);
        evas_object_show(etui->doc.doc);
        evas_object_show(etui->doc.sc);
    }
}
