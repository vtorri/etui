/* Etui - Multi-document rendering library using the EFL
 * Copyright (C) 2013-2014 Vincent Torri
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

#include <config.h>

#include <libgen.h>

#include <Eina.h>
#include <Evas.h>
#include <Ecore_File.h>

#include <EWebKit2.h>

#include <archive.h>
#include <archive_entry.h>

#include "Etui.h"
#include "etui_module.h"
#include "etui_module_epub.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#ifdef ETUI_MODULE_EPUB_DEFAULT_LOG_COLOR
# undef ETUI_MODULE_EPUB_DEFAULT_LOG_COLOR
#endif
#define ETUI_MODULE_EPUB_DEFAULT_LOG_COLOR EINA_COLOR_LIGHTCYAN

#ifdef ERR
# undef ERR
#endif
#define ERR(...)  EINA_LOG_DOM_ERR(_etui_module_epub_log_domain, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...)  EINA_LOG_DOM_DBG(_etui_module_epub_log_domain, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...)  EINA_LOG_DOM_INFO(_etui_module_epub_log_domain, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...)  EINA_LOG_DOM_WARN(_etui_module_epub_log_domain, __VA_ARGS__)

#ifdef CRIT
# undef CRIT
#endif
#define CRIT(...) EINA_LOG_DOM_CRIT(_etui_module_epub_log_domain, __VA_ARGS__)

typedef struct _Etui_Provider_Data Etui_Provider_Data;
typedef struct _Etui_Epub_Ncx_Toc_Item Etui_Epub_Ncx_Toc_Item;

struct _Etui_Epub_Ncx_Toc_Item
{
  char *label;
  char *link;
  Eina_Array *child;
};

struct _Etui_Provider_Data
{
    /* specific EFL stuff for the module */

    struct {
        Evas_Object *obj;
        void *m;
    } efl;

    /* specific EPUB stuff for the module */

    /* Document */
    struct
    {
        char *filename;
        Eina_Tmpstr *path;
        int page_nbr;
        Eina_Array toc;
        void *data;
        size_t size;
    } doc;

    struct
    {
        char *content_file;
        unsigned int has_container : 1;
        unsigned int has_rootfiles : 1;
    } container;

    struct
    {
        Eina_Hash *manifest;
        Eina_Array *spine;
        char *title;
        char *author;
        char *uuid;
        char *toc_id;
        unsigned int has_package : 1;
        unsigned int has_dc_title : 1;
        unsigned int has_dc_creator : 1;
        unsigned int has_dc_identifier : 1;
        unsigned int has_manifest : 1;
        unsigned int has_spine : 1;
    } opf;

    struct
    {
        struct
        {
            char *content;
            char *name;
        } meta_items[2];
        int meta_item_current;
        char *title;
        char *uid;
        char *author;
        Etui_Epub_Ncx_Toc_Item **stack;
        int depth;
        int current_depth;
        unsigned int meta_item_to_free : 1;
        unsigned int has_ncx : 1;
        unsigned int has_title : 1;
        unsigned int has_author : 1;
        unsigned int has_title_text : 1;
        unsigned int has_author_text : 1;
        unsigned int has_navmap : 1;
        unsigned int has_navpoint : 1;
        unsigned int has_navpoint_text : 1;
    } ncx;

    /* Current page */
    struct
    {
        int width;
        int height;
        int page_num;
        Etui_Rotation rotation;
        double scale;
    } page;
};

typedef struct _Etui_Epub_Opf_Manifest_Item Etui_Epub_Opf_Manifest_Item;

struct _Etui_Epub_Opf_Manifest_Item
{
    char *id;
    char *href;
    char *media_type;
};

static int _etui_module_epub_init_count = 0;
static int _etui_module_epub_log_domain = -1;

