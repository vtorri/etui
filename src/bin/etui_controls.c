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

#include "etui_settings.h"
#include "etui_controls.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


static Evas_Object *ct_win = NULL;
static Evas_Object *ct_bg = NULL;
static Evas_Object *ct_frame = NULL; // the frame containing the buttons
static Evas_Object *ct_box = NULL; // for the layout of the buttons
static Evas_Object *ct_over = NULL; // rectangle over panel to avoid hiding it

static Ecore_Timer *ct_del_timer = NULL; // delete objects after timeout

static Eina_Bool ct_out = EINA_FALSE; // is controls displayed ?

/* controls callbacks */

static void
_etui_ct_close_cb(void *_data EINA_UNUSED,
                  Evas_Object *_obj EINA_UNUSED,
                  void *_event EINA_UNUSED)
{
    /* FIXME */
    //term_close(ct_win, ct_term, EINA_FALSE);
}

static void
_etui_ct_settings_cb(void *_data EINA_UNUSED,
                     Evas_Object *_obj EINA_UNUSED,
                     void *_event EINA_UNUSED)
{
    etui_controls_toggle(ct_win, ct_bg);
    etui_settings_toggle(ct_win, ct_bg);
}

static void
_etui_ct_about_cb(void *_data EINA_UNUSED,
                  Evas_Object *_obj EINA_UNUSED,
                  void *_event EINA_UNUSED)
{
    etui_controls_toggle(ct_win, ct_bg);
    /* FIXME */
    //about_toggle(ct_win, ct_bg, ct_term, ct_donecb, ct_donedata);
}

/* gui callbacks */

static Eina_Bool
_etui_ct_del_delay_cb(void *_data EINA_UNUSED)
{
    fprintf(stderr, "delay !!\n");
    if (ct_over)
    {
        evas_object_del(ct_over);
        ct_over = NULL;
    }
    if (ct_frame)
    {
        evas_object_del(ct_frame);
        ct_frame = NULL;
    }
    ct_del_timer = NULL;
    elm_cache_all_flush();

    return EINA_FALSE;
}

static void
_etui_mouse_down_cb(void *_data EINA_UNUSED,
                    Evas *_e EINA_UNUSED,
                    Evas_Object *_obj EINA_UNUSED,
                    void *_ev EINA_UNUSED)
{
    etui_controls_toggle(ct_win, ct_bg);
}

static void
_etui_over_del_cb(void *_data EINA_UNUSED,
                  Evas *_e EINA_UNUSED,
                  Evas_Object *_obj EINA_UNUSED,
                  void *_ev EINA_UNUSED)
{
    ct_over = NULL;
}

static void
_etui_saved_del_cb(void *_data EINA_UNUSED,
                   Evas *_e EINA_UNUSED,
                   Evas_Object *obj,
                   void *_ev EINA_UNUSED)
{
    if (obj == ct_win)
    {
        if (ct_frame)
        {
            evas_object_del(ct_frame);
            ct_frame = NULL;
        }
        if (ct_del_timer)
        {
            ecore_timer_del(ct_del_timer);
            ct_del_timer = NULL;
        }
        if (ct_over)
        {
            evas_object_del(ct_over);
            ct_over = NULL;
        }
        evas_object_event_callback_del(ct_win, EVAS_CALLBACK_DEL, _etui_saved_del_cb);
        ct_win = NULL;
        ct_bg = NULL;
    }
}

static Evas_Object *
_button_add(Evas_Object *win, const char *label, const char *icon, Evas_Smart_Cb cb, void *cbdata)
{
    Evas_Object *bt;
    Evas_Object *o;

    bt = o = elm_button_add(win);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    if (label)
        elm_object_text_set(o, label);

    evas_object_smart_callback_add(o, "clicked", cb, cbdata);

    if (icon)
    {
        o = elm_icon_add(win);
        evas_object_size_hint_aspect_set(o, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        elm_icon_standard_set(o, icon);
        elm_object_part_content_set(bt, "icon", o);
        evas_object_show(o);
    }

    evas_object_show(bt);

    return bt;
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


void
etui_controls_toggle(Evas_Object *win, Evas_Object *bg)
{
    Evas_Object *o;

    fprintf(stderr, "ct_out avant : %d\n", ct_out);

    if (!ct_out)
    {
        if (etui_settings_active_get())
        {
            etui_settings_toggle(win, bg);
            return;
        }
    }

    if ((win != ct_win) && ct_frame)
    {
        evas_object_del(ct_frame);
        ct_frame = NULL;
        ct_win = NULL;
    }

    if (!ct_frame)
    {
        ct_frame = o = elm_frame_add(win);
        evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
        elm_object_text_set(o, "Controls");

        ct_box = o = elm_box_add(win);
        elm_box_horizontal_set(o, EINA_FALSE);
        elm_object_content_set(ct_frame, o);
        evas_object_show(o);

        o = _button_add(win, "Settings", "preferences-desktop", _etui_ct_settings_cb, NULL);
        elm_box_pack_end(ct_box, o);

        o = _button_add(win, "About", "help-about", _etui_ct_about_cb, NULL);
        elm_box_pack_end(ct_box, o);

        o = _button_add(win, "Quit", "window-close", _etui_ct_close_cb, NULL);
        elm_box_pack_end(ct_box, o);
    }

    if (!ct_out)
    {
        elm_object_part_content_set(bg, "fg:etui.controls", ct_frame);
        evas_object_show(ct_frame);

        ct_over = o = evas_object_rectangle_add(evas_object_evas_get(win));
        evas_object_color_set(o, 0, 0, 0, 0);
        elm_object_part_content_set(bg, "fg:etui.dismiss", o);
        evas_object_show(o);
        evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_DOWN,
                                       _etui_mouse_down_cb, NULL);
        evas_object_event_callback_add(o, EVAS_CALLBACK_DEL,
                                       _etui_over_del_cb, NULL);
        ct_win = win;
        ct_bg = bg;
        /* FIXME: callback */
        elm_object_signal_emit(bg, "fg:controls,show", "etui");
        ct_out = EINA_TRUE;
        elm_object_focus_set(ct_frame, EINA_TRUE);
        if (ct_del_timer)
          {
             ecore_timer_del(ct_del_timer);
             ct_del_timer = NULL;
          }
    }
    else
    {
        if (ct_over)
        {
            evas_object_del(ct_over);
            ct_over = NULL;
        }

        elm_object_signal_emit(ct_bg, "fg:controls,hide", "etui");
        ct_out = EINA_FALSE;
        elm_object_focus_set(ct_frame, EINA_FALSE);
        /* FIXME: callback */
        if (ct_del_timer)
            ecore_timer_del(ct_del_timer);
        fprintf(stderr, "delay timer created\n");
        ct_del_timer = ecore_timer_add(10.0, _etui_ct_del_delay_cb, NULL);
    }

    if (ct_win)
        evas_object_event_callback_del(ct_win, EVAS_CALLBACK_DEL,
                                       _etui_saved_del_cb);

    if (ct_out)
        evas_object_event_callback_add(ct_win, EVAS_CALLBACK_DEL,
                                       _etui_saved_del_cb, NULL);

    fprintf(stderr, "ct_out apres : %d\n", ct_out);
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
