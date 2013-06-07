/* Etui - Multi-document rendering library using the EFL
 * Copyright (C) 2013 Vincent Torri
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

#ifndef ETUI_MODULE_H
#define ETUI_MODULE_H

#define ETUI_PROVIDER_DESCRIPTOR_VERSION (1U)
#define ETUI_PROVIDER_DESCRIPTOR_PRIORITY_DEFAULT 50

typedef struct _Etui_Provider_Instance Etui_Provider_Instance;
typedef struct _Etui_Provider_Descriptor Etui_Provider_Descriptor;

struct _Etui_Provider_Descriptor
{
    const char     *name;
    unsigned int    version;
    int             priority;
    void             *(*init)(Evas *evas);
    void              (*shutdown)(void *d);
    Evas_Object      *(*evas_object_get)(void *d);
    Eina_Bool         (*file_open)(void *d, const char *filename);
    void              (*file_close)(void *d);
    void              (*version_get)(void *d, int *maj, int *min);
    char             *(*title_get)(void *d);
    char             *(*author_get)(void *d);
    char             *(*subject_get)(void *d);
    char             *(*keywords_get)(void *d);
    char             *(*creator_get)(void *d);
    char             *(*producer_get)(void *d);
    char             *(*creation_date_get)(void *d);
    char             *(*modification_date_get)(void *d);
    Eina_Bool         (*is_printable)(void *d);
    Eina_Bool         (*is_changeable)(void *d);
    Eina_Bool         (*is_copyable)(void *d);
    Eina_Bool         (*is_notable)(void *d);
    Eina_Bool         (*password_needed)(void *d);
    Eina_Bool         (*password_set)(void *d, const char *password);
    int               (*pages_count)(void *d);
    const Eina_Array *(*toc_get)(void *d);
    void              (*page_use_display_list_set)(void *d, Eina_Bool on);
    Eina_Bool         (*page_use_display_list_get)(void *d);
    Eina_Bool         (*page_set)(void *d, int num);
    int               (*page_get)(void *d);
    void              (*page_size_get)(void *d, int *width, int *height);
    Eina_Bool         (*page_rotation_set)(void *d, Etui_Rotation rotation);
    Etui_Rotation     (*page_rotation_get)(void *d);
    Eina_Bool         (*page_scale_set)(void *d, float hscale, float vscale);
    void              (*page_scale_get)(void *d, float *hscale, float *vscale);
    const Eina_Array *(*page_links_get)(void *d);
    void              (*page_render_pre)(void *d);
    void              (*page_render)(void *d);
    void              (*page_render_end)(void *d);
    char *            (*page_text_extract)(void *d, const Eina_Rectangle *rect);
    Eina_Array       *(*page_text_find)(void *d, const char *needle);
    float             (*page_duration_get)(void *d);
    Etui_Transition   (*page_transition_type_get)(void *d);
    float             (*page_transition_duration_get)(void *d);
    Eina_Bool         (*page_transition_vertical_get)(void *d);
    Eina_Bool         (*page_transition_outwards_get)(void *d);
    int               (*page_transition_direction_get)(void *d);
};

Eina_Bool etui_modules_init(void);
void etui_modules_shutdown(void);

EAPI Eina_Bool etui_module_register(const Etui_Provider_Descriptor *provider);
EAPI Eina_Bool etui_module_unregister(const Etui_Provider_Descriptor *provider);

Etui_Provider_Instance *etui_provider_instance_new(const char *name,
                                                   Evas *evas);
void etui_provider_instance_del(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_name_equal(const Etui_Provider_Instance *inst,
                                            const char *name);
void *etui_provider_instance_data_get(const Etui_Provider_Instance *inst);

/* private calls */

Evas_Object *etui_provider_instance_evas_object_get(Etui_Provider_Instance *inst);

Eina_Bool etui_provider_instance_file_open(Etui_Provider_Instance *inst,
                                           const char *file_name);
void etui_provider_instance_file_close(Etui_Provider_Instance *inst);
void etui_provider_instance_version_get(Etui_Provider_Instance *inst,
                                        int *maj,
                                        int *min);
char *etui_provider_instance_title_get(Etui_Provider_Instance *inst);
char *etui_provider_instance_author_get(Etui_Provider_Instance *inst);
char *etui_provider_instance_subject_get(Etui_Provider_Instance *inst);
char *etui_provider_instance_keywords_get(Etui_Provider_Instance *inst);
char *etui_provider_instance_creator_get(Etui_Provider_Instance *inst);
char *etui_provider_instance_producer_get(Etui_Provider_Instance *inst);
char *etui_provider_instance_creation_date_get(Etui_Provider_Instance *inst);
char *etui_provider_instance_modification_date_get(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_is_printable(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_is_changeable(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_is_copyable(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_is_notable(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_password_needed(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_password_set(Etui_Provider_Instance *inst,
                                              const char *password);
int etui_provider_instance_pages_count(Etui_Provider_Instance *inst);
const Eina_Array *etui_provider_instance_toc_get(Etui_Provider_Instance *inst);

void etui_provider_instance_page_use_display_list_set(Etui_Provider_Instance *inst,
                                                      Eina_Bool on);
Eina_Bool etui_provider_instance_page_use_display_list_get(Etui_Provider_Instance *inst);

Eina_Bool etui_provider_instance_page_set(Etui_Provider_Instance *inst,
                                          int page_num);
int etui_provider_instance_page_get(Etui_Provider_Instance *inst);
void etui_provider_instance_page_size_get(Etui_Provider_Instance *inst,
                                          int *width,
                                          int *height);

Eina_Bool etui_provider_instance_page_rotation_set(Etui_Provider_Instance *inst,
                                                   Etui_Rotation rotation);
Etui_Rotation etui_provider_instance_page_rotation_get(Etui_Provider_Instance *inst);

Eina_Bool etui_provider_instance_page_scale_set(Etui_Provider_Instance *inst,
                                                float hscale,
                                                float vscale);
void etui_provider_instance_page_scale_get(Etui_Provider_Instance *inst,
                                           float *hscale,
                                           float *vscale);
const Eina_Array *etui_provider_instance_page_links_get(Etui_Provider_Instance *inst);

void etui_provider_instance_page_render_pre(Etui_Provider_Instance *inst);
void etui_provider_instance_page_render(Etui_Provider_Instance *inst);
void etui_provider_instance_page_render_end(Etui_Provider_Instance *inst);

char *etui_provider_instance_page_text_extract(Etui_Provider_Instance *inst, const Eina_Rectangle *rect);
Eina_Array *etui_provider_instance_page_text_find(Etui_Provider_Instance *inst, const char *needle);

float etui_provider_instance_page_duration_get(Etui_Provider_Instance *inst);
Etui_Transition etui_provider_instance_page_transition_type_get(Etui_Provider_Instance *inst);
float etui_provider_instance_page_transition_duration_get(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_page_transition_vertical_get(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_page_transition_outwards_get(Etui_Provider_Instance *inst);
int etui_provider_instance_page_transition_direction_get(Etui_Provider_Instance *inst);

#endif