static Eina_Bool
_etui_epub_file_unzip(Etui_Provider_Data *pd)
{
    struct archive *a;
    struct archive_entry *entry;
    int r;

    if (!eina_file_mkdtemp("etui-epub-tmp-XXXXXX", &pd->doc.path))
        return EINA_FALSE;

    a = archive_read_new();
    if (!a)
        goto free_path;

    if (archive_read_support_filter_all(a) != ARCHIVE_OK)
        goto free_path;

    if (archive_read_support_format_zip(a) != ARCHIVE_OK)
        goto free_path;

    r = archive_read_open_filename(a, pd->doc.filename, 16384);
    if (r != ARCHIVE_OK)
        goto free_path;

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {
        if (archive_entry_filetype(entry) == AE_IFREG)
        {
            char buf[PATH_MAX];
            const char *name;
            char *dir;
            char *base;
            size_t size;
            void *data;

            name = archive_entry_pathname(entry);
            dir = strdup(name);
            base = strdup(name);
            if (dir && base && (strcmp(dir, ".") != 0))
            {
                snprintf(buf, sizeof(buf), "%s/%s", pd->doc.path, dirname(dir));
                buf[sizeof(buf) - 1] = '\0';
                ecore_file_mkdir(buf);
                printf(" * %s %s %s\n", name, dirname(dir), basename(base));
            }

            if (base)
                free(base);
            if (dir)
                free(dir);

            size = archive_entry_size(entry);
            data = malloc(size);
            if (data)
            {
                size_t res;

                res = archive_read_data(a, data, size);
                if (res > 0)
                {
                    FILE *f;

                    snprintf(buf, sizeof(buf), "%s/%s", pd->doc.path, name);
                    buf[sizeof(buf) - 1] = '\0';
                    printf(" $ %s\n", buf);
                    f = fopen(buf, "wb");
                    if (f)
                    {
                        fwrite(data, 1, size, f);
                        fclose(f);
                    }
                }

                free(data);
            }
        }
        archive_read_data_skip(a);
    }

    archive_read_free(a);

    return EINA_TRUE;

  free_path:
    eina_tmpstr_del(pd->doc.path);

    return EINA_FALSE;
}

static Eina_Bool
_etui_epub_mimetype_is_valid(Etui_Provider_Data *pd)
{
    char buf[PATH_MAX];
    Eina_File *f;
    void *data;

    snprintf(buf, sizeof(buf), "%s/%s", pd->doc.path, "mimetype");
    buf[sizeof(buf) - 1] = '\0';
    f = eina_file_open(buf, EINA_FALSE);
    if (!f)
        return EINA_FALSE;

    data = eina_file_map_all(f, EINA_FILE_POPULATE);
    if (!data)
    {
        eina_file_close(f);
        return EINA_FALSE;
    }

    if (strcmp(data, "application/epub+zip") != 0)
    {
        eina_file_close(f);
        return EINA_FALSE;
    }

    eina_file_close(f);

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_container_attr_parse_cb(void *data, const char *key, const char *value)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)data;

    if (!strcmp("media-type", key))
    {
        /* Media type must be "application/oebps-package+xml" */

        if (strcmp(value, "application/oebps-package+xml") != 0)
        {
            return EINA_FALSE;
        }
    }

    if (!strcmp("full-path", key))
    {
        pd->container.content_file = strdup(value);
    }

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_container_parse_cb(void *data, Eina_Simple_XML_Type type, const char *content, unsigned offset EINA_UNUSED, unsigned length)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)data;

    if ((type == EINA_SIMPLE_XML_OPEN) || (type == EINA_SIMPLE_XML_OPEN_EMPTY))
    {
        if (strncmp("container", content, strlen("container")) == 0)
        {
            if (pd->container.has_container)
            {
                return EINA_FALSE;
            }
            pd->container.has_container = 1;
        }
        else if (strncmp("rootfiles", content, strlen("rootfiles")) == 0)
        {
            if (!pd->container.has_container || pd->container.has_rootfiles)
            {
                return EINA_FALSE;
            }
            pd->container.has_rootfiles = 1;
        }
        else if (strncmp("rootfile", content, strlen("rootfile")) == 0)
        {
            const char *tags;

            if (!pd->container.has_container || !pd->container.has_rootfiles)
            {
                return EINA_FALSE;
            }

            tags = eina_simple_xml_tag_attributes_find(content,
                                                       length);

            if (!eina_simple_xml_attributes_parse(tags, length - (tags - content),
                                                  _etui_epub_container_attr_parse_cb, pd))
            {
                return EINA_FALSE;
            }
        }
    }

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_container_is_valid(Etui_Provider_Data *pd)
{
    char buf[PATH_MAX];
    Eina_File *f;

    snprintf(buf, sizeof(buf), "%s/%s", pd->doc.path, "META-INF/container.xml");
    buf[sizeof(buf) - 1] = '\0';
    f = eina_file_open(buf, EINA_FALSE);
    if (!f)
        return EINA_FALSE;

    if (!eina_simple_xml_parse((const char *)eina_file_map_all(f, EINA_FILE_POPULATE),
                               eina_file_size_get(f),
                               EINA_TRUE,
                               _etui_epub_container_parse_cb, pd))
    {
        eina_file_close(f);
        return EINA_FALSE;
    }

    eina_file_close(f);

    if (!pd->container.content_file || !*pd->container.content_file)
    {
        return EINA_FALSE;
    }

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_opf_manifest_item_attr_parse_cb(void *data, const char *key, const char *value)
{
    Etui_Epub_Opf_Manifest_Item *item;

    item = (Etui_Epub_Opf_Manifest_Item *)data;

    if (strcmp("id", key) == 0)
        item->id = strdup(value);

    if (!strcmp("href", key))
        item->href = strdup(value);

    if (!strcmp("media-type", key))
        item->media_type = strdup(value);

   return EINA_TRUE;
}

static Eina_Bool
_etui_epub_opf_spine_attr_parse_cb(void *data, const char *key, const char *value)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)data;

    if (!strcmp("toc", key))
        pd->opf.toc_id = strdup(value);

   return EINA_TRUE;
}

