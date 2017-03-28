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

#include "etui_config.h"
#include "private.h"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/


#define ETUI_CONFIG_EDD_KEY_ADD(_s, _m, _t) \
    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_keys, Etui_Config_Keys, _s, _m, _t)

#define ADD_KEYS(Name, Ctrl, Alt, Shift, Win, Cb) \
    kb = calloc(1, sizeof(Etui_Config_Keys)); \
    if (!kb) return; \
    kb->keyname = eina_stringshare_add_length(Name, strlen(Name)); \
    kb->ctrl = Ctrl; \
    kb->alt = Alt; \
    kb->shift = Shift; \
    kb->win = Win; \
    kb->cb = eina_stringshare_add_length(Cb, strlen(Cb)); \
    config->keys = eina_list_append(config->keys, kb)

static Eet_Data_Descriptor *edd_base = NULL;
static Eet_Data_Descriptor *edd_keys = NULL;

static void
_etui_default_keys_add(Etui_Config *config)
{
    Etui_Config_Keys *kb;

    ADD_KEYS("F11", 0, 0, 0, 0, "win_fullscreen");
}

static Etui_Config *
_etui_config_new(void)
{
   Etui_Config *config;

   config = (Etui_Config *)calloc(1, sizeof(Etui_Config));
   if (!config)
       return NULL;

   config->theme = eina_stringshare_add("default.edj");
   config->cg_width = 480;
   config->cg_height = 640;
   config->custom_geometry = EINA_FALSE;
   _etui_default_keys_add(config);

   return config;
}

static const char *
_etui_config_theme_path_get(const char *name)
{
    static char path1[PATH_MAX];
    static char path2[PATH_MAX];
    struct stat s;

    snprintf(path2, sizeof(path2) - 1, "%s/.etui/themes/%s",
             eina_environment_home_get(), name);
    if (stat(path2, &s) == 0)
        return path2;
    snprintf(path1, sizeof(path1) - 1, "%s/themes/%s",
             elm_app_data_dir_get(), name);
    return path1;
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/


void
etui_config_init(void)
{
    char path[PATH_MAX];
    Eet_Data_Descriptor_Class eddc;
    Eet_Data_Descriptor_Class eddkc;

    snprintf(path, sizeof(path), "%s/.etui/themes",
             eina_environment_home_get());
    ecore_file_mkpath(path);

    eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc),
                                              "Config", sizeof(Etui_Config));
    edd_base = eet_data_descriptor_stream_new(&eddc);

    EET_DATA_DESCRIPTOR_ADD_LIST(edd_base, Etui_Config, "keys", keys, edd_keys);

    eet_eina_stream_data_descriptor_class_set(&eddkc, sizeof(eddkc),
                                              "Config_Keys", sizeof(Etui_Config_Keys));
    edd_keys = eet_data_descriptor_stream_new(&eddkc);

    ETUI_CONFIG_EDD_KEY_ADD("keyname", keyname, EET_T_STRING);
    ETUI_CONFIG_EDD_KEY_ADD("ctrl", ctrl, EET_T_UCHAR);
    ETUI_CONFIG_EDD_KEY_ADD("alt", alt, EET_T_UCHAR);
    ETUI_CONFIG_EDD_KEY_ADD("shift", shift, EET_T_UCHAR);
    ETUI_CONFIG_EDD_KEY_ADD("win", win, EET_T_UCHAR);
    ETUI_CONFIG_EDD_KEY_ADD("meta", meta, EET_T_UCHAR);
    ETUI_CONFIG_EDD_KEY_ADD("hyper", hyper, EET_T_UCHAR);
    ETUI_CONFIG_EDD_KEY_ADD("cb", cb, EET_T_STRING);

    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Etui_Config, "theme", theme, EET_T_STRING);
    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Etui_Config, "cg_width", cg_width, EET_T_INT);
    EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Etui_Config, "cg_height", cg_height, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(edd_base, Etui_Config, "custom_geometry", custom_geometry, EET_T_UCHAR);
    EET_DATA_DESCRIPTOR_ADD_LIST(edd_base, Etui_Config, "keys", keys, edd_keys);
}

void
etui_config_shutdown(void)
{
    eet_data_descriptor_free(edd_base);
    eet_data_descriptor_free(edd_keys);
}

Etui_Config *
etui_config_load(const char *key)
{
    char buf[PATH_MAX];
    Eet_File *ef;
    Etui_Config *config = NULL;

    EINA_SAFETY_ON_NULL_RETURN_VAL(key, NULL);

    snprintf(buf, sizeof(buf), "%s/.etui/config/base.cfg",
             eina_environment_home_get());
    buf[sizeof(buf) - 1] = '\0';
    ef = eet_open(buf, EET_FILE_MODE_READ);
    if (ef)
    {
        config = eet_data_read(ef, edd_base, key);
        eet_close(ef);
        if (eina_list_count(config->keys) == 0)
        {
            _etui_default_keys_add(config);
        }
    }

    if (!config)
        config = _etui_config_new();

    if (config)
        config->config_key = eina_stringshare_add(key); /* not in eet */

    return config;
}

void
etui_config_del(Etui_Config *config)
{
    Etui_Config_Keys *key;

    if (!config)
        return;

    eina_stringshare_del(config->theme);

    EINA_LIST_FREE(config->keys, key)
    {
        eina_stringshare_del(key->keyname);
        eina_stringshare_del(key->cb);
        free(key);
     }

    eina_stringshare_del(config->config_key);
    free(config);
}

void
etui_config_save(Etui_Config *config, const char *key)
{
    char buf[PATH_MAX];
    char buf2[PATH_MAX];
    Eet_File *ef;

    EINA_SAFETY_ON_NULL_RETURN(config);

    if (!key) key = config->config_key;
    snprintf(buf, sizeof(buf), "%s/.etui/config/standard",
             eina_environment_home_get());
    ecore_file_mkpath(buf);
    snprintf(buf, sizeof(buf), "%s/.etui/config/standard/base.cfg.tmp",
             eina_environment_home_get());
    snprintf(buf2, sizeof(buf2), "%s/.etui/config/standard/base.cfg",
             eina_environment_home_get());
    ef = eet_open(buf, EET_FILE_MODE_WRITE);
    if (ef)
    {
        int ok;

        ok = eet_data_write(ef, edd_base, key, config, 1);
        eet_close(ef);
        if (ok) ecore_file_mv(buf, buf2);
    }
}

const char *
etui_config_theme_path_get(const Etui_Config *config)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(config, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(config->theme, NULL);

    if (strchr(config->theme, '/'))
        return config->theme;

   return _etui_config_theme_path_get(config->theme);
}

const char *
etui_config_theme_path_default_get(const Etui_Config *config)
{
    static char path[PATH_MAX];

   EINA_SAFETY_ON_NULL_RETURN_VAL(config, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(config->theme, NULL);

   *path = '\0';
   if (path[0]) return path;

   snprintf(path, sizeof(path), "%s/themes/default.edj",
            elm_app_data_dir_get());

   return path;
}


/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
