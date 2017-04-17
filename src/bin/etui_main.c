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
#include <Ecore_Getopt.h>

#include <Etui.h>

#include "etui_private.h"
#include "etui_config.h"
#include "etui_doc_simple.h"
#include "etui_main.h"
#include "etui_open.h"

static const Ecore_Getopt options = {
    PACKAGE_NAME,
    "%prog [options] [filename]",
    PACKAGE_VERSION,
    gettext_noop("(C) 2013-2017 Vincent Torri and others"),
    "AGPL v3",
    gettext_noop("Multi-document rendering application written with Enlightenment Foundation Libraries."),
    EINA_TRUE,
    {
        ECORE_GETOPT_STORE_STR ('g', "geometry",
                                gettext_noop("Window geometry to use (eg 80x24 or 80x24+50+20 etc.).")),
        ECORE_GETOPT_STORE_STR ('r', "role",
                                gettext_noop("Set window role.")),
        ECORE_GETOPT_STORE_TRUE('f', "fullscreen",
                                gettext_noop("Go into the fullscreen mode from start.")),

        ECORE_GETOPT_VERSION   ('V', "version"),
        ECORE_GETOPT_COPYRIGHT ('C', "copyright"),
        ECORE_GETOPT_LICENSE   ('L', "license"),
        ECORE_GETOPT_HELP      ('h', "help"),
        ECORE_GETOPT_SENTINEL
    }
};

#if HAVE_GETTEXT && ENABLE_NLS
static void
_etui_translate_options(void)
{
    Ecore_Getopt_Desc *desc;

    options.copyright = eina_stringshare_printf(_(options.copyright),
                                                2017);

    desc = (Ecore_Getopt_Desc *)options.descs;
    while ((desc->shortname != '\0') ||
           (desc->longname) ||
           (desc->action == ECORE_GETOPT_ACTION_CATEGORY))
    {
        if (desc->help)
        {
            switch (desc->action)
            {
                case ECORE_GETOPT_ACTION_VERSION:
                    desc->help = _("show program version.");
                    break;
                case ECORE_GETOPT_ACTION_COPYRIGHT:
                    desc->help = _("show copyright.");
                    break;
                case ECORE_GETOPT_ACTION_LICENSE:
                    desc->help = _("show license.");
                    break;
                case ECORE_GETOPT_ACTION_HELP:
                    desc->help = _("show this message.");
                    break;
                default:
                    desc->help = _(desc->help);
            }
        }
        desc++;
    }
}
#endif

int etui_app_log_dom_global = 1;

static Etui *
etui_new(const char *filename)
{
    Etui *etui;

    etui = (Etui *)calloc(1, sizeof(Etui));
    if (!etui)
    {
        ERR(_("Can not allocate memory for main structure"));
        return NULL;
    }

    if (filename)
    {
        etui->filename = strdup(filename);
        if (!etui->filename)
        {
            ERR(_("Can not allocate memory for file name"));
            free(etui);
            return NULL;
        }
    }

    return etui;
}

static void
etui_del(Etui *etui)
{
    Etui_Doc_Simple *doc;

    free(etui->filename);
    EINA_LIST_FREE(etui->docs, doc)
        etui_doc_del(doc);
    free(etui);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
    Etui *etui;
    Etui_Config *config;
    const char *filename = NULL;
    char *geometry = NULL;
    char *role = NULL;
    int args;
    int pos_x = 0;
    int pos_y = 0;
    int size_w = 1;
    int size_h = 1;
    Eina_Bool fullscreen = EINA_FALSE;
    Eina_Bool quit_option = EINA_FALSE;
    Eina_Bool pos_set = EINA_FALSE;
    Eina_Bool size_set = EINA_FALSE;
    Ecore_Getopt_Value values[] = {
        ECORE_GETOPT_VALUE_STR(geometry),
        ECORE_GETOPT_VALUE_STR(role),
        ECORE_GETOPT_VALUE_BOOL(fullscreen),

        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),

        ECORE_GETOPT_VALUE_NONE
    };

    elm_language_set("");
    elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
    elm_app_compile_bin_dir_set(PACKAGE_BIN_DIR);
    elm_app_compile_lib_dir_set(PACKAGE_LIB_DIR);
    elm_app_compile_data_dir_set(PACKAGE_DATA_DIR);
#if HAVE_GETTEXT && ENABLE_NLS
    elm_app_compile_locale_set(LOCALEDIR);
#endif
    elm_app_info_set(elm_main, "etui", "themes/default.edj");
    elm_app_name_set("etui");

