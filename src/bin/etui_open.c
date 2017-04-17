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

#include <Etui.h>

#include "etui_private.h"
#include "etui_doc_simple.h"
#include "etui_main.h"
#include "etui_open.h"
#include "etui_settings.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

static Evas_Object *op_inwin = NULL;
static Evas_Object *op_fs = NULL;
static Evas_Object *op_win = NULL;
static Evas_Object *op_bg = NULL;
static Eina_Bool op_out = EINA_FALSE;

static void
_etui_fs_done_cb(void            *data,
                 Evas_Object *obj EINA_UNUSED,
                 void            *event_info)
{
    Etui *etui;

    etui = evas_object_data_get((Evas_Object *)data, "etui");
    if (!etui)
        return;

    if (etui_doc_add(etui, etui_file_new(event_info)))
        etui_open_toggle(etui->window.win, etui->window.base);
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


Eina_Bool
etui_open_active_get(void)
{
    return op_out;
}

void
etui_open_toggle(Evas_Object *win, Evas_Object *bg)
{
    if ((win != op_win) && op_inwin)
    {
        evas_object_del(op_inwin);
        op_inwin = NULL;
        op_win = NULL;
    }

    if ((win != op_win) && op_inwin)
    {
        evas_object_del(op_inwin);
        op_inwin = NULL;
        op_win = NULL;
    }

    if (!op_inwin)
    {
        Evas_Object *o;
        const char *home_env;

        op_inwin = elm_win_inwin_add(win);
        evas_object_show(op_inwin);

        o = elm_fileselector_add(win);
        evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
        elm_fileselector_is_save_set(o, EINA_TRUE);
        elm_fileselector_expandable_set(o, EINA_TRUE);
        home_env = eina_environment_home_get();
        elm_fileselector_path_set(o, home_env);
        evas_object_smart_callback_add(o, "done", _etui_fs_done_cb, win);
        elm_win_inwin_content_set(op_inwin, o);
        evas_object_show(o);
        op_fs = o;
    }

    if (!op_out)
    {
        elm_object_part_content_set(bg, "fs:etui.fileselector", op_inwin);
        evas_object_show(op_win);
        /* evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_DOWN, */
        /*                                _etui_mouse_down_cb, NULL); */
        /* evas_object_event_callback_add(o, EVAS_CALLBACK_DEL, */
        /*                                _etui_over_del_cb, NULL); */
        op_win = win;
        op_bg = bg;
        /* FIXME: callback */
        elm_object_signal_emit(bg, "fs:fileselector,show", "etui");
        op_out = EINA_TRUE;
        elm_object_focus_set(op_inwin, EINA_TRUE);
    }
    else
    {
        elm_object_signal_emit(op_bg, "fs:fileselector,hide", "etui");
        op_out = EINA_FALSE;
        elm_object_focus_set(op_inwin, EINA_FALSE);
        /* FIXME: callback */
    }
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
