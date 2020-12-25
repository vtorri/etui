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

#include <Etui.h>

#include "etui_private.h"
#include "etui_config.h"
#include "et_win.h"
#include "etui_doc_simple.h"
#include "etui_open.h"
#include "etui_settings.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

static Evas_Object *op_inwin = NULL;
static Evas_Object *op_fs = NULL;
static Eina_Bool op_out = EINA_FALSE;

static void
_etui_fs_done_cb(void            *data,
                 Evas_Object *obj EINA_UNUSED,
                 void            *event_info)
{
    Etui *etui;

    etui = evas_object_data_get(data, "etui");
    if (!etui)
        return;

    if (!event_info && (eina_list_count(etui->docs) != 0))
        etui_open_del();

    if (etui_doc_add(data, etui_file_new(event_info)))
        etui_open_del();
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
etui_open_add(Evas_Object *win)
{
    const char *home_env;

    if (op_out)
        return;

    op_inwin = elm_win_inwin_add(win);
    evas_object_show(op_inwin);

    op_fs = elm_fileselector_add(win);
    evas_object_size_hint_weight_set(op_fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(op_fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_fileselector_is_save_set(op_fs, EINA_TRUE);
    elm_fileselector_expandable_set(op_fs, EINA_TRUE);
    home_env = eina_environment_home_get();
    elm_fileselector_path_set(op_fs, home_env);
    evas_object_smart_callback_add(op_fs, "done", _etui_fs_done_cb, win);
    elm_win_inwin_content_set(op_inwin, op_fs);
    evas_object_show(op_fs);

    elm_object_focus_set(op_inwin, EINA_TRUE);
    op_out = EINA_TRUE;
}

void
etui_open_del(void)
{
    evas_object_del(op_fs);
    op_fs = NULL;
    evas_object_del(op_inwin);
    op_inwin = NULL;
    op_out = EINA_FALSE;
    elm_object_focus_set(op_inwin, EINA_FALSE);
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