static Eina_Bool
_etui_epub_opf_spine_item_attr_parse_cb(void *data, const char *key, const char *value)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)data;

    if (!strcmp("idref", key))
        eina_array_push(pd->opf.spine, strdup(value));

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_opf_parse_cb(void *data, Eina_Simple_XML_Type type, const char *content, unsigned offset EINA_UNUSED, unsigned length)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)data;

    if ((type == EINA_SIMPLE_XML_OPEN) || (type == EINA_SIMPLE_XML_OPEN_EMPTY))
    {
        if (strncmp("package", content, strlen("package")) == 0)
        {
            if (pd->opf.has_package)
            {
                return EINA_FALSE;
            }
            pd->opf.has_package = 1;
        }
        else if (strncmp("manifest", content, strlen("manifest")) == 0)
        {
            if (!pd->opf.has_package || pd->opf.has_manifest)
            {
                return EINA_FALSE;
            }
            pd->opf.has_manifest = 1;
        }
        else if ((strncmp("item", content, strlen("item")) == 0) &&
                 (strncmp("itemref", content, strlen("itemref")) != 0))
        {
            Etui_Epub_Opf_Manifest_Item *item;

            if (!pd->opf.has_package || !pd->opf.has_manifest)
            {
                return EINA_FALSE;
            }

            item = (Etui_Epub_Opf_Manifest_Item *)calloc(1, sizeof(Etui_Epub_Opf_Manifest_Item));
            if (item)
            {
                const char *tags;

                tags = eina_simple_xml_tag_attributes_find(content,
                                                           length);

                if (eina_simple_xml_attributes_parse(tags, length - (tags - content),
                                                      _etui_epub_opf_manifest_item_attr_parse_cb, item))
                {
                    eina_hash_add(pd->opf.manifest, item->id, item);
                }
                else
                {
                    if (item->id)
                        free(item->id);
                    if (item->href)
                        free(item->href);
                    if (item->media_type)
                        free(item->media_type);
                    free(item);
                }
            }
        }
        else if (strncmp("spine", content, strlen("spine")) == 0)
        {
            const char *tags;

            if (!pd->opf.has_package || pd->opf.has_spine)
            {
                return EINA_FALSE;
            }
            pd->opf.has_spine = 1;

            tags = eina_simple_xml_tag_attributes_find(content,
                                                       length);

            eina_simple_xml_attributes_parse(tags, length - (tags - content),
                                             _etui_epub_opf_spine_attr_parse_cb, pd);
        }
        else if (strncmp("itemref", content, strlen("itemref")) == 0)
        {
            const char *tags;

            if (!pd->opf.has_package || !pd->opf.has_spine)
            {
                return EINA_FALSE;
            }

            tags = eina_simple_xml_tag_attributes_find(content,
                                                       length);

            eina_simple_xml_attributes_parse(tags, length - (tags - content),
                                             _etui_epub_opf_spine_item_attr_parse_cb, pd);
        }
        else if (strncmp("dc:title", content, strlen("dc:title")) == 0)
        {
            pd->opf.has_dc_title = 1;
        }
        else if (strncmp("dc:creator", content, strlen("dc:creator")) == 0)
        {
            pd->opf.has_dc_creator = 1;
        }
        else if (strncmp("dc:identifier", content, strlen("dc:identifier")) == 0)
        {
            pd->opf.has_dc_identifier = 1;
        }
    }
    else if (type == EINA_SIMPLE_XML_DATA)
    {
        if (pd->opf.has_dc_title)
        {
            pd->opf.title = (char *)malloc(length + 1);
            if (pd->opf.title)
            {
                memcpy(pd->opf.title, content, length);
                pd->opf.title[length] = '\0';
            }
        }
        if (pd->opf.has_dc_creator)
        {
            pd->opf.author = (char *)malloc(length + 1);
            if (pd->opf.author)
            {
                memcpy(pd->opf.author, content, length);
                pd->opf.author[length] = '\0';
            }
        }
        if (pd->opf.has_dc_identifier)
        {
            pd->opf.uuid = (char *)malloc(length + 1);
            if (pd->opf.uuid)
            {
                memcpy(pd->opf.uuid, content, length);
                pd->opf.uuid[length] = '\0';
            }
        }
    }
    else if (type == EINA_SIMPLE_XML_CLOSE)
    {
        if (pd->opf.has_dc_title)
            pd->opf.has_dc_title = 0;

        if (pd->opf.has_dc_creator)
            pd->opf.has_dc_creator = 0;

        if (pd->opf.has_dc_identifier)
            pd->opf.has_dc_identifier = 0;
    }

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_opf_is_valid(Etui_Provider_Data *pd)
{
    char buf[PATH_MAX];
    Eina_File *f;

    snprintf(buf, sizeof(buf), "%s/%s", pd->doc.path, pd->container.content_file);
    buf[sizeof(buf) - 1] = '\0';
    f = eina_file_open(buf, EINA_FALSE);
    if (!f)
        return EINA_FALSE;

    if (!eina_simple_xml_parse((const char *)eina_file_map_all(f, EINA_FILE_POPULATE),
                               eina_file_size_get(f),
                               EINA_TRUE,
                               _etui_epub_opf_parse_cb, pd))
    {
        eina_file_close(f);
        return EINA_FALSE;
    }

    eina_file_close(f);

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_ncx_meta_attr_parse_cb(void *data, const char *key, const char *value)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)data;

    if (!strcmp("name", key) &&
        (!strcmp("dtb:depth", value) || !strcmp("dtb:uid", value)))
    {
        if (pd->ncx.meta_item_current < 2)
            pd->ncx.meta_items[pd->ncx.meta_item_current].name = strdup(value);
    }

    if (!strcmp("content", key))
    {
        if (pd->ncx.meta_item_current < 2)
            pd->ncx.meta_items[pd->ncx.meta_item_current].content = strdup(value);
    }

    if (pd->ncx.meta_items[pd->ncx.meta_item_current].content &&
        pd->ncx.meta_items[pd->ncx.meta_item_current].name)
        pd->ncx.meta_item_current++;

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_ncx_content_attr_parse_cb(void *data, const char *key, const char *value)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)data;

    if (!strcmp("src", key))
    {
        pd->ncx.stack[pd->ncx.current_depth]->link = strdup(value);
    }

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_ncx_parse_cb(void *data, Eina_Simple_XML_Type type, const char *content, unsigned offset EINA_UNUSED, unsigned length)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)data;

    if ((type == EINA_SIMPLE_XML_OPEN) || (type == EINA_SIMPLE_XML_OPEN_EMPTY))
    {
        if (strncmp("ncx", content, strlen("ncx")) == 0)
        {
            if (pd->ncx.has_ncx)
            {
                return EINA_FALSE;
            }
            pd->ncx.has_ncx = 1;
        }
        else if (strncmp("meta", content, strlen("meta")) == 0)
        {
            const char *tags;

            tags = eina_simple_xml_tag_attributes_find(content,
                                                       length);

            eina_simple_xml_attributes_parse(tags, length - (tags - content),
                                             _etui_epub_ncx_meta_attr_parse_cb, pd);
        }
        else if (strncmp("docTitle", content, strlen("docTitle")) == 0)
        {
            if (!pd->ncx.has_ncx || pd->ncx.has_title)
            {
                return EINA_FALSE;
            }
            pd->ncx.has_title = 1;
        }
        else if (strncmp("docAuthor", content, strlen("docAuthor")) == 0)
        {
            if (!pd->ncx.has_ncx || pd->ncx.has_author)
            {
                return EINA_FALSE;
            }
            pd->ncx.has_author = 1;
        }
        else if (strncmp("text", content, strlen("text")) == 0)
        {
            if (pd->ncx.has_title)
                pd->ncx.has_title_text = 1;
            if (pd->ncx.has_author)
                pd->ncx.has_author_text = 1;
            if (pd->ncx.has_navpoint)
                pd->ncx.has_navpoint_text = 1;
        }
        else if (strncmp("navMap", content, strlen("navMap")) == 0)
        {
            int i;

            for (i = 0; i < 2; i++)
            {
                if (!strcmp(pd->ncx.meta_items[i].name, "dtb:uid"))
                    pd->ncx.uid = pd->ncx.meta_items[i].content;
                if (!strcmp(pd->ncx.meta_items[i].name, "dtb:depth"))
                {
                    pd->ncx.depth = atoi(pd->ncx.meta_items[i].content);
                    free(pd->ncx.meta_items[i].content);
                    pd->ncx.meta_item_to_free = 1 - i;
                }
                free(pd->ncx.meta_items[i].name);
            }
            /* FIXME: use an eina_array instead */
            if (pd->ncx.depth == 0)
                pd->ncx.depth = 10;
            pd->ncx.stack = (Etui_Epub_Ncx_Toc_Item **)calloc(pd->ncx.depth, sizeof(Etui_Epub_Ncx_Toc_Item *));
        }
        else if (strncmp("navPoint", content, strlen("navPoint")) == 0)
        {
            pd->ncx.current_depth++;
            pd->ncx.stack[pd->ncx.current_depth] = (Etui_Epub_Ncx_Toc_Item *)calloc(1, sizeof(Etui_Epub_Ncx_Toc_Item));
            if (pd->ncx.current_depth > 0)
            {
                if (pd->ncx.stack[pd->ncx.current_depth - 1]->child == NULL)
                    pd->ncx.stack[pd->ncx.current_depth - 1]->child = eina_array_new(4);
            }
            pd->ncx.has_navpoint = 1;
        }
        else if (strncmp("content", content, strlen("content")) == 0)
        {
            const char *tags;

            tags = eina_simple_xml_tag_attributes_find(content,
                                                       length);

            eina_simple_xml_attributes_parse(tags, length - (tags - content),
                                             _etui_epub_ncx_content_attr_parse_cb, pd);
        }
    }
    else if (type == EINA_SIMPLE_XML_DATA)
    {
        if (pd->ncx.has_title_text)
        {
            pd->ncx.title = (char *)malloc(length + 1);
            if (pd->ncx.title)
            {
                memcpy(pd->ncx.title, content, length);
                pd->ncx.title[length] = '\0';
            }
        }
        else if (pd->ncx.has_author_text)
        {
            pd->ncx.author = (char *)malloc(length + 1);
            if (pd->ncx.author)
            {
                memcpy(pd->ncx.author, content, length);
                pd->ncx.author[length] = '\0';
            }
        }
        else if (pd->ncx.has_navpoint_text)
        {
            if (pd->ncx.stack[pd->ncx.current_depth])
            {
                char *label;

                label = (char *)malloc(length + 1);
                if (label)
                {
                    memcpy(label, content, length);
                    label[length] = '\0';
                    pd->ncx.stack[pd->ncx.current_depth]->label = label;
                }
            }
        }
    }
    else if (type == EINA_SIMPLE_XML_CLOSE)
    {
        if (strncmp("docTitle", content, strlen("docTitle")) == 0)
        {
            pd->ncx.has_title = 0;
        }
        else if (strncmp("docAuthor", content, strlen("docAuthor")) == 0)
        {
            pd->ncx.has_author = 0;
        }
        else if (strncmp("navPoint", content, strlen("navPoint")) == 0)
        {
            pd->ncx.has_navpoint = 0;
            if (pd->ncx.stack[pd->ncx.current_depth])
            {
                if (pd->ncx.current_depth == 0)
                    eina_array_push(&pd->doc.toc, pd->ncx.stack[pd->ncx.current_depth]);
                else
                    eina_array_push(pd->ncx.stack[pd->ncx.current_depth - 1]->child, pd->ncx.stack[pd->ncx.current_depth]);
            }
            pd->ncx.current_depth--;
        }
        else if (strncmp("navMap", content, strlen("navMap")) == 0)
        {
            free(pd->ncx.stack);
            pd->ncx.stack = NULL;
        }
        else if (strncmp("text", content, strlen("text")) == 0)
        {
            if (pd->ncx.has_title)
                pd->ncx.has_title_text = 0;
            if (pd->ncx.has_author)
                pd->ncx.has_author_text = 0;
            if (pd->ncx.has_navpoint)
                pd->ncx.has_navpoint_text = 0;
        }
    }

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_ncx_is_valid(Etui_Provider_Data *pd)
{
    char buf[PATH_MAX];
    Etui_Epub_Opf_Manifest_Item *item;
    Eina_File *f;
    char *oebps_dir;

    if (!pd->opf.toc_id)
        return EINA_FALSE;

    item = eina_hash_find(pd->opf.manifest, pd->opf.toc_id);
    if (!item)
        return EINA_FALSE;

    oebps_dir = strdup(pd->container.content_file);
    if (!oebps_dir)
        return EINA_FALSE;

    snprintf(buf, sizeof(buf), "%s/%s/%s", pd->doc.path, dirname(oebps_dir), item->href);
    buf[sizeof(buf) - 1] = '\0';
    free(oebps_dir);
    f = eina_file_open(buf, EINA_FALSE);
    if (!f)
    {
        return EINA_FALSE;
    }

    pd->ncx.current_depth = -1;

    if (!eina_simple_xml_parse((const char *)eina_file_map_all(f, EINA_FILE_POPULATE),
                               eina_file_size_get(f),
                               EINA_TRUE,
                               _etui_epub_ncx_parse_cb, pd))
    {
        eina_file_close(f);
        return EINA_FALSE;
    }

    eina_file_close(f);

    return EINA_TRUE;
}

