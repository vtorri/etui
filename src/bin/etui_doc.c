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
_etui_doc_key_down_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event)
{
    Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event;
    Etui *etui = (Etui *)data;
    int alt;
    int shift;
    int ctrl;

    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");

    printf("keyname: %s  key: %s  string: %s\n",
           ev->keyname, ev->key, ev->string);

    if ((!alt) && (!ctrl) && (!shift))
    {
        if (!strcmp(ev->key, "Right") || !strcmp(ev->key, "Down") || !strcmp(ev->key, "space"))
        {
            etui_object_page_set(etui->window.doc, etui_object_page_get(etui->window.doc) + 1);
        }
        else if (!strcmp(ev->key, "Left") || !strcmp(ev->key, "Up") || !strcmp(ev->key, "BackSpace"))
        {
            etui_object_page_set(etui->window.doc, etui_object_page_get(etui->window.doc) - 1);
        }
        else if (!strcmp(ev->key, "Next"))
        {
            etui_object_page_set(etui->window.doc, etui_object_page_get(etui->window.doc) + 10);
        }
        else if (!strcmp(ev->key, "Prior"))
        {
            etui_object_page_set(etui->window.doc, etui_object_page_get(etui->window.doc) - 10);
        }
        else if (!strcmp(ev->key, "F1"))
        {
            /* TODO: help */
        }
        else if (!strcmp(ev->key, "F11"))
        {
            /* TODO: toggle fullscreen */
            Evas_Coord win_w;
            Evas_Coord win_h;
            int screen_w;
            int screen_h;
            int w;
            int h;

            if (!elm_win_fullscreen_get(etui->window.win))
            {
                float scale;

                evas_object_geometry_get(etui->window.win,
                                         NULL, NULL, &win_w, &win_h);
                etui->window.win_w = win_w;
                etui->window.win_h = win_h;

                elm_win_screen_size_get(etui->window.win, NULL, NULL,
                                        &screen_w, &screen_h);
                etui_object_page_size_get(etui->window.doc, &w, &h);

                if (screen_w > screen_h)
                {
                    if (w < h)
                    {
                        scale = (float)(screen_h) / (float)h;
                        printf(" ** scale : %f\n", (double)scale);
                        etui_object_page_scale_set(etui->window.doc, scale, scale);
                        evas_object_geometry_get(etui->window.doc, NULL, NULL, &w, &h);
                        printf(" *** %d %d\n", w, h);
                        evas_object_size_hint_min_set(etui->window.doc, w, h);
                        evas_object_size_hint_max_set(etui->window.doc, w, h);
                    }
                    else
                    {
                        scale = (float)(screen_w) / (float)w;
                        etui_object_page_scale_set(etui->window.doc, scale, scale);
                        evas_object_geometry_get(etui->window.doc, NULL, NULL, &w, &h);
                        evas_object_size_hint_min_set(etui->window.doc, w, h);
                        evas_object_size_hint_max_set(etui->window.doc, w, h);
                    }
                }
                else
                {
                    /* something to do ? */
                }

                elm_win_fullscreen_set(etui->window.win, EINA_TRUE);

                evas_object_size_hint_min_set(etui->window.bg, screen_w, screen_h);
                evas_object_resize(etui->window.win, screen_w, screen_h);
                elm_scroller_policy_set(etui->window.sc, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
            }
            else
            {

                elm_win_fullscreen_set(etui->window.win, EINA_FALSE);
                evas_object_size_hint_min_set(etui->window.bg, etui->window.win_w, etui->window.win_h);
                evas_object_resize(etui->window.win, etui->window.win_w, etui->window.win_h);
                elm_scroller_policy_set(etui->window.sc, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
                etui_object_page_scale_set(etui->window.doc, etui->window.scale, etui->window.scale);
                evas_object_geometry_get(etui->window.doc, NULL, NULL, &w, &h);
                evas_object_size_hint_min_set(etui->window.doc, w, h);
                evas_object_size_hint_max_set(etui->window.doc, w, h);
            }
        }
        else if (!strcmp(ev->key, "Escape"))
        {
            /* TODO: exit fullscreen if fullscreen ? */
        }
    }

    if ((!alt) && (ctrl) && (!shift))
    {
        if (!strcmp(ev->key, "q"))
        {
            etui_win_free(etui);
        }
        else if (!strcmp(ev->key, "KP_Add"))
        {
            int w;
            int h;

            etui->window.scale *= M_SQRT2;
            etui_object_page_scale_set(etui->window.doc, etui->window.scale, etui->window.scale);
            evas_object_geometry_get(etui->window.doc, NULL, NULL, &w, &h);
            evas_object_size_hint_min_set(etui->window.doc, w, h);
            evas_object_size_hint_max_set(etui->window.doc, w, h);
        }
        else if (!strcmp(ev->key, "KP_Subtract"))
        {
            int w;
            int h;

            etui->window.scale *= M_SQRT1_2;
            etui_object_page_scale_set(etui->window.doc, etui->window.scale, etui->window.scale);
            evas_object_geometry_get(etui->window.doc, NULL, NULL, &w, &h);
            evas_object_size_hint_min_set(etui->window.doc, w, h);
            evas_object_size_hint_max_set(etui->window.doc, w, h);
        }
        else if (!strcmp(ev->key, "KP_0"))
        {
            int w;
            int h;

            /* zoom to fit page */
            etui->window.scale *= M_SQRT2;
            etui_object_page_scale_set(etui->window.doc, etui->window.scale, etui->window.scale);
            evas_object_geometry_get(etui->window.doc, NULL, NULL, &w, &h);
            evas_object_size_hint_min_set(etui->window.doc, w, h);
            evas_object_size_hint_max_set(etui->window.doc, w, h);
        }
        else if (!strcmp(ev->key, "KP_1"))
        {
            int w;
            int h;

            /* zoom to 100% */
            etui->window.scale = 1.0f;
            etui_object_page_scale_set(etui->window.doc, etui->window.scale, etui->window.scale);
            evas_object_geometry_get(etui->window.doc, NULL, NULL, &w, &h);
            evas_object_size_hint_min_set(etui->window.doc, w, h);
            evas_object_size_hint_max_set(etui->window.doc, w, h);
        }
        else if (!strcmp(ev->key, "KP_2"))
        {
            Evas_Coord win_w;
            int w;
            int h;

            /* zoom to fit width */
            evas_object_geometry_get(etui->window.win, NULL, NULL, &win_w, NULL);
            etui_object_page_size_get(etui->window.doc, &w, NULL);
            etui->window.scale = (float)win_w / (float)w;
            etui_object_page_scale_set(etui->window.doc, etui->window.scale, etui->window.scale);
            evas_object_geometry_get(etui->window.doc, NULL, NULL, &w, &h);
            evas_object_size_hint_min_set(etui->window.doc, w, h);
            evas_object_size_hint_max_set(etui->window.doc, w, h);
        }
        else if (!strcmp(ev->key, "KP_3"))
        {
            Evas_Coord win_h;
            int w;
            int h;

            /* zoom to fit height */
            evas_object_geometry_get(etui->window.win, NULL, NULL, NULL, &win_h);
            etui_object_page_size_get(etui->window.doc, NULL, &h);
            etui->window.scale = (float)win_h / (float)h;
            etui_object_page_scale_set(etui->window.doc, etui->window.scale, etui->window.scale);
            evas_object_geometry_get(etui->window.doc, NULL, NULL, &w, &h);
            evas_object_size_hint_min_set(etui->window.doc, w, h);
            evas_object_size_hint_max_set(etui->window.doc, w, h);
        }
        else if (!strcmp(ev->key, "Home"))
        {
            etui_object_page_set(etui->window.doc, 0);
        }
        else if (!strcmp(ev->key, "End"))
        {
            etui_object_page_set(etui->window.doc, etui_object_document_pages_count(etui->window.doc) - 1);
        }
        else if (!strcmp(ev->key, "o"))
        {
            /* TODO: open document */
        }
    }
}

static void
_etui_doc_key_up_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event)
{
    Evas_Event_Key_Up *ev = (Evas_Event_Key_Up *)event;
    Etui *etui = (Etui *)data;
    int alt;
    int shift;
    int ctrl;

    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");

    if ((!alt) && (ctrl) && (!shift))
    {
        if (!strcmp(ev->key, "q"))
            etui_win_free(etui);
    }
}

static Eina_Bool
_etui_doc_input_cb(void *data, Evas_Object *obj EINA_UNUSED, Evas_Object *src EINA_UNUSED, Evas_Callback_Type type, void *event_info EINA_UNUSED)
{
   return ((type == EVAS_CALLBACK_KEY_UP) ||
           (type == EVAS_CALLBACK_KEY_DOWN));
}

static void
etui_splash_icon_set(Etui *etui)
{
    Evas_Object *o;

    o = elm_layout_add(etui->window.win);
    if (!etui_theme_apply(o, etui, "etui/splash"))
        return;

    elm_layout_content_set(etui->window.base, "base.content", o);
    evas_object_show(o);
}

void etui_doc_set(Etui *etui)
{
    Evas_Object *sc;
    Evas_Object *box;
    Evas_Object *doc;
    int w;
    int h;

    if (!etui->filename)
        goto splash;

    sc = elm_scroller_add(etui->window.win);
    elm_scroller_policy_set(sc, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
    elm_scroller_bounce_set(sc, EINA_TRUE, EINA_TRUE);
    evas_object_size_hint_weight_set(sc, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(sc, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(sc);

    etui->window.sc = sc;

    box = elm_box_add(etui->window.win);
    evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_content_set(sc, box);
    evas_object_show(box);

    doc = etui_object_add(evas_object_evas_get(etui->window.win));
    if (!doc)
        goto splash;

    if (!etui_object_file_set(doc, etui->filename))
        goto splash;

    etui_object_page_set(doc, 0);
    evas_object_geometry_get(doc, NULL, NULL, &w, &h);
    evas_object_size_hint_min_set(doc, w, h);
    evas_object_size_hint_max_set(doc, w, h);
    /* evas_object_size_hint_weight_set(doc, 0.5, 0.5); */
    /* evas_object_size_hint_fill_set(doc, 0.5, 0.5); */
    evas_object_focus_set(doc, EINA_TRUE);
    etui_object_page_use_display_list_set(doc, EINA_FALSE);
    elm_box_pack_end(box, doc);
    evas_object_show(doc);

    etui->window.doc = doc;
    etui->window.scale = 1.0f;

    elm_layout_content_set(etui->window.base, "base.content", sc);

    elm_object_event_callback_add(sc, _etui_doc_input_cb, NULL);
    evas_object_event_callback_add(sc, EVAS_CALLBACK_KEY_DOWN,
                                   _etui_doc_key_down_cb, etui);
    return;

  splash:
    etui_splash_icon_set(etui);
}
