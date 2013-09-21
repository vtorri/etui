/* Etui - Multi-document rendering library using the EFL
 * Copyright (C) 2013 Vincent Torri
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

static void
etui_win_del(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   elm_exit();
}

static Eina_Bool
etui_win_main(const char *filename)
{
    Evas_Object *win;
    Evas_Object *sc;
    Evas_Object *etui;
    int w;
    int h;

    win = elm_win_add(NULL, "maiiin", ELM_WIN_BASIC);
    if (!win)
        return 0;

    elm_win_title_set(win, "test etui");
    evas_object_smart_callback_add(win, "delete,request", etui_win_del, NULL);

    sc = elm_scroller_add(win);
    elm_scroller_bounce_set(sc, EINA_TRUE, EINA_TRUE);
    evas_object_size_hint_weight_set(sc, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(sc, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_win_resize_object_add(win, sc);
    evas_object_show(sc);

    etui = etui_object_add(evas_object_evas_get(win));
    if (!etui)
        return 0;

    if (!etui_object_file_set(etui, filename))
        return 0;

    etui_object_page_set(etui, 0);
    evas_object_focus_set(etui, EINA_TRUE);
    etui_object_page_use_display_list_set(etui, EINA_FALSE);
    evas_object_geometry_get(etui, NULL, NULL, &w, &h);
    evas_object_size_hint_min_set(etui, w, h);
    evas_object_size_hint_weight_set(etui, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(etui, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_content_set(sc, etui);
    evas_object_show(etui);

    evas_object_resize(win, 600, 800);
    evas_object_show(win);

    return 1;
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
    if (argc < 2)
        return -1;

    if (!etui_init())
        goto shutdown_elm;

    elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

    if (!etui_win_main(argv[1]))
        goto shutdown_etui;

    elm_run();

    etui_shutdown();
    elm_shutdown();

    return 0;

  shutdown_etui:
    etui_shutdown();
  shutdown_elm:
    elm_shutdown();

    return -1;
}

ELM_MAIN()