static Eina_Bool
_etui_epub_is_valid(Etui_Provider_Data *pd)
{
    /* the mimetype file must exist et has "application/epub+zip" as content */

    if (!_etui_epub_mimetype_is_valid(pd))
        return EINA_FALSE;

    /* then, the file "META-INF/container.xml" mst exist and point to */
    /* the .opf file (usually content.opf) with its relative path if any */

    if (!_etui_epub_container_is_valid(pd))
        return EINA_FALSE;

    /* Then we parse the .opf file to obtain the files of the Epub archive */

    if (!_etui_epub_opf_is_valid(pd))
        return EINA_FALSE;

    /* Then, we parse the .ncx file to obtain last checks and the ToC */

    if (!_etui_epub_ncx_is_valid(pd))
        return EINA_FALSE;

    /* Finally the latest checks */

    /* title of opf and ncx must be the same */
    printf(" *** title : %s %s\n", pd->opf.title, pd->ncx.title);
    if (pd->opf.title && pd->ncx.title)
    {
        if (strcmp(pd->opf.title, pd->ncx.title) != 0)
            return EINA_FALSE;
    }

    /* author of opf and ncx must be the same */
    printf(" *** authors : %s %s\n", pd->opf.author, pd->ncx.author);
    if (pd->opf.author && pd->ncx.author)
    {
        if (strcmp(pd->opf.author, pd->ncx.author) != 0)
            return EINA_FALSE;
    }

    /* uuid of opf and ncx must be the same */
    printf(" *** UUID : %s %s\n", pd->opf.uuid, pd->ncx.uid);
    if (pd->opf.uuid && pd->ncx.uid)
    {
        if (strcmp(pd->opf.uuid, pd->ncx.uid) != 0)
            return EINA_FALSE;
    }

    return EINA_TRUE;
}

