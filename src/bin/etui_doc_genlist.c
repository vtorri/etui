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
#include <Etui.h>

#include "etui_private.h"
#include "etui_main.h"
#include "etui_doc_genlist.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


struct Etui_Doc_Genlist_
{
    Etui_File *ef;
    Evas_Object *obj;
    Evas_Object *gl;
};

static void
gl_sel(void *data, Evas_Object *obj, void *event_info)
{
   printf("sel item data [%p] on genlist obj [%p], item pointer [%p], index [%d]\n",
          data, obj, event_info, elm_genlist_item_index_get(event_info));
}

static Evas_Object *
gl_content_get(void *data, Evas_Object *obj, const char *part)
{
    int num;
    Evas_Object *o;

    if (strcmp(part, "elm.swallow.content") != 0)
        return NULL;

    o  = evas_object_rectangle_add(evas_object_evas_get(obj));
    evas_object_size_hint_min_set(o, 50, 200);
    evas_object_size_hint_max_set(o, 50, 200);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);

    num = (int)(uintptr_t)data;
    switch (num % 3)
    {
        case 0:
            evas_object_color_set(o, 255, 0, 0, 255);
            break;
        case 1:
            evas_object_color_set(o, 0, 255, 0, 255);
            break;
        case 2:
            evas_object_color_set(o, 0, 0, 255, 255);
            break;
    }
    evas_object_show(o);

    return o;
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


Eina_Bool
etui_doc_add(Etui *etui, const char *filename)
{
    Elm_Genlist_Item_Class *itc;
    Etui_Doc_Genlist *doc;
    int num_pages;
    int i;

    doc = (Etui_Doc_Genlist *)calloc(1, sizeof(Etui_Doc_Genlist));
    if (!doc)
        return EINA_FALSE;

    doc->ef = etui_file_new(filename);
    if (!doc->ef)
        goto free_doc;

    doc->obj = etui_object_add(evas_object_evas_get(etui->window.win));
    etui_object_file_set(doc->obj, doc->ef);
    num_pages = etui_object_document_pages_count(doc->obj);

    doc->gl = elm_genlist_add(etui->window.win);
    elm_genlist_mode_set(doc->gl, ELM_LIST_COMPRESS);
    elm_genlist_multi_select_set(doc->gl, EINA_FALSE);
    evas_object_size_hint_weight_set(doc->gl,
                                     EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(doc->gl,
                                    EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_part_content_set(etui->window.base, "content", doc->gl);
    elm_win_resize_object_add(etui->window.win, doc->gl);
    evas_object_show(doc->gl);

    itc = elm_genlist_item_class_new();
    itc->item_style = "full";
    itc->func.text_get = NULL; //gl_text_get1;
    itc->func.content_get = gl_content_get;
    itc->func.state_get = NULL;//gl_state_get;
    itc->func.del = NULL;
    itc->func.reusable_content_get = NULL;//gl_reusable_content_get;

    for (i = 0; i < num_pages; i++)
    {
        Elm_Object_Item *gli;

        gli = elm_genlist_item_append(doc->gl, itc,
                                      (void *)(uintptr_t)i/* item data */,
                                      NULL/* parent */,
                                      ELM_GENLIST_ITEM_NONE,
                                      gl_sel/* func */,
                                      (void *)(uintptr_t)(i * 10)/* func data */);
    }
    elm_genlist_item_class_free(itc);

    etui->docs = eina_list_append(etui->docs, doc);

    evas_object_show(doc->obj);

    return EINA_TRUE;

  free_doc:
    free(doc);

    return EINA_FALSE;
}

void
etui_doc_del(Etui_Doc_Genlist *doc)
{
    evas_object_del(doc->gl);
    evas_object_del(doc->obj);
    etui_file_free(doc->ef);
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

