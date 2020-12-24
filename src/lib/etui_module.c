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

#include <config.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <Eina.h>
#include <Ecore.h> /* for Ecore_Thread in Etui_Module */

#include "Etui.h"
#include "etui_private.h"
#include "etui_module.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#define ETUI_EINA_STATIC_MODULE_DEFINE(name) \
    Eina_Bool etui_##name##_init(void); \
    void etui_##name##_shutdown(void)

#define ETUI_EINA_STATIC_MODULE_USE(name) \
    { etui_##name##_init, etui_##name##_shutdown }

static Eina_Hash *_etui_modules = NULL;
static Eina_List *_etui_module_paths = NULL;
static Eina_Prefix *_etui_prefix = NULL;


#ifdef ETUI_BUILD_STATIC_CB
ETUI_EINA_STATIC_MODULE_DEFINE(cb);
#endif

#ifdef ETUI_BUILD_STATIC_DJVU
ETUI_EINA_STATIC_MODULE_DEFINE(djvu);
#endif

#ifdef ETUI_BUILD_STATIC_PDF
ETUI_EINA_STATIC_MODULE_DEFINE(pdf);
#endif

#ifdef ETUI_BUILD_STATIC_PS
ETUI_EINA_STATIC_MODULE_DEFINE(ps);
#endif

#ifdef ETUI_BUILD_STATIC_TIFF
ETUI_EINA_STATIC_MODULE_DEFINE(tiff);
#endif

static const struct
{
    Eina_Bool (*init)(void);
    void (*shutdown)(void);
} _etui_static_module[] =
{
#ifdef ETUI_BUILD_STATIC_CB
    ETUI_EINA_STATIC_MODULE_USE(cb),
#endif
#ifdef ETUI_BUILD_STATIC_DJVU
    ETUI_EINA_STATIC_MODULE_USE(djvu),
#endif
#ifdef ETUI_BUILD_STATIC_PDF
    ETUI_EINA_STATIC_MODULE_USE(pdf),
#endif
#ifdef ETUI_BUILD_STATIC_PS
    ETUI_EINA_STATIC_MODULE_USE(ps),
#endif
#ifdef ETUI_BUILD_STATIC_TIFF
    ETUI_EINA_STATIC_MODULE_USE(tiff),
#endif
    { NULL, NULL}
};

static Eina_Bool
_etui_path_is_file(const char *path)
{
    struct stat st;

    if (!path || !*path)
        return EINA_FALSE;

    if (stat(path, &st) == -1)
    {
        ERR("stat failed on %s : %s", path, strerror(errno));
        return EINA_FALSE;
    }

    if (S_ISREG(st.st_mode))
        return EINA_TRUE;

    ERR("path %s is not a regular file", path);
    return EINA_FALSE;
}

static Eina_Bool
_etui_module_close_cb(const Eina_Hash *hash EINA_UNUSED,
                      const void *key EINA_UNUSED,
                      void *data,
                      void *fdata EINA_UNUSED)
{
    Etui_Module *em;

    if (!data)
        return EINA_FALSE;

    em = (Etui_Module *)data;

    if (em->loaded)
    {
        em->definition->func.close(em);
        em->loaded = 0;
    }

    return EINA_TRUE;
}

static Eina_List *
_etui_module_append(Eina_List *list, char *path)
{
    struct stat st;

    if (!path || !*path)
        return list;

    if (stat(path, &st) == 0)
        list = eina_list_append(list, path);
    else
        free(path);

    return list;
}

static void
_etui_module_paths_init(void)
{
    char *path = NULL;

    if (!_etui_prefix)
        _etui_prefix = eina_prefix_new(NULL,
                                       _etui_module_paths_init,
                                       "ETUI", "etui", "checkme",
                                       PACKAGE_BIN_DIR,
                                       PACKAGE_LIB_DIR,
                                       PACKAGE_DATA_DIR,
                                       PACKAGE_DATA_DIR);

    if (_etui_prefix)
    {
        const char *libdir;

        libdir = eina_prefix_lib_get(_etui_prefix);
        if (libdir)
        {
            size_t l;

            l = strlen(libdir);
            path = malloc(l + sizeof("/etui/modules"));
            if (path)
            {
                memcpy(path, libdir, l);
                memcpy(path + l, "/etui/modules", sizeof("/etui/modules"));
            }
        }
    }
    else
        path = eina_module_symbol_path_get(_etui_module_paths_init, "/etui/modules");

    if (path)
    {
        if (eina_list_search_unsorted(_etui_module_paths,
                                      (Eina_Compare_Cb)strcmp, path))
            free(path);
        else
            _etui_module_paths = _etui_module_append(_etui_module_paths, path);
    }

    path = PACKAGE_LIB_DIR "/etui/modules";
    if (!eina_list_search_unsorted(_etui_module_paths,
                                   (Eina_Compare_Cb)strcmp, path))
    {
        path = strdup(path);
        if (path)
            _etui_module_paths = _etui_module_append(_etui_module_paths, path);
    }
}


/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


Eina_Bool
etui_module_init(void)
{
    int i;

    _etui_module_paths_init();

    _etui_modules = eina_hash_string_small_new(NULL);
    if (!_etui_modules)
    {
        ERR("can not create hash\n");
        return EINA_FALSE;
    }

    for (i = 0; _etui_static_module[i].init; ++i)
        _etui_static_module[i].init();

    return EINA_TRUE;
}

void
etui_module_shutdown(void)
{
    char *path;
    int i;

    for (i = 0; _etui_static_module[i].shutdown; ++i)
        _etui_static_module[i].shutdown();

    eina_hash_foreach(_etui_modules, _etui_module_close_cb, NULL);
    eina_hash_free(_etui_modules);
    _etui_modules = NULL;

    EINA_LIST_FREE(_etui_module_paths, path)
        free(path);

    if (_etui_prefix)
    {
        eina_prefix_free(_etui_prefix);
        _etui_prefix = NULL;
    }
}

Eina_Bool
etui_module_register(const Etui_Module_Api *module)
{
    Etui_Module *em;

    if (!module)
        return EINA_FALSE;

    em = eina_hash_find(_etui_modules, module->name);
    if (em)
        return EINA_FALSE;

    em = calloc(1, sizeof (Etui_Module));
    if (!em)
        return EINA_FALSE;

    em->definition = module;

    eina_hash_direct_add(_etui_modules, module->name, em);

    return EINA_TRUE;
}

Eina_Bool
etui_module_unregister(const Etui_Module_Api *module)
{
    Etui_Module *em;

    if (!module)
        return EINA_FALSE;

    em = eina_hash_find(_etui_modules, module->name);
    if (!em || em->definition != module)
        return EINA_FALSE;

    eina_hash_del(_etui_modules, module->name, em);

    if (em->loaded)
    {
        em->definition->func.close(em);
        em->loaded = 0;
    }

    free(em);

    return EINA_TRUE;
}

Eina_Bool
etui_module_load(Etui_Module *em)
{
    if (!em)
        return EINA_FALSE;

    if (em->loaded)
        return EINA_TRUE;

    if (!em->definition)
        return EINA_FALSE;

    if (!em->definition->func.open(em))
        return EINA_FALSE;

    em->loaded = 1;

    return EINA_TRUE;
}

void
etui_module_unload(Etui_Module *em)
{
    if (!em || !em->loaded || !em->definition)
        return;

    em->definition->func.close(em);
    em->loaded = 0;
}

Etui_Module *
etui_module_find(const char *name)
{
    Etui_Module *em;
    Eina_List *l;
    const char *path;

    if (!name || !*name)
        return NULL;

    em = eina_hash_find(_etui_modules, name);
    if (em)
    {
        if (etui_module_load(em))
            return em;

        return NULL;
    }

    EINA_LIST_FOREACH(_etui_module_paths, l, path)
    {
        char buffer[PATH_MAX];
        Eina_Module *mod;

        snprintf(buffer, sizeof(buffer), "%s/%s/%s/%s",
                 path, name, MODULE_ARCH, "libmodule." MODULE_EXT);

        if (!_etui_path_is_file(buffer))
            continue;

        mod = eina_module_new(buffer);
        if (!mod)
            continue;

        if (!eina_module_load(mod))
        {
            eina_module_free(mod);
            continue;
        }

        em = eina_hash_find(_etui_modules, name);
        if (em && etui_module_load(em))
            return em;

        eina_module_free(mod);
    }

    return NULL;
}

Eina_List *
etui_module_list(void)
{
    Eina_List *r = NULL, *l, *ll;
    Eina_Iterator *it, *it2;
    const char *s, *s2;
    char buf[PATH_MAX];

    EINA_LIST_FOREACH(_etui_module_paths, l, s)
    {
        it = eina_file_direct_ls(s);
        if (it)
        {
            Eina_File_Direct_Info *fi;

            EINA_ITERATOR_FOREACH(it, fi)
            {
                const char *fname = fi->path + fi->name_start;

                snprintf(buf, sizeof(buf), "%s/%s/%s",
                         s, fname, MODULE_ARCH);

                it2 = eina_file_ls(buf);
                if (it2)
                {
                    EINA_LIST_FOREACH(r, ll, s2)
                    {
                        if (!strcmp(fname, s2)) break;
                    }
                    if (!ll)
                        r = eina_list_append(r, eina_stringshare_add(fname));
                    eina_iterator_free(it2);
                }
            }
            eina_iterator_free(it);
        }
    }

    return r;
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