static void
_etui_epub_opf_manifest_free_cb(void *data)
{
    Etui_Epub_Opf_Manifest_Item *item;

    item = (Etui_Epub_Opf_Manifest_Item *)data;

    if (item->id)
        free(item->id);
    if (item->href)
        free(item->href);
    if (item->media_type)
        free(item->media_type);
    free(item);
}

static void
_etui_epub_toc_unfill(Eina_Array *items, Eina_Bool free_array)
{
    Etui_Epub_Ncx_Toc_Item *item;
    Eina_Array_Iterator iterator;
    unsigned int i;

    if (!items)
        return;

    EINA_ARRAY_ITER_NEXT(items, i, item, iterator)
    {
        free(item->label);
        free(item->link);

        if (item->child)
            _etui_epub_toc_unfill(item->child, EINA_TRUE);

        free(item);
    }

    if (free_array)
        eina_array_free(items);
}

/* Virtual functions */

static void *
_etui_epub_init(Evas *evas)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)calloc(1, sizeof(Etui_Provider_Data));
    if (!pd)
        return NULL;

    DBG("init module");

    pd->efl.obj = evas_object_image_add(evas);
    if (!pd->efl.obj)
        goto free_pd;

    return pd;

  free_pd:
    free(pd);

    return NULL;
}

