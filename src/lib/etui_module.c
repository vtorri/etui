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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Eina.h>
#include <Evas.h>

#include "Etui.h"
#include "etui_private.h"
#include "etui_module.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#define ETUI_PROVIDER_INSTANCE_CHECK(inst, meth, ...) \
do                                                    \
{                                                     \
    if (!inst)                                        \
    {                                                 \
        DBG("no instance to call "#meth);             \
            return __VA_ARGS__;                       \
    }                                                 \
    if (!inst->provider->meth)                        \
    {                                                 \
        DBG("no "#meth" in instance=%p", inst);       \
        return __VA_ARGS__;                           \
    }                                                 \
} while (0)

#define ETUI_PROVIDER_INSTANCE_CALL(inst, meth, ...)  \
do                                                    \
{                                                     \
    ETUI_PROVIDER_INSTANCE_CHECK(inst, meth);         \
    inst->provider->meth(inst->data, ## __VA_ARGS__); \
} while (0)

#define ETUI_PROVIDER_INSTANCE_CALL_RET(inst, meth, retval, ...) \
do                                                               \
{                                                                \
    ETUI_PROVIDER_INSTANCE_CHECK(inst, meth, retval);            \
    return inst->provider->meth(inst->data, ## __VA_ARGS__);     \
} while (0)

typedef struct _Etui_Provider_Registry_Entry Etui_Provider_Registry_Entry;

struct _Etui_Provider_Registry_Entry
{
    const Etui_Provider_Descriptor *provider;
    int priority;
};

struct _Etui_Provider_Instance
{
    const Etui_Provider_Descriptor *provider;
    Evas *evas;
    void *data;
};

static Eina_Array *_etui_modules = NULL;
static Eina_Hash *_etui_providers = NULL;
static Eina_List *_etui_provider_registries = NULL;
static Eina_Bool _etui_modules_loaded = EINA_FALSE;

/* Registry functions */

static void
_etui_provider_registry_entry_free(Etui_Provider_Registry_Entry *re)
{
    free(re);
}

static int
_etui_provider_registry_entry_cmp(const void *pa, const void *pb)
{
    const Etui_Provider_Registry_Entry *a, *b;
    int r;

    a = (Etui_Provider_Registry_Entry *)pa;
    b = (Etui_Provider_Registry_Entry *)pb;
    r = a->priority - b->priority;

    if (r == 0)
        /* guarantee some order to ease debug */
        r = strcmp(a->provider->name, b->provider->name);

    return r;
}
static const Etui_Provider_Descriptor *
_etui_provider_registry_find(const char *name)
{
    const Eina_List *n;
    const Etui_Provider_Registry_Entry *re;

    EINA_LIST_FOREACH(_etui_provider_registries, n, re)
    {
        if (strcmp(re->provider->name, name) == 0)
            return re->provider;
    }

    return NULL;
}

static Etui_Provider_Instance *
_etui_provider_instance_new(const Etui_Provider_Descriptor *provider,
                            Evas                           *evas,
                            void                           *data)
{
    Etui_Provider_Instance *inst;

    inst = calloc(1, sizeof(Etui_Provider_Instance));
    EINA_SAFETY_ON_NULL_GOTO(inst, del_data);
    inst->provider = provider;
    inst->evas = evas;
    inst->data = data;

    return inst;

  del_data:
    provider->shutdown(data);
    return NULL;
}

/* modules functions */

static Eina_Bool
_etui_modules_load(void)
{
    char buf[PATH_MAX];
    char *path;
    Eina_Prefix *prefix;

    if (_etui_modules_loaded)
        return EINA_TRUE;

    _etui_modules_loaded = EINA_TRUE;

    path = eina_module_environment_path_get("HOME", "/.etui/modules");
    if (path)
    {
        _etui_modules = eina_module_arch_list_get(_etui_modules, path, MODULE_ARCH);
        free(path);
    }

    path = eina_module_environment_path_get("ETUI_MODULES_DIR", "/etui/modules");
    if (path)
    {
        _etui_modules = eina_module_arch_list_get(_etui_modules, path, MODULE_ARCH);
        free(path);
    }

    prefix = eina_prefix_new(NULL, etui_init,
                             "ETUI", "etui", "checkme",
                             PACKAGE_BIN_DIR, PACKAGE_LIB_DIR,
                             PACKAGE_DATA_DIR, PACKAGE_DATA_DIR);
    if (prefix)
    {
        snprintf(buf, sizeof(buf),
                 "%s/etui/modules", eina_prefix_lib_get(prefix));
        _etui_modules = eina_module_arch_list_get(_etui_modules, buf, MODULE_ARCH);
        eina_prefix_free(prefix);
    }
    else
    {
        INF("Could not create prefix, use library path directly.");
        _etui_modules = eina_module_arch_list_get(_etui_modules, PACKAGE_LIB_DIR"/etui/modules", MODULE_ARCH);
    }

    path = eina_module_symbol_path_get(etui_object_add, "/etui/modules");
    if (path)
    {
        _etui_modules = eina_module_list_get(_etui_modules, path, 0, NULL, NULL);
        free(path);
    }

    if (!_etui_modules)
    {
        ERR("Could not find modules.");
        return EINA_FALSE;
    }

    eina_module_list_load(_etui_modules);

    /* TODO: registry */

    return EINA_TRUE;
}

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

#ifdef ETUI_BUILD_STATIC_PDF
Eina_Bool etui_module_pdf_init(void);
void etui_module_pdf_shutdown(void);
#endif

Eina_Bool
etui_modules_init(void)
{
    _etui_providers = eina_hash_string_superfast_new(NULL);
    if (!_etui_providers)
    {
        ERR("Could not allocate providers hash table.");
        return EINA_FALSE;
    }

    /* TODO : STATIC modules */
#ifdef ETUI_BUILD_STATIC_PDF
    etui_module_pdf_init();
#endif

    return EINA_TRUE;
}

void
etui_modules_shutdown(void)
{
    Etui_Provider_Registry_Entry *re;

    /* TODO : STATIC modules */
#ifdef ETUI_BUILD_STATIC_PDF
    etui_module_pdf_shutdown();
#endif

    if (_etui_modules)
    {
        eina_module_list_free(_etui_modules);
        eina_array_free(_etui_modules);
        _etui_modules = NULL;
    }

	eina_hash_free(_etui_providers);

    EINA_LIST_FREE(_etui_provider_registries, re)
    {
        WRN("Engine was not unregistered: %p", re->provider);
        _etui_provider_registry_entry_free(re);
    }

    _etui_modules_loaded = EINA_FALSE;
}

EAPI Eina_Bool
etui_module_register(const Etui_Provider_Descriptor *provider)
{
    Etui_Provider_Registry_Entry *re;

    EINA_SAFETY_ON_NULL_RETURN_VAL(provider, EINA_FALSE);

    if (provider->version != ETUI_PROVIDER_DESCRIPTOR_VERSION)
    {
        ERR("Module '%p' uses api version=%u while %u was expected",
            provider, provider->version, ETUI_PROVIDER_DESCRIPTOR_VERSION);
        return EINA_FALSE;
    }

    EINA_SAFETY_ON_NULL_RETURN_VAL(provider->name, EINA_FALSE);

    INF("register name=%s, version=%u, priority=%d, provider=%p",
        provider->name, provider->version, provider->priority, provider);

    re = (Etui_Provider_Registry_Entry *)calloc(1, sizeof(Etui_Provider_Registry_Entry));
    EINA_SAFETY_ON_NULL_RETURN_VAL(re, EINA_FALSE);

    re->provider = provider;
    re->priority = provider->priority; // TODO: use user-priority from file as well.

    _etui_provider_registries = eina_list_sorted_insert(_etui_provider_registries,
                                                        _etui_provider_registry_entry_cmp,
                                                        re);

    return EINA_TRUE;
}

EAPI Eina_Bool
etui_module_unregister(const Etui_Provider_Descriptor *provider)
{
    Eina_List *n;
    Etui_Provider_Registry_Entry *re;

    EINA_SAFETY_ON_NULL_RETURN_VAL(provider, EINA_FALSE);
    if (provider->version != ETUI_PROVIDER_DESCRIPTOR_VERSION)
    {
        ERR("Module '%p' uses provider version=%u while %u was expected",
            provider, provider->version, ETUI_PROVIDER_DESCRIPTOR_VERSION);
        return EINA_FALSE;
    }

    INF("unregister name=%s, provider=%p", provider->name, provider);

    EINA_LIST_FOREACH(_etui_provider_registries, n, re)
    {
        if (re->provider == provider)
        {
            _etui_provider_registry_entry_free(re);
            _etui_provider_registries = eina_list_remove_list(_etui_provider_registries, n);
            return EINA_TRUE;
        }
    }

    ERR("module not registered name=%s, provider=%p", provider->name, provider);
    return EINA_FALSE;
}


Etui_Provider_Instance *
etui_provider_instance_new(const char *name,
                           Evas       *evas)
{
    const Eina_List *n;
    const Etui_Provider_Registry_Entry *re;
    const Etui_Provider_Descriptor *provider;
    void *data;

    if (!_etui_modules_load())
        return NULL;

    if ((!name) && getenv("ETUI_PROVIDER"))
    {
        name = getenv("ETUI_PROVIDER");
        DBG("using ETUI_PROVIDER=%s", name);
    }

    if (name)
    {
        provider = _etui_provider_registry_find(name);
        if (!provider)
            ERR("Couldn't find requested provider: %s. Try fallback", name);
        else
        {
            data = provider->init(evas);
            if (data)
            {
                INF("Using requested provider %s, data=%p", name, data);
                return _etui_provider_instance_new(provider, evas, data);
            }

            ERR("Requested provider '%s' could not be used. Try fallback", name);
        }
    }

    EINA_LIST_FOREACH(_etui_provider_registries, n, re)
    {
        provider = re->provider;
        DBG("Trying provider %s, priority=%d (%d)",
            provider->name, re->priority, provider->priority);

        data = provider->init(evas);
        if (data)
        {
            INF("Using fallback provider %s, data=%p", provider->name, data);
            return _etui_provider_instance_new(provider, evas, data);
        }
    }

    ERR("No provider worked");
    return NULL;
}

void
etui_provider_instance_del(Etui_Provider_Instance *inst)
{
    EINA_SAFETY_ON_NULL_RETURN(inst);
    inst->provider->shutdown(inst->data);
}

Eina_Bool
etui_provider_instance_name_equal(const Etui_Provider_Instance *inst, const char *name)
{
    /* these are valid, no safety macros here */
    if (!name) return EINA_FALSE;
    if (!inst) return EINA_FALSE;

    return strcmp(name, inst->provider->name) == 0;
}

void *
etui_provider_instance_data_get(const Etui_Provider_Instance *inst)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(inst, NULL);

    return inst->data;
}

/* private calls */

Eina_Bool
etui_provider_instance_file_open(Etui_Provider_Instance *inst,
                                 const char *filename)
{
    ETUI_PROVIDER_INSTANCE_CALL_RET(inst, file_open, EINA_FALSE, filename);
}

void
etui_provider_instance_file_close(Etui_Provider_Instance *inst)
{
    ETUI_PROVIDER_INSTANCE_CALL(inst, file_close);
}

Eina_Bool
etui_provider_instance_password_needed(Etui_Provider_Instance *inst)
{
    ETUI_PROVIDER_INSTANCE_CALL_RET(inst, password_needed, EINA_FALSE);
}

Eina_Bool
etui_provider_instance_password_set(Etui_Provider_Instance *inst,
                                    const char *password)
{
    ETUI_PROVIDER_INSTANCE_CALL_RET(inst, password_set, EINA_FALSE, password);
}

int
etui_provider_instance_pages_count(Etui_Provider_Instance *inst)
{
    ETUI_PROVIDER_INSTANCE_CALL_RET(inst, pages_count, -1);
}

void
etui_provider_instance_page_set(Etui_Provider_Instance *inst, int page_num)
{
    ETUI_PROVIDER_INSTANCE_CALL(inst, page_set, page_num);
}

int
etui_provider_instance_page_get(Etui_Provider_Instance *inst)
{
    ETUI_PROVIDER_INSTANCE_CALL_RET(inst, page_get, -1);
}

void
etui_provider_instance_page_size_get(Etui_Provider_Instance *inst, int *width, int *height)
{
    /* FIXME: if error, set the size to 0 */
    ETUI_PROVIDER_INSTANCE_CHECK(inst, page_size_get);

    inst->provider->page_size_get(inst->data, width, height);
}

void
etui_provider_instance_rotation_set(Etui_Provider_Instance *inst,
                                    Etui_Rotation rotation)
{
    ETUI_PROVIDER_INSTANCE_CALL(inst, rotation_set, rotation);
}

Etui_Rotation
etui_provider_instance_rotation_get(Etui_Provider_Instance *inst)
{
    ETUI_PROVIDER_INSTANCE_CALL_RET(inst, rotation_get, ETUI_ROTATION_0);
}

void
etui_provider_instance_scale_set(Etui_Provider_Instance *inst,
                                 float hscale,
                                 float vscale)
{
    ETUI_PROVIDER_INSTANCE_CALL(inst, scale_set, hscale, vscale);
}

void
etui_provider_instance_scale_get(Etui_Provider_Instance *inst,
                                 float *hscale,
                                 float *vscale)
{
    /* FIXME: if error, set the scales to 1.0f */
    ETUI_PROVIDER_INSTANCE_CHECK(inst, scale_get);

    inst->provider->scale_get(inst->data, hscale, vscale);
}

void
etui_provider_instance_render(Etui_Provider_Instance *inst)
{
    ETUI_PROVIDER_INSTANCE_CALL(inst, render);
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
