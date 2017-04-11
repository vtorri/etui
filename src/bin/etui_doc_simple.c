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
#include "etui_doc_simple.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


struct Etui_Doc_Simple_
{
    Etui_File *ef;
    Evas_Object *sc;
    Evas_Object *bx;
    Evas_Object *obj;
};

static void
_etui_doc_key_down_cb(void *data,
                      Evas *_e EINA_UNUSED,
                      Evas_Object *_obj EINA_UNUSED,
                      void *event)
{
    Evas_Event_Key_Down *ev;
    Etui *etui;
    Etui_Doc_Simple *doc;

    EINA_SAFETY_ON_NULL_RETURN(data);
    EINA_SAFETY_ON_NULL_RETURN(event);

    etui = (Etui *)data;
    ev = (Evas_Event_Key_Down *)event;
    ERR("Key: %s\n", ev->keyname);

    doc = (Etui_Doc_Simple *)eina_list_data_get(etui->docs);
    if (!strcmp(ev->keyname, "Right"))
        etui_object_page_set(doc->obj, etui_object_page_get(doc->obj) + 1);
    else if (!strcmp(ev->keyname, "Left"))
        etui_object_page_set(doc->obj, etui_object_page_get(doc->obj) - 1);
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


Eina_Bool
etui_doc_add(Etui *etui, const char *filename)
{
    Etui_Doc_Simple *doc;

    doc = (Etui_Doc_Simple *)calloc(1, sizeof(Etui_Doc_Simple));
    if (!doc)
        return EINA_FALSE;

    doc->ef = etui_file_new(filename);
    if (!doc->ef)
        goto free_doc;

    /* scroller object */
    doc->sc = elm_scroller_add(etui->window.win);
    elm_scroller_policy_set(doc->sc,
                            ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
    elm_scroller_bounce_set(doc->sc, EINA_TRUE, EINA_TRUE);
    evas_object_size_hint_weight_set(doc->sc,
                                     EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(doc->sc,
                                    EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(doc->sc);

    /* box object */
    doc->bx = elm_box_add(etui->window.win);
    evas_object_size_hint_weight_set(doc->bx,
                                     EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(doc->bx,
                                    EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_content_set(doc->sc, doc->bx);
    evas_object_show(doc->bx);

    etui->docs = eina_list_append(etui->docs, doc);

    doc->obj = etui_object_add(evas_object_evas_get(etui->window.win));
    etui_object_file_set(doc->obj, doc->ef);
    etui_object_page_set(doc->obj, 0);
    evas_object_focus_set(doc->obj, EINA_TRUE);
    evas_object_repeat_events_set(doc->obj, EINA_TRUE);
    evas_object_show(doc->obj);

    evas_object_event_callback_add(doc->obj, EVAS_CALLBACK_KEY_DOWN,
                                   _etui_doc_key_down_cb, etui);

    return EINA_TRUE;

  free_doc:
    free(doc);

    return EINA_FALSE;
}

void
etui_doc_del(Etui_Doc_Simple *doc)
{
    etui_file_free(doc->ef);
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