static void
_etui_epub_shutdown(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    DBG("shutdown module");

    pd = (Etui_Provider_Data *)d;

    if (pd->doc.filename)
      free(pd->doc.filename);
    evas_object_del(pd->efl.obj);
    free(pd);
}

static Evas_Object *
_etui_epub_evas_object_get(void *d)
{
    if (!d)
        return NULL;

    return ((Etui_Provider_Data *)d)->efl.obj;
}

static Eina_Bool
_manifest_foreach_cb(const Eina_Hash *manifest EINA_UNUSED,
                     const void *key EINA_UNUSED,
                     void *data, void *fdata EINA_UNUSED)
{
   const Etui_Epub_Opf_Manifest_Item *item = data;

   printf(" %s %s %s\n", item->id, item->href, item ->media_type);

   return EINA_TRUE;
}

static Eina_Bool
_print(const void *container EINA_UNUSED, void *data, void *fdata EINA_UNUSED)
{
  printf("%s\n", (char *)data);
  return EINA_TRUE;
}

static Eina_Bool
_print_toc(const void *container EINA_UNUSED, void *data, void *fdata EINA_UNUSED)
{
  char buf[1024];
  int i;
  Etui_Epub_Ncx_Toc_Item *item = (Etui_Epub_Ncx_Toc_Item *)data;
  static int d = 0;

  d++;
  for (i = 0; i < 2*d; i++)
    buf[i] = ' ';
  buf[i] = '\0';
  printf("%s%s (%s)\n", buf, item->label, item->link);
  if (item->child)
    {
      eina_array_foreach(item->child, _print_toc, NULL);
    }
  d--;
  return EINA_TRUE;
}

