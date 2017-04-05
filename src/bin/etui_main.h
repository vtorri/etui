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

#ifndef ETUI_BIN_MAIN_H
#define ETUI_BIN_MAIN_H


struct Etui_
{
    char *filename;

    struct
    {
        Evas_Object *win;
        Evas_Object *conform;
        Evas_Object *base;
        Evas_Object *panel;
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

Eina_Bool etui_win_new(Etui *etui, const char *role,
                       Eina_Bool pos_set, int x, int y, int width, int height,
                       Eina_Bool fullscreen, Etui_Config *config);


#endif /* ETUI_BIN_MAIN_H */
