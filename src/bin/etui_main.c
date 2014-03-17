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
#include <Ecore_Getopt.h>

#include <Etui.h>

#include "etui_private.h"
#include "etui_theme.h"
#include "etui_win.h"


static const Ecore_Getopt options = {
   PACKAGE_NAME,
   "%prog [options] [filename]",
   PACKAGE_VERSION,
   "(C) 2014 Vincent Torri",
   "GPL v3",
   "Multi-document rendering application written with Enlightenment Foundation Libraries.",
   EINA_TRUE,
   {
      ECORE_GETOPT_STORE_STR ('g', "geometry",
                              "Window geometry to use (eg 80x24 or 80x24+50+20 etc.)."),
      ECORE_GETOPT_STORE_TRUE('f', "fullscreen",
                              "Go into the fullscreen mode from start."),

      ECORE_GETOPT_VERSION   ('V', "version"),
      ECORE_GETOPT_COPYRIGHT ('C', "copyright"),
      ECORE_GETOPT_LICENSE   ('L', "license"),
      ECORE_GETOPT_HELP      ('h', "help"),
      ECORE_GETOPT_SENTINEL
   }
};

static Etui *
_etui_new(const char *filename)
{
    Etui *etui;

    etui = (Etui *)calloc(1, sizeof(Etui));
    if (!etui)
        return NULL;

    if (filename)
    {
        etui->filename = strdup(filename);
        if (!etui->filename)
        {
            ERR("Can not allocate memory for filename %s", filename);
            goto free_etui;
        }
    }

    return etui;

  free_etui:
    free(etui);

    return NULL;
}

static void
_etui_free(Etui *etui)
{
    if (etui->theme.file)
        free(etui->theme.file);
    if (etui->filename)
        free(etui->filename);
    free(etui);
}

int etui_app_log_dom_global = 1;

int elm_main(int argc, char **argv);

EAPI_MAIN int
elm_main(int argc, char **argv)
{
    Etui *etui;
    char *geometry = NULL;
    Eina_Bool fullscreen = EINA_FALSE;
    Eina_Bool quit_option = EINA_FALSE;
    Ecore_Getopt_Value values[] = {
        ECORE_GETOPT_VALUE_STR(geometry),
        ECORE_GETOPT_VALUE_BOOL(fullscreen),

        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),

        ECORE_GETOPT_VALUE_NONE
    };
    const char *filename = NULL;
    char *theme = NULL;
    int args;
    int pos_set = 0;
    int size_set = 0;
    int pos_x = 0;
    int pos_y = 0;
    int size_w = 1;
    int size_h = 1;

    etui_app_log_dom_global = eina_log_domain_register("etui-app", NULL);
    if (etui_app_log_dom_global < 0)
    {
        EINA_LOG_CRIT("could not create log domain 'etui-app'.");
        goto shutdown_elm;
    }

    args = ecore_getopt_parse(&options, values, argc, argv);
    if (args < 0)
    {
        ERR("Could not parse command line options.");
        goto shutdown_elm;
    }

    if (quit_option)
        goto shutdown_elm;

    if (geometry)
    {
        if (sscanf(geometry,"%ix%i+%i+%i", &size_w, &size_h, &pos_x, &pos_y) == 4)
        {
            pos_set = 1;
            size_set = 1;
        }
        else if (sscanf(geometry,"%ix%i-%i+%i", &size_w, &size_h, &pos_x, &pos_y) == 4)
        {
            pos_x = -pos_x;
            pos_set = 1;
            size_set = 1;
        }
        else if (sscanf(geometry,"%ix%i-%i-%i", &size_w, &size_h, &pos_x, &pos_y) == 4)
        {
            pos_x = -pos_x;
            pos_y = -pos_y;
            pos_set = 1;
            size_set = 1;
        }
        else if (sscanf(geometry,"%ix%i+%i-%i", &size_w, &size_h, &pos_x, &pos_y) == 4)
        {
            pos_y = -pos_y;
            pos_set = 1;
            size_set = 1;
        }
        else if (sscanf(geometry,"%ix%i", &size_w, &size_h) == 2)
        {
            size_set = 1;
        }
        else if (sscanf(geometry,"+%i+%i", &pos_x, &pos_y) == 2)
        {
            pos_set = 1;
        }
        else if (sscanf(geometry,"-%i+%i", &pos_x, &pos_y) == 2)
        {
            pos_x = -pos_x;
            pos_set = 1;
        }
        else if (sscanf(geometry,"+%i-%i", &pos_x, &pos_y) == 2)
        {
            pos_y = -pos_y;
            pos_set = 1;
        }
        else if (sscanf(geometry,"-%i-%i", &pos_x, &pos_y) == 2)
        {
            pos_x = -pos_x;
            pos_y = -pos_y;
            pos_set = 1;
        }
    }

    if (args != argc)
        filename = argv[args];

    elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
    elm_app_compile_bin_dir_set(PACKAGE_BIN_DIR);
    elm_app_compile_data_dir_set(PACKAGE_DATA_DIR);
    elm_app_info_set(elm_main, "etui", "themes/default.edj");
    elm_app_name_set("etui");

    if (!etui_init())
        goto shutdown_elm;

    etui = _etui_new(filename);
    if (!etui)
        goto shutdown_etui;

    theme = etui_theme_default_get(etui);
    if (!theme)
        goto shutdown_etui;

    elm_theme_overlay_add(NULL, theme);

    if (!etui_win_new(etui))
        goto free_etui;

    evas_object_size_hint_min_set(etui->window.bg, size_w, size_h);
    evas_object_resize(etui->window.win, size_w, size_h);

    if (!size_set)
    {
        /* if (config->custom_geometry) */
        /* { */
        /*     size_w = config->cg_width; */
        /*     size_h = config->cg_height; */
        /* } */
        /* else */
        {
            size_w = 600;
            size_h = 800;
        }
    }

    if (pos_set)
    {
        int screen_w;
        int screen_h;

        elm_win_screen_size_get(etui->window.win, NULL, NULL, &screen_w, &screen_h);
        if (pos_x < 0) pos_x = screen_w + pos_x;
        if (pos_y < 0) pos_y = screen_h + pos_y;
        evas_object_move(etui->window.win, pos_x, pos_y);
     }

    evas_object_show(etui->window.win);

    elm_run();

    etui_win_free(etui);
    elm_theme_overlay_del(NULL, etui_theme_default_get(etui));
    _etui_free(etui);
    etui_shutdown();
    eina_log_domain_unregister(etui_app_log_dom_global);
    etui_app_log_dom_global = -1;
    elm_shutdown();

    return EXIT_SUCCESS;

  free_etui:
    elm_theme_overlay_del(NULL, etui_theme_default_get(etui));
    _etui_free(etui);
    etui = NULL;
  shutdown_etui:
    etui_shutdown();
  shutdown_elm:
    eina_log_domain_unregister(etui_app_log_dom_global);
    etui_app_log_dom_global = -1;
    elm_shutdown();

    return EXIT_FAILURE;
}

ELM_MAIN()
