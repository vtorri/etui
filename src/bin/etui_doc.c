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
#include "etui_theme.h"
#include "etui_doc.h"

static void
etui_splash_icon_set(Etui *etui)
{
    Evas_Object *o;

    o = elm_layout_add(etui->window.win);
    if (!etui_theme_apply(o, etui, "etui/splash"))
        return;

    elm_layout_content_set(etui->window.base, "base.content", o);
    evas_object_show(o);
}

void etui_doc_set(Etui *etui)
{
    Evas_Object *sc;
    Evas_Object *box;
    Evas_Object *doc;
    int w;
    int h;

    if (!etui->filename)
    {
        etui_splash_icon_set(etui);
        return;
    }

    sc = elm_scroller_add(etui->window.win);
    elm_scroller_policy_set(sc, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
    elm_scroller_bounce_set(sc, EINA_TRUE, EINA_TRUE);
    evas_object_size_hint_weight_set(sc, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(sc, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(sc);

    box = elm_box_add(etui->window.win);
    evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_content_set(sc, box);
    evas_object_show(box);

    doc = etui_object_add(evas_object_evas_get(etui->window.win));
    if (!doc)
        return;

    if (!etui_object_file_set(doc, etui->filename))
        return;

    etui_object_page_set(doc, 0);
    evas_object_geometry_get(doc, NULL, NULL, &w, &h);
    evas_object_size_hint_min_set(doc, w, h);
    evas_object_size_hint_max_set(doc, w, h);
    evas_object_size_hint_weight_set(doc, 0.5, 0.5);
    evas_object_size_hint_fill_set(doc, 0.5, 0.5);
    evas_object_focus_set(doc, EINA_TRUE);
    etui_object_page_use_display_list_set(doc, EINA_FALSE);
    elm_box_pack_end(box, doc);
    evas_object_show(doc);

    elm_layout_content_set(etui->window.base, "base.content", sc);
}
