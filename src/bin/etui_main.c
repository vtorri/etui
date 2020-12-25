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
#include <Ecore_Getopt.h>

#include <Etui.h>

#include "etui_private.h"
#include "etui_config.h"
#include "et_win.h"
#include "etui_doc_simple.h"

int etui_app_log_dom_global = 1;

static const Ecore_Getopt options = {
    PACKAGE_NAME,
    "%prog [options] [filename]",
    PACKAGE_VERSION,
    gettext_noop("(C) 2013-2017 Vincent Torri and others"),
    "AGPL v3",
    gettext_noop("Multi-document rendering application written with Enlightenment Foundation Libraries."),
    EINA_TRUE,
    {
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

static EAPI_MAIN int
elm_main(int argc, char **argv)
{
    const char *filename = NULL;
    Evas_Object *win;
    int args;
    Eina_Bool fullscreen = EINA_FALSE;
    Eina_Bool quit_option = EINA_FALSE;
    Ecore_Getopt_Value values[] = {
        ECORE_GETOPT_VALUE_BOOL(fullscreen),

        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),
        ECORE_GETOPT_VALUE_BOOL(quit_option),

        ECORE_GETOPT_VALUE_NONE
    };

    etui_app_log_dom_global = eina_log_domain_register("etui-app", NULL);
    if (etui_app_log_dom_global < 0)
    {
        EINA_LOG_CRIT(_("could not create log domain 'etui-app'."));
        goto end;
    }

    args = ecore_getopt_parse(&options, values, argc, argv);
    if (args < 0)
    {
        ERR(_("Could not parse command line options."));
        goto end;
    }

    if (quit_option)
        goto end;

    if (args != argc)
        filename = argv[args];

    etui_config_init();
    /* FIXME: key bindings */

    elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
    elm_app_compile_bin_dir_set(PACKAGE_BIN_DIR);
    elm_app_compile_lib_dir_set(PACKAGE_LIB_DIR);
    elm_app_compile_data_dir_set(PACKAGE_DATA_DIR);
#if HAVE_GETTEXT && ENABLE_NLS
    elm_app_compile_locale_set(LOCALEDIR);
#endif
    elm_app_name_set("etui");
    elm_app_info_set(elm_main, "etui", "themes/default.edj");

#if HAVE_GETTEXT && ENABLE_NLS
    bindtextdomain(PACKAGE, elm_app_locale_dir_get());
    textdomain(PACKAGE);
    _etui_translate_options();
#endif

    if (!etui_init())
    {
        ERR(_("could not initialize Etui library."));
        goto shutdown_config;
    }

    win = etui_win_add();
    if (!win)
    {
        ERR(_("could not create main window."));
        goto shutdown_etui;
    }

    //evas_object_event_callback_add(win, EVAS_CALLBACK_RESIZE, _cb_resize, NULL);
    evas_object_resize(win,
                       562 * elm_config_scale_get(),
                       800 * elm_config_scale_get());

    etui_doc_add(win, etui_file_new(filename));

    if (fullscreen) elm_win_fullscreen_set(win, EINA_TRUE);

    evas_object_show(win);

    elm_run();

    return 0;

  shutdown_etui:
    etui_shutdown();
  shutdown_config:
    etui_config_shutdown();
    eina_log_domain_unregister(etui_app_log_dom_global);
    etui_app_log_dom_global = -1;
  end:
    return 1;
}
ELM_MAIN()