#if HAVE_GETTEXT && ENABLE_NLS
    bindtextdomain(PACKAGE, elm_app_locale_dir_get());
    textdomain(PACKAGE);
    _etui_translate_options();
#endif

    etui_app_log_dom_global = eina_log_domain_register("etui-app", NULL);
    if (etui_app_log_dom_global < 0)
    {
        EINA_LOG_CRIT(_("could not create log domain 'etui-app'."));
        goto shutdown_elm;
    }

    if (!etui_init())
    {
        ERR(_("could not initialize Etui library."));
        goto shutdown_elm;
    }

    etui_config_init();
    config = etui_config_load("config");
    if (!config)
    {
        ERR(_("could not create log domain 'etui-app'."));
        goto shutdown_config;
    }
    /* FIXME: key bindings */

    args = ecore_getopt_parse(&options, values, argc, argv);
    if (args < 0)
    {
        ERR(_("Could not parse command line options."));
        goto del_config;
    }

    if (quit_option)
        goto del_config;

    if (geometry)
    {
        if (sscanf(geometry,"%ix%i+%i+%i", &size_w, &size_h, &pos_x, &pos_y) == 4)
        {
            pos_set = EINA_TRUE;
            size_set = EINA_TRUE;
        }
        else if (sscanf(geometry,"%ix%i-%i+%i", &size_w, &size_h, &pos_x, &pos_y) == 4)
        {
            pos_x = -pos_x;
            pos_set = EINA_TRUE;
            size_set = EINA_TRUE;
        }
        else if (sscanf(geometry,"%ix%i-%i-%i", &size_w, &size_h, &pos_x, &pos_y) == 4)
        {
            pos_x = -pos_x;
            pos_y = -pos_y;
            pos_set = EINA_TRUE;
            size_set = EINA_TRUE;
        }
        else if (sscanf(geometry,"%ix%i+%i-%i", &size_w, &size_h, &pos_x, &pos_y) == 4)
        {
            pos_y = -pos_y;
            pos_set = EINA_TRUE;
            size_set = EINA_TRUE;
        }
        else if (sscanf(geometry,"%ix%i", &size_w, &size_h) == 2)
        {
            size_set = EINA_TRUE;
        }
        else if (sscanf(geometry,"+%i+%i", &pos_x, &pos_y) == 2)
        {
            pos_set = EINA_TRUE;
        }
        else if (sscanf(geometry,"-%i+%i", &pos_x, &pos_y) == 2)
        {
            pos_x = -pos_x;
            pos_set = EINA_TRUE;
        }
        else if (sscanf(geometry,"+%i-%i", &pos_x, &pos_y) == 2)
        {
            pos_y = -pos_y;
            pos_set = EINA_TRUE;
        }
        else if (sscanf(geometry,"-%i-%i", &pos_x, &pos_y) == 2)
        {
            pos_x = -pos_x;
            pos_y = -pos_y;
            pos_set = EINA_TRUE;
        }
    }

    if (args != argc)
        filename = argv[args];

    if (!size_set)
    {
        if (config->custom_geometry)
        {
            size_w = config->cg_width;
            size_h = config->cg_height;
        }
        else
        {
            size_w = 480;
            size_h = 640;
        }
    }

    elm_theme_overlay_add(NULL, etui_config_theme_path_default_get(config));
    elm_theme_overlay_add(NULL, etui_config_theme_path_get(config));

    etui = etui_new(filename);
    if (!etui)
        goto del_config;

    if (!etui_win_new(etui, role, pos_set, pos_x, pos_y, size_w, size_h, fullscreen, config))
        goto del_etui;

    etui_doc_add(etui, etui_file_new(filename));

    /* if no document, we display a file selector to open a file */
    if (eina_list_count(etui->docs) == 0)
        etui_open_toggle(etui->window.win, etui->window.base);

    evas_object_show(etui->window.win);

    elm_run();

    etui_del(etui);
    etui_config_del(config);
    /* key binding shutdown */
    etui_config_shutdown();
    etui_shutdown();
    eina_log_domain_unregister(etui_app_log_dom_global);
    etui_app_log_dom_global = -1;

#if HAVE_GETTEXT && ENABLE_NLS
    eina_stringshare_del(options.copyright);
#endif

    elm_shutdown();

    return 0;

  del_etui:
    etui_del(etui);
  del_config:
    etui_config_del(config);
  shutdown_config:
    etui_config_shutdown();
    etui_shutdown();
    eina_log_domain_unregister(etui_app_log_dom_global);
    etui_app_log_dom_global = -1;
  shutdown_elm:
    elm_shutdown();

    return -1;
}
ELM_MAIN()
