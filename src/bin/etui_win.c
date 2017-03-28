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

#include "etui_config.h"
#include "private.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


static void
_etui_win_delete_request_cb(void *data EINA_UNUSED,
                            Evas_Object *obj EINA_UNUSED,
                            void *event EINA_UNUSED)
{
    elm_exit();
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


void
etui_win_new(Etui *etui, const char *role,
             Eina_Bool pos_set, int x, int y, int width, int height,
             Eina_Bool fullscreen, Etui_Config *config)
{
    Evas_Object *o;

    /* all pointers and values are valid */

    o = elm_win_add(NULL, PACKAGE_NAME, ELM_WIN_BASIC);
    elm_win_title_set(o, "Etui");
    /* TODO: icon name */
    if (role)
        elm_win_role_set(o, role);
    if (fullscreen)
        elm_win_fullscreen_set(o, EINA_TRUE);
    elm_win_focus_highlight_enabled_set(o, EINA_TRUE);
    elm_win_autodel_set(o, EINA_TRUE);

    evas_object_resize(o, width, height);
    if (pos_set)
    {
        int screen_w;
        int screen_h;

        elm_win_screen_size_get(o, NULL, NULL, &screen_w, &screen_h);
        evas_object_move(etui->window.win,
                         (x < 0) ? screen_w + x : x,
                         (y < 0) ? screen_h + y : y);
    }

    evas_object_smart_callback_add(o, "delete,request",
                                   _etui_win_delete_request_cb,
                                   NULL);

    etui->window.win = o;
    etui->window.config = config;
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
