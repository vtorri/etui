/* Etui - Multi-document rendering library using the EFL
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

#ifndef ETUI_MODULE_H
#define ETUI_MODULE_H


/* Used at the end of module source file for static linking */
#define ETUI_MODULE_DEFINE(Name) \
    Eina_Bool etui_##Name##_init(void);  \
    Eina_Bool etui_##Name##_init(void) \
    { \
        return etui_module_register(&_etui_modapi); \
    } \
    void etui_##Name##_shutdown(void); \
    void etui_##Name##_shutdown(void) \
    { \
        etui_module_unregister(&_etui_modapi); \
    }

/* Used at the end of module source file for non-static linking */
#define ETUI_EINA_MODULE_DEFINE(Name) \
    EINA_MODULE_INIT(etui_##Name##_init); \
    EINA_MODULE_SHUTDOWN(etui_##Name##_shutdown);


typedef struct _Etui_Module_Func Etui_Module_Func;
typedef struct _Etui_Module_Api Etui_Module_Api;
typedef struct _Etui_Module Etui_Module;

struct _Etui_Module_Func
{
    void             *(*init)(const Etui_File *ef);
    void              (*shutdown)(void *d);
    Evas_Object      *(*evas_object_add)(void *d, Evas *evas);
    void              (*evas_object_del)(void *d);
    const void       *(*info_get)(void *d);
    const char       *(*title_get)(void *d);
    int               (*pages_count)(void *d);
    const Eina_Array *(*toc_get)(void *d);
    Eina_Bool         (*page_set)(void *d, int num);
    int               (*page_get)(void *d);
    void              (*page_size_get)(void *d, int *width, int *height);
    Eina_Bool         (*page_rotation_set)(void *d, Etui_Rotation rotation);
    Etui_Rotation     (*page_rotation_get)(void *d);
    Eina_Bool         (*page_scale_set)(void *d, double scale);
    double            (*page_scale_get)(void *d);
    void              (*page_render_pre)(void *d);
    void              (*page_render)(void *d);
    void              (*page_render_end)(void *d);
    const void       *(*api_get)(void *d);
};

struct _Etui_Module_Api
{
    const char *name;

    struct
    {
        Eina_Bool (*open)(Etui_Module *);
        void (*close)(Etui_Module *);
    } func;
};

struct _Etui_Module
{
    const Etui_Module_Api *definition;
    Etui_Module_Func *functions; /* functions exported by the module */
    int ref; /* how many refs */
    void *data; /* data returned by functions->init() */
    Ecore_Thread *render; /* render thread for functions->page_render*() */
    unsigned char loaded : 1;
};

Eina_Bool etui_module_init(void);
void etui_module_shutdown(void);

Eina_Bool etui_module_load(Etui_Module *em);
void etui_module_unload(Etui_Module *em);

Etui_Module *etui_module_find(const char *name);
Eina_List *etui_module_list(void);

EAPI Eina_Bool etui_module_register(const Etui_Module_Api *module);
EAPI Eina_Bool etui_module_unregister(const Etui_Module_Api *module);


#endif /* ETUI_MODULE_H */
