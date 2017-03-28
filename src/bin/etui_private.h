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

#ifndef ETUI_BIN_PRIVATE_H
#define ETUI_BIN_PRIVATE_H

#if HAVE_GETTEXT && ENABLE_NLS
# define _(string) gettext(string)
#else
# define _(string) (string)
#endif
#define gettext_noop(String) String

extern int etui_app_log_dom_global;

#ifdef ETUI_DEFAULT_LOG_COLOR
# undef ETUI_DEFAULT_LOG_COLOR
#endif
#define ETUI_DEFAULT_LOG_COLOR EINA_COLOR_CYAN

#ifdef ERR
# undef ERR
#endif
#define ERR(...)  EINA_LOG_DOM_ERR(etui_app_log_dom_global, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...)  EINA_LOG_DOM_DBG(etui_app_log_dom_global, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...)  EINA_LOG_DOM_INFO(etui_app_log_dom_global, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...)  EINA_LOG_DOM_WARN(etui_app_log_dom_global, __VA_ARGS__)

#ifdef CRIT
# undef CRIT
#endif
#define CRIT(...) EINA_LOG_DOM_CRIT(etui_app_log_dom_global, __VA_ARGS__)

typedef struct Etui_ Etui;

struct Etui_
{
    char *filename;

    struct
    {
        Evas_Object *win;
        Evas_Object *conform;
        Evas_Object *base;
        Evas_Object *event;
        Etui_Config *config;
        int win_w;
        int win_h;
        Eina_Bool focused : 1;
    } window;

    struct
    {
        Ecore_Event_Handler *handle_key_down;
        Evas_Object *splash;
        Evas_Object *sc;
        Evas_Object *box;
        Evas_Object *doc;
        float scale;
    } doc;

    struct
    {
        char *file;
    } theme;
};

void etui_win_new(Etui *etui, const char *role,
                  Eina_Bool pos_set, int x, int y, int width, int height,
                  Eina_Bool fullscreen, Etui_Config *config);

#endif /* ETUI_BIN_PRIVATE_H */
