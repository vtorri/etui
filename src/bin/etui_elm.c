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

#include <config.h>

#include <Elementary.h>

#include <Etui.h>

typedef struct
{
    Evas_Object *o;
    Evas_Object *sc;
    Ecore_Event_Handler *handler;
    Etui_Rotation rotation;
    float scale;
} Etui_Config;


static Eina_Bool
_etui_key_down(void *data, int type EINA_UNUSED, void *event)
{
    Etui_Config *cfg;
    Ecore_Event_Key *ev;

    cfg = evas_object_data_get((Evas_Object *)data, "config");
    ev = event;

    if (ev->key)
    {
        if ((!strcmp(ev->key, "Right")))
            etui_object_page_set(cfg->o, etui_object_page_get(cfg->o) + 1);
        else if (!strcmp(ev->key, "Left"))
            etui_object_page_set(cfg->o, etui_object_page_get(cfg->o) - 1);
        else if (!strcmp(ev->key, "Up"))
        {
            cfg->scale *= M_SQRT2;
            etui_object_page_scale_set(cfg->o, cfg->scale);
        }
        else if (!strcmp(ev->key, "Down"))
        {
            int w,h;
            cfg->scale *= M_SQRT1_2;
            etui_object_page_scale_set(cfg->o, cfg->scale);
            elm_scroller_child_size_get(cfg->sc, &w, &h);
            printf(" bin 0 ** %dx%d\n", w, h);
            evas_object_geometry_get(cfg->o, NULL, NULL, &w, &h);
            printf(" bin 1 ** %dx%d\n", w, h);
            evas_object_size_hint_min_set(cfg->o, cfg->scale*w, cfg->scale*h);
            /* evas_object_size_hint_max_set(cfg->o, cfg->scale*w, cfg->scale*h); */
            printf(" bin 2 ** %dx%d\n", w, h);
        }
        else if (!strcmp(ev->key, "<"))
        {
            cfg->rotation += 90;
            if (cfg->rotation > ETUI_ROTATION_270)
                cfg->rotation = ETUI_ROTATION_0;
            etui_object_page_rotation_set(cfg->o, cfg->rotation);
        }
        else if (!strcmp(ev->key, ">"))
        {
            cfg->rotation -= 90;
            if (cfg->rotation < ETUI_ROTATION_0)
                cfg->rotation = ETUI_ROTATION_270;
            etui_object_page_rotation_set(cfg->o, cfg->rotation);
        }
        else if (!strcmp(ev->key, "q"))
            elm_exit();
    }

    return ECORE_CALLBACK_PASS_ON;
}

static void
etui_win_del(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   elm_exit();
}

static Eina_Bool
etui_win_main(const char *filename)
{
    Etui_Config *cfg;
    Ecore_Event_Handler *handler;
    Evas_Object *win;
    Evas_Object *sc;
    Evas_Object *box;
    Evas_Object *etui;
    int w;
    int h;

    cfg = (Etui_Config *)malloc(sizeof(Etui_Config));
    if (!cfg)
        return 0;

    win = elm_win_add(NULL, "main", ELM_WIN_BASIC);
    if (!win)
        goto free_cfg;

    elm_win_title_set(win, "test etui");
    evas_object_smart_callback_add(win, "delete,request", etui_win_del, NULL);

    sc = elm_scroller_add(win);
    elm_scroller_policy_set(sc, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
    elm_scroller_bounce_set(sc, EINA_TRUE, EINA_TRUE);
    evas_object_size_hint_weight_set(sc, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(sc, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_win_resize_object_add(win, sc);
    evas_object_show(sc);

    box = elm_box_add(win);
    evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_content_set(sc, box);
    evas_object_show(box);

    etui = etui_object_add(evas_object_evas_get(win));
    if (!etui)
        goto free_cfg;

    if (!etui_object_file_set(etui, filename))
        goto free_cfg;

    etui_object_page_set(etui, 0);
    evas_object_geometry_get(etui, NULL, NULL, &w, &h);
    evas_object_size_hint_weight_set(etui, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(etui, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_focus_set(etui, EINA_TRUE);
    etui_object_page_use_display_list_set(etui, EINA_FALSE);
    elm_box_pack_end(box, etui);
    evas_object_show(etui);

    handler = ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, _etui_key_down, win);

    cfg->o = etui;
    cfg->sc = sc;
    cfg->handler = handler;
    cfg->rotation = ETUI_ROTATION_0;
    cfg->scale = 1.0f;
    evas_object_data_set(win, "config", cfg);

    evas_object_resize(win, 600, 800);
    evas_object_show(win);

    return EINA_TRUE;

  free_cfg:
    free(cfg);

    return EINA_FALSE;
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
