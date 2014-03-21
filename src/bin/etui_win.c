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

#include "etui_private.h"
#include "etui_theme.h"
#include "etui_win.h"

static void
_etui_win_delete_request_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event EINA_UNUSED)
{
    elm_exit();
}

static void
_etui_win_delete_cb(void *data, Evas *evas EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event EINA_UNUSED)
{
    etui_win_free((Etui *)data);
}

static void
_etui_win_focus_in_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event EINA_UNUSED)
{
    Etui *etui = (Etui *)data;

    if (!etui->window.focused)
        elm_win_urgent_set(etui->window.win, EINA_FALSE);
    etui->window.focused = EINA_TRUE;
    edje_object_signal_emit(etui->window.bg, "focus,in", "etui");
    edje_object_signal_emit(etui->window.base, "focus,in", "etui");
    /* elm_object_focus_set(term->term, EINA_TRUE); */
}

static void
_etui_win_focus_out_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event EINA_UNUSED)
{
    Etui *etui = (Etui *)data;

    etui->window.focused = EINA_FALSE;
    edje_object_signal_emit(etui->window.bg, "focus,out", "etui");
    edje_object_signal_emit(etui->window.base, "focus,out", "etui");
    /* elm_object_focus_set(term->term, EINA_FALSE); */
    /* elm_cache_all_flush(); */
}

static void
_etui_win_title_set(Etui *etui)
{
    char buf[PATH_MAX];

    if (etui->filename)
    {
        snprintf(buf, sizeof(buf), "etui - %s", etui->filename);
        elm_win_title_set(etui->window.win, buf);
    }
    else
        elm_win_title_set(etui->window.win, "etui");
}

Eina_Bool etui_win_new(Etui *etui)
{
    char buf[PATH_MAX];
    Evas_Object *o;
    Evas_Load_Error err;

    etui->window.win = elm_win_add(NULL, PACKAGE_NAME, ELM_WIN_BASIC);
    _etui_win_title_set(etui);

    evas_object_smart_callback_add(etui->window.win, "delete,request",
                                   _etui_win_delete_request_cb,
                                   NULL);

    evas_object_event_callback_add(etui->window.win, EVAS_CALLBACK_DEL,
                                   _etui_win_delete_cb, etui);

    elm_win_autodel_set(etui->window.win, EINA_TRUE);

    /* icon */
    o = evas_object_image_add(evas_object_evas_get(etui->window.win));
    snprintf(buf, sizeof(buf), "%s/256x256/etui.png",
             elm_app_data_dir_get());
    evas_object_image_file_set(o, buf, NULL);
    err = evas_object_image_load_error_get(o);
    if (err != EVAS_LOAD_ERROR_NONE)
        INF("Can not find icon: %s", buf);
    elm_win_icon_object_set(etui->window.win, o);

    /* background */
    o = evas_object_rectangle_add(evas_object_evas_get(etui->window.win));
    evas_object_color_set(o, 0, 0, 0, 255);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_win_resize_object_add(etui->window.win, o);
    evas_object_show(o);
    etui->window.bg = o;

    /* conformant */
    o = elm_conformant_add(etui->window.win);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_win_resize_object_add(etui->window.win, o);
    evas_object_show(o);
    etui->window.conform = o;

    /* gui */
    o = elm_layout_add(etui->window.win);
    if (!etui_theme_apply(o, etui, "etui/base"))
    {
        etui_win_free(etui);
        return EINA_FALSE;
    }
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_content_set(etui->window.conform, o);
    evas_object_show(o);
    etui->window.base = o;

    /* FIXME: focus seems to not work anymore */
    evas_object_smart_callback_add(etui->window.win, "focus,in",
                                   _etui_win_focus_in_cb, etui);
    evas_object_smart_callback_add(etui->window.win, "focus,out",
                                   _etui_win_focus_out_cb, etui);

    return EINA_TRUE;
}

void
etui_win_free(Etui *etui)
{
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

    if (etui->window.bg)
    {
        evas_object_del(etui->window.bg);
        etui->window.bg = NULL;
    }

    if (etui->window.win)
    {
        evas_object_event_callback_del_full(etui->window.win, EVAS_CALLBACK_DEL,
                                            _etui_win_delete_cb, etui);
        evas_object_del(etui->window.win);
        etui->window.win = NULL;
    }
}
