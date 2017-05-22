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

#include "etui_private.h"
#include "etui_main.h"
#include "etui_settings.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


static void
_etui_settings_elm_config_launch(void *_data EINA_UNUSED,
                                 Evas_Object *_obj EINA_UNUSED,
                                 void *_event_info EINA_UNUSED)
{
    Ecore_Exe *exe;

    exe = ecore_exe_pipe_run("elementary_config", 0, NULL);
    ecore_exe_free(exe);
}



/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


void
etui_settings_elm(Evas_Object *stbox, Evas_Object *_term EINA_UNUSED)
{
    Evas_Object *box;
    Evas_Object *o;

    /* frame */
    o = elm_frame_add(stbox);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_text_set(o, "Toolkit");
    elm_box_pack_end(stbox, o);
    evas_object_show(o);

    /* box */
    box = elm_box_add(stbox);
    elm_object_content_set(o, box);
    evas_object_show(box);

    /* entry */
    o = elm_entry_add(stbox);
    elm_entry_context_menu_disabled_set(o, EINA_TRUE);
    elm_entry_editable_set(o, EINA_FALSE);
    elm_entry_line_wrap_set(o, ELM_WRAP_MIXED);
    elm_object_text_set(o, "<em>Terminology</em> uses the <hilight>elementary</hilight> toolkit.<br>"
                              "The toolkit configuration settings can be accessed by running <keyword>elementary_config</keyword>.");
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.5);
    elm_box_pack_end(box, o);
    evas_object_show(o);

    /* button */
    o = elm_button_add(stbox);
    evas_object_smart_callback_add(o, "clicked",
                                   _etui_settings_elm_config_launch, NULL);
    evas_object_propagate_events_set(o, EINA_FALSE);
    elm_layout_text_set(o, NULL, "Launch elementary_config");
    elm_box_pack_end(box, o);
    evas_object_show(o);
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