static Eina_Bool
_etui_epub_file_open(void *d, const char *filename)
{
    Etui_Provider_Data *pd;

    if (!d || !filename || !*filename)
        return EINA_FALSE;

    DBG("open file %s", filename);

    pd = (Etui_Provider_Data *)d;

    if (pd->doc.filename && (strcmp(filename, pd->doc.filename) == 0))
      return EINA_TRUE;

    if (pd->doc.filename)
        free(pd->doc.filename);
    pd->doc.filename = strdup(filename);
    if (!pd->doc.filename)
        return EINA_FALSE;

    pd->opf.manifest = eina_hash_string_superfast_new(_etui_epub_opf_manifest_free_cb);
    if (!pd->opf.manifest)
        goto free_filename;

    pd->opf.spine = eina_array_new(4);
    if (!pd->opf.spine)
        goto free_manifest;

    if (!_etui_epub_file_unzip(pd))
        goto free_spine;

    while (eina_array_count(&pd->doc.toc))
        free(eina_array_pop(&pd->doc.toc));
    eina_array_step_set(&pd->doc.toc, sizeof(Eina_Array), 4);

    if (!_etui_epub_is_valid(pd))
        goto remove_files;

    {
        printf("\n\n");
        printf(" * full path : %s\n", pd->container.content_file);
        printf(" * manifest :\n");
        eina_hash_foreach(pd->opf.manifest, _manifest_foreach_cb, NULL);
        printf(" * spine :\n");
        eina_array_foreach(pd->opf.spine, _print, NULL);
        printf(" * toc :\n");
        eina_array_foreach(&pd->doc.toc, _print_toc, NULL);
    }

    return EINA_TRUE;

  remove_files:
    printf("merde\n");
    ecore_file_recursive_rm(pd->doc.path);
    eina_tmpstr_del(pd->doc.path);
  free_spine:
    eina_array_free(pd->opf.spine);
  free_manifest:
    eina_hash_free(pd->opf.manifest);
  free_filename:
    free(pd->doc.filename);
    pd->doc.filename = NULL;

  return EINA_FALSE;
}

static void
_etui_epub_file_close(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    pd = (Etui_Provider_Data *)d;

    DBG("close file %s", pd->doc.filename);

    _etui_epub_toc_unfill(&pd->doc.toc, EINA_FALSE);
    eina_array_flush(&pd->doc.toc);

    free(pd->ncx.title);
    free(pd->ncx.author);
    free(pd->ncx.meta_items[pd->ncx.meta_item_to_free].content);

    free(pd->opf.toc_id);
    free(pd->opf.title);
    free(pd->opf.author);

    while (eina_array_count(pd->opf.spine))
        free(eina_array_pop(pd->opf.spine));
    eina_array_free(pd->opf.spine);

    eina_hash_free(pd->opf.manifest);

    free(pd->container.content_file);

    ecore_file_recursive_rm(pd->doc.path);
    eina_tmpstr_del(pd->doc.path);

    if (pd->doc.filename)
        free(pd->doc.filename);
    pd->doc.filename = NULL;
}

static void
_etui_epub_version_get(void *d EINA_UNUSED, int *maj, int *min)
{
    if (maj) *maj = 0;
    if (min) *min = 0;
}

static char *
_etui_epub_title_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    return strdup(pd->opf.title);
}

static char *
_etui_epub_author_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    return strdup(pd->opf.author);
}

static int
_etui_epub_pages_count(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return -1;

    pd = (Etui_Provider_Data *)d;

    return eina_array_count(pd->opf.spine);
}

static const Eina_Array *
_etui_epub_toc_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    return &pd->doc.toc;
}

static Eina_Bool
_etui_epub_page_set(void *d, int page_num)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if ((page_num < 0) || (page_num >= (int)eina_array_count(pd->opf.spine)))
        return EINA_FALSE;

    if (page_num == pd->page.page_num)
        return EINA_FALSE;

    pd->page.page_num = page_num;
    pd->page.rotation = ETUI_ROTATION_0;
    pd->page.scale = 1.0f;

    return EINA_TRUE;
}

static int
_etui_epub_page_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return -1;

    pd = (Etui_Provider_Data *)d;

    return pd->page.page_num;
}

static Eina_Bool
_etui_epub_page_rotation_set(void *d, Etui_Rotation rotation)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (pd->page.rotation == rotation)
        return EINA_TRUE;

    pd->page.rotation = rotation;

    return EINA_TRUE;
}

static Etui_Rotation
_etui_epub_page_rotation_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return ETUI_ROTATION_0;

    pd = (Etui_Provider_Data *)d;
    return pd->page.rotation;
}

static Eina_Bool
_etui_epub_page_scale_set(void *d, double scale)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (pd->page.scale != scale)
      pd->page.scale = scale;

    return EINA_TRUE;
}

