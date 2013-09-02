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

/*
 * Information source :
 * http://en.wikipedia.org/wiki/Tar_%28file_format%29
 * http://www.mkssoftware.com/docs/man4/tar.4.asp
 * http://www.freebsd.org/cgi/man.cgi?query=tar&sektion=5&manpath=FreeBSD+8-current
 * http://cdrecord.berlios.de/private/man/star/star.4.html
 */

/*
 * Adapted from http://codexpert.ro/forum/viewtopic.php?f=47&t=2232
 * Creative Common license (CC BY-SA 3.0)
 */

/*
 * offset size description
 *
 * 0      100  File name
 * 100    8    File mode
 * 108    8    Owner's numeric user ID
 * 116    8    Group's numeric user ID
 * 124    12   File size in bytes (octal basis)
 * 136    12   Last modification time in numeric Unix time format (octal)
 * 148    8    Checksum for header record
 * 156    1    Link indicator: 0 normal, 1 hardlink, 2 symbolic link
 * 157    100  Name of linked file
 *
 * UStar format:
 * ignored if UStar_indicator is not the nul terminated string "ustar"
 * but it's for tar format before 1988, so
 * let's assume it's UStar format
 *
 * 257    6    UStar indicator "ustar"
 * 263    2    UStar version "00"
 * 265    32   Owner user name
 * 297    32   Owner group name
 * 329    8    Device major number
 * 337    8    Device minor number
 * 345    155  Filename prefix
 */

#include <stdio.h>

#include <Eina.h>

#include "tar.h"

#define CHUNK_SIZE 512

typedef struct _Etui_Img_Cbt_Header_Ascii Etui_Img_Cbt_Header_Ascii;

struct _Etui_Img_Cbt_Header_Ascii
{
    char file_name[100];
    char file_mode[8];
    char owner_id[8];
    char group_id[8];
    char file_size[12];
    char last_time[12];
    char checksum[8];
    char link_indicator;
    char link_name[100];
    char ustar_indicator[6];
    char ustar_version[2];
    char ustar_owner_user_name[32];
    char ustar_owner_group_name[32];
    char ustar_device_major_number[8];
    char ustar_device_minor_number[8];
    char ustar_file_name_prefix[155];
    char pad[12];
};

static size_t
_etui_img_cbt_read(Etui_Img_Cbt *tar, void *buffer, size_t size)
{
    size_t is_read;

    if ((size_t)(tar->iter - tar->data) <= (tar->size - size))
        is_read = size;
    else
        is_read = tar->size - (tar->iter - tar->data);

    memcpy(buffer, tar->iter, is_read);
    tar->iter += is_read;

    return is_read;
}

static size_t
_etui_img_cbt_entry_read(Etui_Img_Cbt_Entry *entry, void *buffer, size_t size)
{
    size_t is_read;

    if ((size_t)(entry->iter - entry->data) <= (entry->size - size))
        is_read = size;
    else
        is_read = entry->size - (entry->iter - entry->data);

    memcpy(buffer, entry->iter, is_read);
    entry->iter += is_read;

    return is_read;
}

static unsigned long long
_etui_img_cbt_string_to_octal(const char *s, unsigned int l)
{
    char *tmp;
    unsigned long long res;
    unsigned int i;

    tmp = (char *)s;
    i = 0;
    while ((*tmp == ' ') || (*tmp == '\0'))
    {
        if (i >= 12)
            break;
        i++;
        tmp++;
    }

    res = 0;
    for (; i < l; i++, tmp++)
    {
        if ((*tmp == ' ') || (*tmp == '\0'))
            continue;
        res = (res << 3) + (unsigned long long)(*tmp - '0');
    }

    return res;
}

static unsigned long long
_etui_img_cbt_header_ascii_checksum_get(const Etui_Img_Cbt_Header_Ascii *a)
{
    unsigned char *iter;
    unsigned long long checksum;
    int i;

    iter = (unsigned char *)a;
    checksum = 0;
    for (i = 0; i < 148; i++, iter++)
        checksum += *iter;
    for (i = 148; i < 156; i++, iter++)
        checksum += ' ';
    for (i = 156; i < 500; i++, iter++)
        checksum += *iter;

    return checksum;
}

static Eina_Bool
_etui_img_cbt_header_ascii_to_internal(const Etui_Img_Cbt_Header_Ascii *a, Etui_Img_Cbt_Header *t)
{
    size_t l;
    unsigned long long computed_checksum;
    unsigned long long original_checksum;

    computed_checksum = _etui_img_cbt_header_ascii_checksum_get(a);
    original_checksum = _etui_img_cbt_string_to_octal(a->checksum, 8);

    if ((computed_checksum != original_checksum) &&
        ((original_checksum != 0) || (strlen(a->file_name) == 0)))
    {
        printf("wrong (or no) checksum\n");
        return EINA_FALSE;
    }

    l = strlen(a->file_name) + 1;
    t->file_name = (char *)malloc(l * sizeof(char));
    if (!t->file_name)
        return EINA_FALSE;

    memcpy(t->file_name, a->file_name, l);

    memcpy(t->file_mode, a->file_mode, 8);
    t->owner_id = _etui_img_cbt_string_to_octal(a->owner_id, 8);
    t->group_id = _etui_img_cbt_string_to_octal(a->group_id, 8);
    t->file_size = _etui_img_cbt_string_to_octal(a->file_size, 12);
    t->last_time = _etui_img_cbt_string_to_octal(a->last_time, 12);
    t->checksum = original_checksum;
    t->link_indicator = a->link_indicator;

    l = strlen(a->link_name) + 1;
    t->link_name = (char *)malloc(l * sizeof(char));
    if (!t->link_name)
        goto free_file_name;

    memcpy(t->link_name, a->link_name, l);

    if (memcmp(a->ustar_indicator, "ustar", sizeof("ustar")) != 0)
    {
        if (memcmp(a->ustar_indicator, "ustar  ", sizeof("ustar  ")) != 0)
        {
            if (memcmp(a->ustar_indicator, "GNUtar ", sizeof("GNUtar ")) != 0)
                goto free_link_name;
        }
    }
    else
    {
        if ((a->ustar_version[0] != '0') || (a->ustar_version[1] != '0'))
            goto free_link_name;
    }

    return EINA_TRUE;

  free_link_name:
    free(t->link_name);
  free_file_name:
    free(t->file_name);
    return EINA_FALSE;
}

