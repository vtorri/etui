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

#ifndef ETUI_DOC_SIMPLE_H
#define ETUI_DOC_SIMPLE_H


typedef struct Etui_Doc_Simple_ Etui_Doc_Simple;

struct Etui_Doc_Simple_
{
    Etui_File *ef;
    Evas_Object *sc;
    Evas_Object *bx;
    Evas_Object *obj;
    float scale; /* scale before fullscreen */
    struct
    {
        Evas_Object *vbox;
        Evas_Object *hbox;
        Evas_Object *entry;
        Evas_Object *bt_next;
        Evas_Object *bt_prev;
        Evas_Object *list;
        Eina_Bool searching : 1;
    } search;
};

Eina_Bool etui_doc_add(Evas_Object *win, Etui_File *ef);
void etui_doc_del(Etui_Doc_Simple *doc);


#endif /* ETUI_DOC_SIMPLE_H */
