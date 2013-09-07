/* Etui - Multi-document rendering library using the EFL
 * Copyright (C) 2013 Vincent Torri
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ETUI_MODULE_IMG_TAR_H
#define ETUI_MODULE_IMG_TAR_H

typedef struct _Etui_Img_Cbt_Header Etui_Img_Cbt_Header;
typedef struct _Etui_Img_Cbt_Entry Etui_Img_Cbt_Entry;
typedef struct _Etui_Img_Cbt Etui_Img_Cbt;

struct _Etui_Img_Cbt_Header
{
    char *file_name;
    char file_mode[8];
    unsigned long long owner_id;
    unsigned long long group_id;
    unsigned long long file_size;
    unsigned long long last_time;
    unsigned long long checksum;
    char link_indicator;
    char *link_name;
};

struct _Etui_Img_Cbt_Entry
{
    unsigned char *data;
    unsigned char *iter;
    size_t size;

    Etui_Img_Cbt_Header header;
    size_t position;
    size_t total_read;
};

struct _Etui_Img_Cbt
{
    Eina_File *file;
    unsigned char *data;
    unsigned char *iter;
    size_t size;
    unsigned long long header_size;
    unsigned long next_entry_position;
};

void etui_img_cbt_entry_free(Etui_Img_Cbt_Entry *entry);
Eina_List *etui_img_cbt_entries_get(Etui_Img_Cbt *tar);
Eina_Bool etui_img_cbt_entry_is_file(Etui_Img_Cbt_Entry *e);
void etui_img_cbt_entry_rewind(Etui_Img_Cbt_Entry *entry);
size_t etui_img_cbt_entry_read(Etui_Img_Cbt_Entry *entry, unsigned char *buffer, size_t chunk_size);

#endif