static int
_etui_img_cbt_sort_cb(const void *d1, const void *d2)
{
  return strcasecmp(((Etui_Img_Cbt_Entry *)d1)->header.file_name,
                    ((Etui_Img_Cbt_Entry *)d2)->header.file_name);
}

static Etui_Img_Cbt_Entry *
_etui_img_cbt_entry_new(Etui_Img_Cbt *tar, Etui_Img_Cbt_Header *header)
{
    Etui_Img_Cbt_Entry *entry;

    entry = (Etui_Img_Cbt_Entry *)calloc(1, sizeof(Etui_Img_Cbt_Entry));
    if (!entry)
        return NULL;

    entry->data = tar->data;
    entry->iter = tar->iter;
    entry->size = tar->size;

    entry->position = tar->iter - tar->data;
    entry->header = *header;

    return entry;
}

static void
_etui_img_cbt_entry_free(Etui_Img_Cbt_Entry *entry)
{
    if (!entry)
        return;

    free(entry->header.link_name);
    free(entry->header.file_name);
    free(entry);
}

static Etui_Img_Cbt_Entry *
_etui_img_cbt_entry_next_get(Etui_Img_Cbt *tar)
{
    char block[CHUNK_SIZE];
    char zero[CHUNK_SIZE];
    Etui_Img_Cbt_Header_Ascii header_ascii;
    Etui_Img_Cbt_Header header;
    size_t is_read;

    memset(block, 0, sizeof(block));
    memset(zero, 0, sizeof(zero));

    tar->iter = tar->data + tar->next_entry_position;
    is_read = _etui_img_cbt_read(tar, block, CHUNK_SIZE);
    tar->header_size += is_read;
    tar->next_entry_position += is_read;

    if (is_read < CHUNK_SIZE)
    {
        printf("should be MD5");
        return NULL;
    }

    if (memcmp(block, zero, CHUNK_SIZE) == 0)
    {
        if((tar->size - tar->header_size) < CHUNK_SIZE)
        {
            // TODO: read checksum
        }

        return NULL;
    }

    memcpy(&header_ascii, block, sizeof(header_ascii));

    if (!_etui_img_cbt_header_ascii_to_internal(&header_ascii, &header))
    {
        printf("headers conversion failed\n");
        return NULL;
    }

    if (header.file_size > 0)
    {
        if (header.file_size % CHUNK_SIZE == 0)
            tar->next_entry_position += header.file_size;
         else
            tar->next_entry_position += CHUNK_SIZE * (1 + header.file_size / CHUNK_SIZE);
      }

    return _etui_img_cbt_entry_new(tar, &header);
}

static Etui_Img_Cbt_Entry *
_etui_img_cbt_entry_first_get(Etui_Img_Cbt *tar)
{
    tar->header_size = 0;
    tar->next_entry_position = 0;

    return _etui_img_cbt_entry_next_get(tar);
}

/***** API *****/

Eina_List *
etui_img_cbt_entries_get(Etui_Img_Cbt *tar)
{
    Etui_Img_Cbt_Entry *e;
    Eina_List *list = NULL;

    if (!tar)
        return NULL;

    e = _etui_img_cbt_entry_first_get(tar);
    do
    {
        list = eina_list_append(list, e);
        e = _etui_img_cbt_entry_next_get(tar);
    } while (e);

    list = eina_list_sort(list, eina_list_count(list), _etui_img_cbt_sort_cb);

    return list;
}

void
etui_img_cbt_entries_free(Eina_List *list)
{
    Etui_Img_Cbt_Entry *e;

    EINA_LIST_FREE(list, e)
        _etui_img_cbt_entry_free(e);
}

Eina_Bool
etui_img_cbt_entry_is_file(Etui_Img_Cbt_Entry *e)
{
    if (!e)
        return EINA_FALSE;

    return ((e->header.link_indicator == 0) || (e->header.link_indicator == '0'));
}

void
etui_img_cbt_entry_rewind(Etui_Img_Cbt_Entry *entry)
{
    if (!entry)
        return;

    if ((size_t)(entry->iter - entry->data) != entry->position)
        entry->iter = entry->data + entry->position;
}

size_t
etui_img_cbt_entry_read(Etui_Img_Cbt_Entry *entry, unsigned char *buffer, size_t chunk_size)
{
    size_t left;
    size_t is_read;

    if (!buffer || (chunk_size == 0))
        return 0;

    left = entry->header.file_size - entry->total_read;
    if (left == 0)
        return 0;

    if (left < chunk_size)
        chunk_size = left;

    is_read = _etui_img_cbt_entry_read(entry, buffer, chunk_size);
    entry->total_read += is_read;

    return is_read;
}
