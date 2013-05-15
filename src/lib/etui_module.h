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
    void         *(*init)(Evas *evas);
    void          (*shutdown)(void *d);
    Evas_Object  *(*evas_object_get)(void *d);
    Eina_Bool     (*file_open)(void *d, const char *filename);
    void          (*file_close)(void *d);
    Eina_Bool     (*password_needed)(void *d);
    Eina_Bool     (*password_set)(void *d, const char *password);
    int           (*pages_count)(void *d);
    void          (*page_set)(void *d, int num);
    int           (*page_get)(void *d);
    void          (*page_size_get)(void *d, int *width, int *height);
    void          (*rotation_set)(void *d, Etui_Rotation rotation);
    Etui_Rotation (*rotation_get)(void *d);
    void          (*scale_set)(void *d, float hscale, float vscale);
    void          (*scale_get)(void *d, float *hscale, float *vscale);
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

Eina_Bool etui_provider_instance_file_open(Etui_Provider_Instance *inst,
                                           const char *file_name);
void etui_provider_instance_file_close(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_password_needed(Etui_Provider_Instance *inst);
Eina_Bool etui_provider_instance_password_set(Etui_Provider_Instance *inst,
                                              const char *password);
int etui_provider_instance_pages_count(Etui_Provider_Instance *inst);

void etui_provider_instance_page_set(Etui_Provider_Instance *inst, int page_num);
int etui_provider_instance_page_get(Etui_Provider_Instance *inst);
void etui_provider_instance_page_size_get(Etui_Provider_Instance *inst, int *width, int *height);

void etui_provider_instance_rotation_set(Etui_Provider_Instance *inst, Etui_Rotation rotation);
Etui_Rotation etui_provider_instance_rotation_get(Etui_Provider_Instance *inst);

void etui_provider_instance_scale_set(Etui_Provider_Instance *inst, float hscale, float vscale);
void etui_provider_instance_scale_get(Etui_Provider_Instance *inst, float *hscale, float *vscale);

#endif
