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

#ifndef ETUI_BIN_CONFIG_H
#define ETUI_BIN_CONFIG_H


typedef struct _Etui_Config_Keys Etui_Config_Keys;

struct _Etui_Config_Keys
{
    const char *keyname;
    Eina_Bool   ctrl;
    Eina_Bool   alt;
    Eina_Bool   shift;
    Eina_Bool   win;
    Eina_Bool   meta;
    Eina_Bool   hyper;
    const char *cb;
};

struct _Etui_Config
{
    const char *theme;
    int         cg_width;
    int         cg_height;
    Eina_Bool   custom_geometry;
    Eina_List  *keys;

    const char *config_key;
};

void etui_config_init(void);
void etui_config_shutdown(void);
Etui_Config *etui_config_load(const char *key);
void etui_config_del(Etui_Config *config);

const char *etui_config_theme_path_get(const Etui_Config *config);
const char *etui_config_theme_path_default_get(const Etui_Config *config);


#endif /* ETUI_BIN_CONFIG_H */
