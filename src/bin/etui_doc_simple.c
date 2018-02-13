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
#include "etui_config.h"
#include "etui_main.h"
#include "etui_open.h"
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

static void
_etui_doc_search_add(Etui *etui)
{
    Etui_Doc_Simple *doc;
    Evas_Object *o;

    doc = (Etui_Doc_Simple *)eina_list_data_get(etui->docs);

    o = elm_box_add(doc->sc);
    elm_box_horizontal_set(o, EINA_FALSE);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(o);
    doc->search.vbox = o;

    o = elm_box_add(doc->sc);
    elm_box_horizontal_set(o, EINA_TRUE);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_pack_end(doc->search.vbox, o);
    evas_object_show(o);
    doc->search.hbox = o;

    o = elm_entry_add(doc->sc);
    elm_entry_single_line_set(o, EINA_TRUE);
    elm_object_part_text_set(o, "guide", "Type search text here");
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_pack_end(doc->search.hbox, o);
    evas_object_show(o);
    doc->search.entry = o;

    o = elm_button_add(doc->sc);
    elm_object_style_set(o, "spinner/decrease/vertical");
    evas_object_size_hint_weight_set(o, 0.0, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_pack_end(doc->search.hbox, o);
    evas_object_show(o);
    doc->search.bt_next = o;

    o = elm_button_add(doc->sc);
    elm_object_style_set(o, "spinner/increase/vertical");
    evas_object_size_hint_weight_set(o, 0.0, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_pack_end(doc->search.hbox, o);
    evas_object_show(o);
    doc->search.bt_prev = o;

    o = elm_genlist_add(doc->sc);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_pack_end(doc->search.vbox, o);
    evas_object_show(o);
    doc->search.list = o;

    /* append items */
}

static void
_etui_doc_zoom(const Etui_Doc_Simple *doc)
{
    int w;
    int h;

    etui_object_page_scale_set(doc->obj, doc->scale);
    etui_object_page_mode_set(doc->obj, ETUI_MODE_FREE);
    evas_object_geometry_get(doc->obj, NULL, NULL, &w, &h);
    evas_object_size_hint_min_set(doc->obj, w, h);
    evas_object_size_hint_max_set(doc->obj, w, h);
}

static void
_etui_doc_fullscreen_set(const Etui *etui, Eina_Bool on)
{
    Etui_Doc_Simple *doc;
    int w;
    int h;

    doc = (Etui_Doc_Simple *)eina_list_data_get(etui->docs);

    if (on)
    {
        float scale;
        int ws;
        int hs;

        /* we save the scale before going fullscreen */
        etui_object_page_scale_get(doc->obj, &doc->scale, &doc->scale);

        /* we set the new scale to fit the entire screen */
        etui_object_page_size_get(doc->obj, &w, &h);
        elm_win_screen_size_get(etui->window.win, NULL, NULL, &ws, &hs);
        scale = (w < h) ? (float)(hs) / (float)h : (float)(ws) / (float)w;
        etui_object_page_scale_set(doc->obj, scale, scale);

        /* no more scrollbar */
        elm_scroller_policy_set(doc->sc,
                                ELM_SCROLLER_POLICY_OFF,
                                ELM_SCROLLER_POLICY_OFF);
    }
    else
    {
        elm_scroller_policy_set(doc->sc,
                                ELM_SCROLLER_POLICY_AUTO,
                                ELM_SCROLLER_POLICY_AUTO);

        /* we restore the scale */
        etui_object_page_scale_set(doc->obj, doc->scale, doc->scale);
    }

    /* FIXME: is it necessary ? */
    evas_object_geometry_get(doc->obj, NULL, NULL, &w, &h);
    evas_object_size_hint_min_set(doc->obj, w, h);
    evas_object_size_hint_max_set(doc->obj, w, h);

    elm_win_fullscreen_set(etui->window.win, on);
}

static void
_etui_doc_key_down_cb(void *data,
                      Evas *_e EINA_UNUSED,
                      Evas_Object *_obj EINA_UNUSED,
                      void *event)
{
    Evas_Event_Key_Down *ev;
    Etui *etui;
    Etui_Doc_Simple *doc;
    int ctrl, alt, shift, win, meta, hyper;

    EINA_SAFETY_ON_NULL_RETURN(data);
    EINA_SAFETY_ON_NULL_RETURN(event);

    etui = (Etui *)data;
    ev = (Evas_Event_Key_Down *)event;
    doc = (Etui_Doc_Simple *)eina_list_data_get(etui->docs);

    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");
    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    win = evas_key_modifier_is_set(ev->modifiers, "Super");
    meta =
        evas_key_modifier_is_set(ev->modifiers, "Meta") ||
        evas_key_modifier_is_set(ev->modifiers, "AltGr") ||
        evas_key_modifier_is_set(ev->modifiers, "ISO_Level3_Shift");
    hyper = evas_key_modifier_is_set(ev->modifiers, "Hyper");

    /* No modifier */
    if (!ctrl && !alt && !shift && !win && !meta && !hyper)
    {
        if (!strcmp(ev->keyname, "Right") || !strcmp(ev->keyname, "space"))
        {
            int page;

            page = etui_object_page_get(doc->obj);
            if ((page == (etui_object_document_pages_count(doc->obj) - 1)) &&
                (!etui->window.config->disable_visual_bell))
            {
                elm_object_signal_emit(etui->window.base, "bell:bell", "etui");
                if (etui->window.config->bell_rings)
                    elm_object_signal_emit(etui->window.base,
                                           "bell:bell,ring", "etui");
            }
            else
                etui_object_page_set(doc->obj, page + 1);
        }
        else if (!strcmp(ev->keyname, "Left") || !strcmp(ev->keyname, "BackSpace"))
        {
            int page;

            page = etui_object_page_get(doc->obj);
            if ((page == 0) &&
                (!etui->window.config->disable_visual_bell))
            {
                elm_object_signal_emit(etui->window.base, "bell:bell", "etui");
                if (etui->window.config->bell_rings)
                    elm_object_signal_emit(etui->window.base,
                                           "bell:bell,ring", "etui");
            }
            else
                etui_object_page_set(doc->obj, page - 1);
        }
        if (!strcmp(ev->keyname, "Next"))
        {
            int page;
            int num_pages;

            page = etui_object_page_get(doc->obj);
            num_pages = etui_object_document_pages_count(doc->obj);
            if ((page == (num_pages - 1)) &&
                (!etui->window.config->disable_visual_bell))
            {
                elm_object_signal_emit(etui->window.base, "bell:bell", "etui");
                if (etui->window.config->bell_rings)
                    elm_object_signal_emit(etui->window.base,
                                           "bell:bell,ring", "etui");
            }
            else
                etui_object_page_set(doc->obj,
                                     ((page + 10) < num_pages) ? (page + 10) : (num_pages - 1));
        }
        else if (!strcmp(ev->keyname, "Prior"))
        {
            int page;

            page = etui_object_page_get(doc->obj);
            if ((page == 0) &&
                (!etui->window.config->disable_visual_bell))
            {
                elm_object_signal_emit(etui->window.base, "bell:bell", "etui");
                if (etui->window.config->bell_rings)
                    elm_object_signal_emit(etui->window.base,
                                           "bell:bell,ring", "etui");
            }
            else
                etui_object_page_set(doc->obj, (page >= 10) ? (page - 10) : 0);
        }
        else if (!strcmp(ev->keyname, "F11"))
        {
            _etui_doc_fullscreen_set(etui,
                                     !elm_win_fullscreen_get(etui->window.win));
        }
        else if (!strcmp(ev->keyname, "Escape"))
        {
            if (elm_win_fullscreen_get(etui->window.win))
                _etui_doc_fullscreen_set(etui, EINA_FALSE);
            if (doc->search.searching)
            {
                elm_object_signal_emit(etui->window.base,
                                       "doc:search,hide", "etui");
                doc->search.searching = EINA_FALSE;
                elm_object_focus_set(doc->sc, EINA_TRUE);
            }
        }
    }

    /* Ctrl modifier */
    if (ctrl && !alt && !shift && !win && !meta && !hyper)
    {
        if (!strcmp(ev->key, "KP_Add"))
        {
            doc->scale *= M_SQRT2;
            _etui_doc_zoom(doc);
        }
        else if (!strcmp(ev->key, "KP_Subtract"))
        {
            doc->scale *= M_SQRT1_2;
            _etui_doc_zoom(doc);
        }
        else if (!strcmp(ev->key, "KP_1"))
        {
            /* zoom to 100% */
            doc->scale = 1.0f;
            _etui_doc_zoom(doc);
        }
        else if (!strcmp(ev->key, "Home"))
        {
            etui_object_page_set(doc->obj, 0);
        }
        else if (!strcmp(ev->key, "End"))
        {
            etui_object_page_set(doc->obj,
                                 etui_object_document_pages_count(doc->obj) - 1);
        }
        else if (!strcmp(ev->key, "f"))
        {
            if (!doc->search.searching)
            {
                if (!doc->search.vbox)
                {
                    _etui_doc_search_add(etui);
                    elm_object_part_content_set(etui->window.base,
                                                "doc:etui.search",
                                                doc->search.vbox);
                }
                elm_object_signal_emit(etui->window.base,
                                       "doc:search,show", "etui");
                doc->search.searching = EINA_TRUE;
                elm_object_focus_set(doc->search.entry, EINA_TRUE);
            }
        }
        else if (!strcmp(ev->key, "g"))
        {
            const char *text;

            if (doc->search.searching &&
                (text = elm_entry_entry_get(doc->search.entry)))
            {
                Eina_Rectangle *r;
                const Etui_Module_Pdf_Api *api;
                Eina_Inarray *a;
                int page;
                int num_pages;
                int i;

                api = etui_object_api_get(doc->obj);
                num_pages = etui_object_document_pages_count(doc->obj);
                a = NULL;
                for (i = 0; i < num_pages; i++)
                {
                    if ((a = api->search(api->mod, i, text)))
                    {
                        page = i;
                        break;
                    }
                }
                if (a)
                {
                    etui_object_page_set(doc->obj, page);
                    fprintf(stderr, "text '%s' find page %d\n", text, page);
                    EINA_INARRAY_FOREACH(a, r)
                    {
                        fprintf(stderr, "  box (%d, %d) (%d, %d)\n",
                                r->x, r->y, r->w, r->h);
                    }
                }
            }
        }
    }
}

static void
_etui_doc_mouse_down_cb(void *data,
                        Evas *e EINA_UNUSED,
                        Evas_Object *obj EINA_UNUSED,
                        void *event)
{
    Evas_Event_Mouse_Down *ev;
    Etui *etui;
    int ctrl, alt, shift, win, meta, hyper;

    ev = (Evas_Event_Mouse_Down *)event;
    etui = (Etui *)data;

    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");
    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    win = evas_key_modifier_is_set(ev->modifiers, "Super");
    meta =
        evas_key_modifier_is_set(ev->modifiers, "Meta") ||
        evas_key_modifier_is_set(ev->modifiers, "AltGr") ||
        evas_key_modifier_is_set(ev->modifiers, "ISO_Level3_Shift");
    hyper = evas_key_modifier_is_set(ev->modifiers, "Hyper");

    /* No modifier */
    if (!ctrl && !alt && !shift && !win && !meta && !hyper)
    {
        if ((ev->button == 1) && (ev->flags & EVAS_BUTTON_DOUBLE_CLICK))
        {
            _etui_doc_fullscreen_set(etui,
                                     !elm_win_fullscreen_get(etui->window.win));
        }
    }
}

static void
_etui_doc_mouse_wheel_cb(void *data,
                         Evas *e EINA_UNUSED,
                         Evas_Object *obj EINA_UNUSED,
                         void *event)
{
    Evas_Event_Mouse_Wheel *ev;
    Etui *etui;
    Etui_Doc_Simple *doc;
    int ctrl, alt, shift, win, meta, hyper;

    ev = (Evas_Event_Mouse_Wheel *)event;
    etui = (Etui *)data;
    doc = (Etui_Doc_Simple *)eina_list_data_get(etui->docs);

    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");
    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    win = evas_key_modifier_is_set(ev->modifiers, "Super");
    meta =
        evas_key_modifier_is_set(ev->modifiers, "Meta") ||
        evas_key_modifier_is_set(ev->modifiers, "AltGr") ||
        evas_key_modifier_is_set(ev->modifiers, "ISO_Level3_Shift");
    hyper = evas_key_modifier_is_set(ev->modifiers, "Hyper");

    /* Ctrl modifier */
    if (ctrl && !alt && !shift && !win && !meta && !hyper)
    {
        if (ev->z == 1)
            doc->scale *= M_SQRT2;
        else
            doc->scale *= M_SQRT1_2;
        _etui_doc_zoom(doc);
    }
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


Eina_Bool
etui_doc_add(Etui *etui, Etui_File *ef)
{
    Etui_Doc_Simple *doc;
    int width;
    int height;

    if (!ef)
        return EINA_FALSE;

    doc = (Etui_Doc_Simple *)calloc(1, sizeof(Etui_Doc_Simple));
    if (!doc)
        return EINA_FALSE;

    doc->ef = ef;

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

    doc->obj = etui_object_add(evas_object_evas_get(etui->window.win));
    printf("Mode set lolollol\n");
    etui_object_page_mode_set(doc->obj, ETUI_MODE_FIT_AUTO);
    etui_object_file_set(doc->obj, doc->ef);
    etui_object_page_set(doc->obj, 0);
    evas_object_geometry_get(doc->obj, NULL, NULL, &width, &height);
    evas_object_size_hint_weight_set(doc->obj, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(doc->obj, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_pack_end(doc->bx, doc->obj);
    evas_object_show(doc->obj);

    etui_object_page_scale_get(doc->obj, &doc->scale, NULL);

    elm_object_focus_set(doc->sc, EINA_TRUE);

    etui->docs = eina_list_append(etui->docs, doc);

    elm_object_part_content_set(etui->window.base, "doc:etui.content", doc->sc);

    evas_object_event_callback_add(etui->window.win, EVAS_CALLBACK_KEY_DOWN,
                                   _etui_doc_key_down_cb, etui);
    evas_object_event_callback_add(doc->sc, EVAS_CALLBACK_MOUSE_DOWN,
                                   _etui_doc_mouse_down_cb, etui);
    evas_object_event_callback_add(doc->sc, EVAS_CALLBACK_MOUSE_WHEEL,
                                   _etui_doc_mouse_wheel_cb, etui);

    return EINA_TRUE;
}

void
etui_doc_del(Etui_Doc_Simple *doc)
{
    evas_object_del(doc->obj);
    evas_object_del(doc->bx);
    evas_object_del(doc->sc);
    etui_file_free(doc->ef);
    free(doc);
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