static double
_etui_epub_page_scale_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
    {
        return -1.0;
    }

    pd = (Etui_Provider_Data *)d;

    return pd->page.scale;
}

static Etui_Provider_Descriptor _etui_provider_descriptor_epub =
{
    /* .name                          */ "epub",
    /* .version                       */ ETUI_PROVIDER_DESCRIPTOR_VERSION,
    /* .priority                      */ ETUI_PROVIDER_DESCRIPTOR_PRIORITY_NORMAL,
    /* .init                          */ _etui_epub_init,
    /* .shutdown                      */ _etui_epub_shutdown,
    /* .evas_object_get               */ _etui_epub_evas_object_get,
    /* .file_open                     */ _etui_epub_file_open,
    /* .file_close                    */ _etui_epub_file_close,
    /* .version_get                   */ _etui_epub_version_get,
    /* .title_get                     */ _etui_epub_title_get,
    /* .author_get                    */ _etui_epub_author_get,
    /* .subject_get                   */ NULL,
    /* .keywords_get                  */ NULL,
    /* .creator_get                   */ NULL,
    /* .producer_get                  */ NULL,
    /* .creation_date_get             */ NULL,
    /* .modification_date_get         */ NULL,
    /* .is_printable                  */ NULL,
    /* .is_changeable                 */ NULL,
    /* .is_copyable                   */ NULL,
    /* .is_notable                    */ NULL,
    /* .password_needed               */ NULL,
    /* .password_set                  */ NULL,
    /* .pages_count                   */ _etui_epub_pages_count,
    /* .toc_get                       */ _etui_epub_toc_get,
    /* .page_use_display_list_set     */ NULL,
    /* .page_use_display_list_get     */ NULL,
    /* .page_set                      */ _etui_epub_page_set,
    /* .page_get                      */ _etui_epub_page_get,
    /* .page_size_get                 */ NULL,//_etui_epub_page_size_get,
    /* .page_rotation_set             */ _etui_epub_page_rotation_set,
    /* .page_rotation_get             */ _etui_epub_page_rotation_get,
    /* .page_scale_set                */ _etui_epub_page_scale_set,
    /* .page_scale_get                */ _etui_epub_page_scale_get,
    /* .page_dpi_set                  */ NULL,
    /* .page_dpi_get                  */ NULL,
    /* .page_links_get                */ NULL,
    /* .page_render_pre               */ NULL,//_etui_epub_page_render_pre,
    /* .page_render                   */ NULL,//_etui_epub_page_render,
    /* .page_render_end               */ NULL,//_etui_epub_page_render_end,
    /* .page_text_extract             */ NULL,
    /* .page_text_find                */ NULL,
    /* .page_duration_get             */ NULL,
    /* .page_transition_type_get      */ NULL,
    /* .page_transition_duration_get  */ NULL,
    /* .page_transition_vertical_get  */ NULL,
    /* .page_transition_outwards_get  */ NULL,
    /* .page_transition_direction_get */ NULL
};

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

Eina_Bool
etui_module_epub_init(void)
{
    if (_etui_module_epub_init_count > 0)
    {
        _etui_module_epub_init_count++;
        return EINA_TRUE;
    }

    _etui_module_epub_log_domain = eina_log_domain_register("etui-epub",
                                                            ETUI_MODULE_EPUB_DEFAULT_LOG_COLOR);
    if (_etui_module_epub_log_domain < 0)
    {
        EINA_LOG_CRIT("Could not register log domain 'etui-epub'");
        return EINA_FALSE;
    }

    if (!etui_module_register(&_etui_provider_descriptor_epub))
    {
        ERR("Could not register module %p", &_etui_provider_descriptor_epub);
        goto unregister_log;
    }

    _etui_module_epub_init_count = 1;
    return EINA_TRUE;

  unregister_log:
    eina_log_domain_unregister(_etui_module_epub_log_domain);
    _etui_module_epub_log_domain = -1;

    return EINA_FALSE;
}

void
etui_module_epub_shutdown(void)
{
    if (_etui_module_epub_init_count > 1)
    {
        _etui_module_epub_init_count--;
        return;
    }
    else if (_etui_module_epub_init_count == 0)
    {
        ERR("Too many etui_module_epub_shutdown() calls");
        return;
    }

    DBG("shutdown epub module");

    _etui_module_epub_init_count = 0;

    etui_module_unregister(&_etui_provider_descriptor_epub);

    eina_log_domain_unregister(_etui_module_epub_log_domain);
    _etui_module_epub_log_domain = -1;
}

#ifndef ETUI_BUILD_STATIC_EPUB

EINA_MODULE_INIT(etui_module_epub_init);
EINA_MODULE_SHUTDOWN(etui_module_epub_shutdown);

#endif

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
