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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Eina.h>
#include <Ecore.h> /* for Ecore_Thread in Etui_Module */
#include <Evas.h>

#include "mupdf/fitz.h"
#include "mupdf/pdf.h"

#include "Etui.h"
#include "etui_module.h"
#include "etui_file.h"
#include "etui_module_pdf.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

#ifdef ETUI_MODULE_PDF_DEFAULT_LOG_COLOR
# undef ETUI_MODULE_PDF_DEFAULT_LOG_COLOR
#endif
#define ETUI_MODULE_PDF_DEFAULT_LOG_COLOR EINA_COLOR_LIGHTCYAN

#ifdef ERR
# undef ERR
#endif
#define ERR(...)  EINA_LOG_DOM_ERR(_etui_module_pdf_log_domain, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...)  EINA_LOG_DOM_DBG(_etui_module_pdf_log_domain, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...)  EINA_LOG_DOM_INFO(_etui_module_pdf_log_domain, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...)  EINA_LOG_DOM_WARN(_etui_module_pdf_log_domain, __VA_ARGS__)

#ifdef CRIT
# undef CRIT
#endif
#define CRIT(...) EINA_LOG_DOM_CRIT(_etui_module_pdf_log_domain, __VA_ARGS__)

typedef struct _Etui_Module_Data Etui_Module_Data;

struct _Etui_Module_Data
{
    /* specific EFL stuff for the module */

    struct {
        Evas_Object *obj;
        void *m;
    } efl;

    /* specific PDF stuff for the module */

    /* Document */
    struct
    {
        Etui_Module_Pdf_Info *info; /* information specific to the document (creator, ...) */
        Etui_Module_Pdf_Api *api; /* api (search, etc...) */
        int page_nbr;
        Eina_Array toc;
        char *title;
        fz_context *ctx;
        fz_document *doc;
    } doc;

    /* Current page */
    struct
    {
        fz_page *page;
        fz_display_list *list;
        fz_pixmap *image;
        int width;
        int height;
        Eina_Array links;
        int page_num;
        Etui_Rotation rotation;
        double scale;
        float duration;
        fz_transition *transition;
        unsigned int use_display_list :1;
    } page;
};

static int _etui_module_pdf_init_count = 0;
static int _etui_module_pdf_log_domain = -1;

static Eina_Inarray *
_etui_pdf_search(void *mod, int page_num, const char *needle)
{
#if FZ_VERSION_MINOR >= 16
    fz_quad r[5000];
    fz_rect hit;
#else
    fz_rect r[512];
#endif
    Etui_Module_Data *md;
    int nbr;

    if ((page_num < 0) || !needle || !*needle)
        return NULL;

    md = (Etui_Module_Data *)mod;
    nbr = fz_search_page_number(md->doc.ctx, md->doc.doc,
                                page_num, needle, r, nelem(r));
    if (nbr > 0)
    {
        Eina_Inarray *boxes;
        int i;

        boxes = eina_inarray_new(sizeof(Eina_Rectangle), 0);
        if (!boxes)
            return NULL;

        for (i = 0; i < nbr; i++)
        {
            Eina_Rectangle box;

#if FZ_VERSION_MINOR >= 16
            hit = fz_rect_from_quad(r[i]);
            box.x = floor(hit.x0);
            box.y = floor(hit.y0);
            box.w = ceil(hit.x1 - hit.x0);
            box.h = ceil(hit.y1 - hit.y0);
#else
            box.x = floor(r[i].x0);
            box.y = floor(r[i].y0);
            box.w = ceil(r[i].x1 - r[i].x0);
            box.h = ceil(r[i].y1 - r[i].y0);
#endif
            eina_inarray_push(boxes, &box);
        }

        return boxes;
    }

    return NULL;
}

#if 0

static char
_etui_pdf_to_lowercase(char c)
{
    return ((c >= 'A') && (c <= 'Z')) ? c + 32  : c;
}

/* code borrowed from doc_search.c */
static fz_text_char
_etui_pdf_page_text_text_charat(fz_text_page *page, int idx)
{
    static fz_text_char emptychar = { {0,0,0,0}, ' ' };
    fz_text_block *block;
    fz_text_line *line;
    fz_text_span *span;
    int ofs = 0;

    for (block = page->blocks; block < page->blocks + page->len; block++)
    {
        for (line = block->lines; line < block->lines + block->len; line++)
        {
            for (span = line->spans; span < line->spans + line->len; span++)
            {
                if (idx < ofs + span->len)
                    return span->text[idx - ofs];
                /* pseudo-newline */
                if (span + 1 == line->spans + line->len)
                {
                    if (idx == ofs + span->len)
                        return emptychar;
                    ofs++;
                }
                ofs += span->len;
            }
        }
    }

    return emptychar;
}

/* code borrowed from doc_search.c */
static int
_etui_pdf_page_text_charat(fz_text_page *page, int idx)
{
    return _etui_pdf_page_text_text_charat(page, idx).c;
}

/* code borrowed from doc_search.c */
static int
_etui_pdf_page_text_match(fz_text_page *page, const char *s, int n)
{
    int orig = n;
    int c;

    while (*s)
    {
        s += fz_chartorune(&c, (char *)s);
        if (c == ' ' && _etui_pdf_page_text_charat(page, n) == ' ')
        {
            while (_etui_pdf_page_text_charat(page, n) == ' ')
                n++;
        }
        else
        {
            if (_etui_pdf_to_lowercase(c) != _etui_pdf_to_lowercase(_etui_pdf_page_text_charat(page, n)))
                return 0;
            n++;
        }
    }

    return n - orig;
}

#endif

static Eina_Array *
_etui_pdf_toc_fill(const Etui_Module_Data *md, Eina_Array *items, fz_outline *outline)
{
    Eina_Array *res;
    Etui_Toc_Item *item;

    res = (items) ? items : eina_array_new(4);

    while (outline)
    {
        item = (Etui_Toc_Item *)calloc(1, sizeof(Etui_Toc_Item));
        if (!item)
            continue;

        if (outline->title)
            item->title = strdup(outline->title);

        if (fz_is_external_link(md->doc.ctx, outline->uri))
        {
            Etui_Link_Uri l;

            item->kind = ETUI_LINK_KIND_URI;
            l.uri = strdup(outline->uri);
            l.is_open = !!outline->is_open;
            item->dest.uri = l;
        }
        else
        {
            Etui_Link_Goto l;

            item->kind = ETUI_LINK_KIND_GOTO;
            l.page = fz_resolve_link(md->doc.ctx, md->doc.doc,
                                     outline->uri, &l.page_x, &l.page_y);
            item->dest.goto_ = l;
        }

        if (outline->down)
            item->child = _etui_pdf_toc_fill(md, item->child, outline->down);

        eina_array_push(res, item);

        outline = outline->next;
    }

    return res;
}

static void
_etui_pdf_toc_unfill(Eina_Array *items, Eina_Bool free_array)
{
    Etui_Toc_Item *item;
    Eina_Array_Iterator iterator;
    unsigned int i;

    if (!items)
        return;

    EINA_ARRAY_ITER_NEXT(items, i, item, iterator)
    {
        switch (item->kind)
        {
            case ETUI_LINK_KIND_GOTO:
                break;
            case ETUI_LINK_KIND_URI:
                free(item->dest.uri.uri);
                break;
            default:
                break;
        }

        if (item->title)
            free(item->title);

        if (item->child)
            _etui_pdf_toc_unfill(item->child, EINA_TRUE);

        free(item);
    }

    if (free_array)
        eina_array_free(items);
}

static char *_etui_pdf_metadata_get(const Etui_Module_Data *md, const char *key)
{
    char *metadata = NULL;
    int sz;
    char buf[2];

    /* md is valid */

    sz = fz_lookup_metadata(md->doc.ctx, md->doc.doc, key,
                            buf, 2);
    if (sz > 0)
    {
        metadata = (char *)malloc(sz * sizeof(char));
        if (metadata)
        {
            fz_lookup_metadata(md->doc.ctx, md->doc.doc, key,
                               metadata, sz);
        }
    }

    return metadata;
}

static char *
_etui_pdf_info_date_get(const Etui_Module_Data *md, const char *key)
{
    char *date;
    char *fmt;

    date = _etui_pdf_metadata_get(md, key);
    if (!date)
        return NULL;

    /* FIXME: manage minutes and seconds ? */

    fmt = (char *)malloc(21);
    if (!fmt)
        return NULL;

    memcpy(fmt, date + 2, 4);
    fmt[4] = '-';
    memcpy(fmt + 5, date + 6, 2);
    fmt[7] = '-';
    memcpy(fmt + 8, date + 8, 2);
    fmt[10] = ',';
    fmt[11] = ' ';
    memcpy(fmt + 12, date + 10, 2);
    fmt[14] = ':';
    memcpy(fmt + 15, date + 12, 2);
    fmt[17] = ':';
    memcpy(fmt + 18, date + 14, 2);
    fmt[20] = '\0';
    free(date);

    return fmt;
}

static void
_etui_pdf_info_del(Etui_Module_Data *md)
{
#define METADATA_DEL(memb_) \
    free(md->doc.info->memb_); \
    md->doc.info->memb_ = NULL

    METADATA_DEL(author);
    METADATA_DEL(subject);
    METADATA_DEL(keywords);
    METADATA_DEL(creator);
    METADATA_DEL(producer);
    METADATA_DEL(creation_date);
    METADATA_DEL(modification_date);
    METADATA_DEL(encryption);

#undef METADATA_DEL
}

static void
_etui_pdf_info_set(Etui_Module_Data *md)
{
    _etui_pdf_info_del(md);
    md->doc.info->author = _etui_pdf_metadata_get(md, FZ_META_INFO_AUTHOR);
    md->doc.info->subject = _etui_pdf_metadata_get(md, "info:Subject");
    md->doc.info->keywords = _etui_pdf_metadata_get(md, "info:Keywords");
    md->doc.info->creator = _etui_pdf_metadata_get(md, "info:Creator");
    md->doc.info->producer = _etui_pdf_metadata_get(md, "info:Producer");
    md->doc.info->creation_date = _etui_pdf_info_date_get(md, "info:CreationDate");
    md->doc.info->modification_date = _etui_pdf_info_date_get(md, "info:ModDate");
    md->doc.info->encryption = _etui_pdf_metadata_get(md, "encryption");
}

/* Virtual functions */

static void *
_etui_pdf_init(const Etui_File *ef)
{
    Etui_Module_Data *md;

    md = (Etui_Module_Data *)calloc(1, sizeof(Etui_Module_Data));
    if (!md)
        return NULL;

    DBG("init module");

    fz_var(md->doc.doc);
    /* FIXME:
     * 1st parameter: custom memory allocator ?
     * 2nd parameter: locks/unlocks for multithreading
     */
    md->doc.ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
    if (!md->doc.ctx)
    {
        ERR("Could not create context");
        goto free_md;
    }

    fz_try(md->doc.ctx)
    {
        fz_stream *stream;
        fz_page *page;
        fz_outline *outline;

        /* FIXME: add alpha as option ? */
        fz_set_text_aa_level(md->doc.ctx, 8);
        fz_set_graphics_aa_level(md->doc.ctx, 8);
        /* FIXME: add min line width as option ? */
        fz_set_graphics_min_line_width(md->doc.ctx, 0);
        fz_disable_icc(md->doc.ctx);

		fz_register_document_handlers(md->doc.ctx);
        stream = fz_open_memory(md->doc.ctx,
                                (unsigned char *)etui_file_base_get(ef),
                                etui_file_size_get(ef));
        md->doc.doc = fz_open_document_with_stream(md->doc.ctx,
                                                   "pdf", stream);
        if (!md->doc.doc)
        {
            ERR("could not open document %s", etui_file_filename_get(ef));
            goto drop_ctx;
        }

        /* try to open the first page */
        /* if it's a damaged PDF, no error */
        /* if not, an error will be thrown */
        page = fz_load_page(md->doc.ctx, md->doc.doc, 0);
        if (!page)
        {
            ERR("could not open first page from the document");
            goto close_doc;
        }

        fz_drop_page(md->doc.ctx, page);

        md->doc.title = _etui_pdf_metadata_get(md, FZ_META_INFO_TITLE);

        while (eina_array_count(&md->doc.toc))
            free(eina_array_pop(&md->doc.toc));
        eina_array_step_set(&md->doc.toc, sizeof(Eina_Array), 4);

        outline = fz_load_outline(md->doc.ctx, md->doc.doc);
        if (outline)
        {
            md->doc.toc = *_etui_pdf_toc_fill(md, &md->doc.toc, outline);
            fz_drop_outline(md->doc.ctx, outline);
        }
    }
    fz_catch(md->doc.ctx)
    {
        ERR("Could not open file %s", etui_file_filename_get(ef));
        goto drop_ctx;
    }

    md->doc.info = (Etui_Module_Pdf_Info *)calloc(1, sizeof(Etui_Module_Pdf_Info));
    if (!md->doc.info)
    {
        ERR("Could not allocate memory for information structure");;
        goto free_title;
    }

    md->doc.api = (Etui_Module_Pdf_Api *)calloc(1, sizeof(Etui_Module_Pdf_Api));
    if (!md->doc.api)
    {
        ERR("Could not allocate memory for api structure");;
        goto free_info;
    }

    md->doc.api->mod = md;
    md->doc.api->search = _etui_pdf_search;

    md->doc.page_nbr = fz_count_pages(md->doc.ctx, md->doc.doc);
    md->page.page_num = -1;
    md->page.rotation = ETUI_ROTATION_0;
    md->page.scale = 1.0f;

    return md;

  free_info:
    free(md->doc.info);
  free_title:
    _etui_pdf_toc_unfill(&md->doc.toc, EINA_FALSE);
    eina_array_flush(&md->doc.toc);
    free(md->doc.title);
  close_doc:
    fz_drop_document(md->doc.ctx, md->doc.doc);
  drop_ctx:
    fz_drop_context(md->doc.ctx);
  free_md:
    free(md);

    return NULL;
}

static void
_etui_pdf_shutdown(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return;

    DBG("shutdown module");

    md = (Etui_Module_Data *)d;

    /* _etui_pdf_links_unfill(&md->page.links); */
    /* eina_array_flush(&md->page.links); */

    free(md->doc.api);
    _etui_pdf_info_del(md);
    free(md->doc.info);
    _etui_pdf_toc_unfill(&md->doc.toc, EINA_FALSE);
    eina_array_flush(&md->doc.toc);
    free(md->doc.title);
    fz_drop_document(md->doc.ctx, md->doc.doc);
    fz_drop_context(md->doc.ctx);
    free(md);
}

static Evas_Object *
_etui_pdf_evas_object_add(void *d, Evas *evas)
{
    if (!d)
        return NULL;

    ((Etui_Module_Data *)d)->efl.obj = evas_object_image_add(evas);
    return ((Etui_Module_Data *)d)->efl.obj;
}

static void
_etui_pdf_evas_object_del(void *d)
{
    if (!d)
        return;

    evas_object_del(((Etui_Module_Data *)d)->efl.obj);
}

static const void *
_etui_pdf_info_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;

    return md->doc.info;
}

static const char *
_etui_pdf_title_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;

    return md->doc.title;
}

static int
_etui_pdf_pages_count(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return -1;

    md = (Etui_Module_Data *)d;

    if (!md->doc.doc)
    {
        ERR("no opened document");
        return -1;
    }

    return fz_count_pages(md->doc.ctx, md->doc.doc);
}

static const Eina_Array *
_etui_pdf_toc_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;

    return &md->doc.toc;
}

static Eina_Bool
_etui_pdf_page_set(void *d, int page_num)
{
    Etui_Module_Data *md;
    fz_page *page;
    fz_link *links;

    if (!d)
        return EINA_FALSE;

    md = (Etui_Module_Data *)d;

    if (!md->doc.doc)
    {
        ERR("no opened document");
        return EINA_FALSE;
    }

    if ((page_num < 0) || (page_num >= fz_count_pages(md->doc.ctx, md->doc.doc)))
        return EINA_FALSE;

    if (page_num == md->page.page_num)
        return EINA_FALSE;

    page = fz_load_page(md->doc.ctx, md->doc.doc, page_num);
    if (!page)
    {
        ERR("could not set page %d from the document", page_num);
        return EINA_FALSE;
    }

    /* TODO SEGV HERE !!!! */
    if (md->page.page)
        fz_drop_page(md->doc.ctx, md->page.page);

    /* _etui_pdf_links_unfill(&md->page.links); */
    /* if (md->page.links.data) */
    /*     eina_array_flush(&md->page.links); */

    /* eina_array_step_set(&md->page.links, sizeof(Eina_Array), 4); */

    /* links = fz_load_links(md->doc.doc, page); */
    /* if (links) */
    /* { */
    /*     md->page.links = *_etui_pdf_page_links_fill(&md->page.links, links); */
    /*     fz_drop_link(md->doc.ctx, links); */
    /* } */

    md->page.page = page;
    md->page.page_num = page_num;
    md->page.rotation = ETUI_ROTATION_0;
    md->page.scale = 1.0f;
    md->page.duration = 0.0;
    /* md->page.transition = fz_page_presentation(md->doc.ctx, md->page.page, &md->page.duration); */

    /* info */
    _etui_pdf_info_set(md);

    return EINA_TRUE;
}

static int
_etui_pdf_page_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return -1;

    md = (Etui_Module_Data *)d;

    return md->page.page_num;
}

static void
_etui_pdf_page_size_get(void *d, int *width, int *height)
{
    Etui_Module_Data *md;
    fz_rect rect;

    if (!d)
        goto _err;

    md = (Etui_Module_Data *)d;

    if (!md->doc.doc)
    {
        ERR("no opened document");
        goto _err;
    }

    evas_object_image_size_get(md->efl.obj, width, height);

    return;

  _err:
    if (width) *width = 0;
    if (height) *height = 0;
}

static Eina_Bool
_etui_pdf_page_rotation_set(void *d, Etui_Rotation rotation)
{
    Etui_Module_Data *md;

    if (!d)
        return EINA_FALSE;

    md = (Etui_Module_Data *)d;

    if (md->page.rotation != rotation)
        md->page.rotation = rotation;

    return EINA_TRUE;
}

static Etui_Rotation
_etui_pdf_page_rotation_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return ETUI_ROTATION_0;

    md = (Etui_Module_Data *)d;
    return md->page.rotation;
}

static Eina_Bool
_etui_pdf_page_scale_set(void *d, double scale)
{
    Etui_Module_Data *md;

    if (!d)
        return EINA_FALSE;

    md = (Etui_Module_Data *)d;

    if (md->page.scale != scale)
      md->page.scale = scale;

    return EINA_TRUE;
}

static double
_etui_pdf_page_scale_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
    {
        return -1.0;
    }

    md = (Etui_Module_Data *)d;

    return md->page.scale;
}

static void
_etui_pdf_page_render_pre(void *d)
{
    Etui_Module_Data *md;
    fz_matrix ctm;
    fz_rect bounds;
    fz_irect ibounds;
    int width;
    int height;

    if (!d)
        return;

    DBG("render pre");

    md = (Etui_Module_Data *)d;

    if (!md->doc.doc)
    {
        ERR("no opened document");
        return;
    }

#if FZ_VERSION_MINOR >= 16
    bounds = fz_bound_page(md->doc.ctx, md->page.page);
    ctm = fz_rotate(md->page.rotation);
    ctm = fz_pre_scale(ctm, md->page.scale, md->page.scale);
    ibounds = fz_round_rect(fz_transform_rect(bounds, ctm));
#else
    fz_bound_page(md->doc.ctx, md->page.page, &bounds);
    fz_pre_scale(fz_rotate(&ctm, md->page.rotation), md->page.scale, md->page.scale);
    fz_round_rect(&ibounds, fz_transform_rect(&bounds, &ctm));
#endif

    width = ibounds.x1 - ibounds.x0;
    height = ibounds.y1 - ibounds.y0;
    printf(" pre 0 $$ %dx%d\n", width, height);

    evas_object_image_size_set(md->efl.obj, width, height);
    printf(" pre 1 $$ %dx%d\n", width, height);
    evas_object_image_filled_set(md->efl.obj, EINA_TRUE);
    printf(" pre 2 $$ %dx%d\n", width, height);
    md->efl.m = evas_object_image_data_get(md->efl.obj, 1);
    md->page.width = width;
    md->page.height = height;

//    evas_object_resize(md->efl.obj, width, height);
    printf(" pre 3 $$ %dx%d\n", width, height);
}

static void
_etui_pdf_page_render(void *d)
{
    Etui_Module_Data *md;
    fz_device *dev = NULL;
    fz_pixmap *image;
    fz_cookie cookie = { 0 };
    fz_matrix ctm;
    fz_rect bounds;
    fz_irect ibounds;
    int width;
    int height;

    if (!d)
        return;

    DBG("render");

    md = (Etui_Module_Data *)d;

    if (md->page.use_display_list)
    {
        if (md->page.list)
            fz_drop_display_list(md->doc.ctx, md->page.list);
        md->page.list = fz_new_display_list_from_page(md->doc.ctx, md->page.page);

        dev = fz_new_list_device(md->doc.ctx, md->page.list);
#if FZ_VERSION_MINOR >= 16
        fz_run_page(md->doc.ctx, md->page.page, dev, fz_identity, &cookie);
#else
        fz_run_page(md->doc.ctx, md->page.page, dev, &fz_identity, &cookie);
#endif
        fz_close_device(md->doc.ctx, dev);
    }
#if FZ_VERSION_MINOR >= 16
    bounds = fz_bound_page(md->doc.ctx, md->page.page);
    ctm = fz_rotate(md->page.rotation);
    ctm = fz_pre_scale(ctm, md->page.scale, md->page.scale);
    ibounds = fz_round_rect(fz_transform_rect(bounds, ctm));
#else
    fz_bound_page(md->doc.ctx, md->page.page, &bounds);
    fz_pre_scale(fz_rotate(&ctm, md->page.rotation),
                 md->page.scale, md->page.scale);
    fz_round_rect(&ibounds, fz_transform_rect(&bounds, &ctm));
#endif
    width = ibounds.x1 - ibounds.x0;
    height = ibounds.y1 - ibounds.y0;
#if FZ_VERSION_MINOR == 11
    image = fz_new_pixmap_with_bbox_and_data(md->doc.ctx,
                                             fz_device_bgr(md->doc.ctx),
                                             &ibounds, 1,
                                             (unsigned char *)md->efl.m);
#elif FZ_VERSION_MINOR == 12
    image = fz_new_pixmap_with_bbox_and_data(md->doc.ctx,
                                             fz_device_bgr(md->doc.ctx),
                                             &ibounds, NULL, 1,
                                             (unsigned char *)md->efl.m);
#else
    image = fz_new_pixmap_with_bbox_and_data(md->doc.ctx,
                                             fz_device_bgr(md->doc.ctx),
                                             ibounds, NULL, 1,
                                             (unsigned char *)md->efl.m);
#endif

    fz_clear_pixmap_with_value(md->doc.ctx, image, 0xff);
    dev = fz_new_draw_device(md->doc.ctx, fz_identity, image);
    if (md->page.use_display_list)
#if FZ_VERSION_MINOR >= 16
        fz_run_display_list(md->doc.ctx, md->page.list, dev, ctm, bounds, &cookie);
#else
        fz_run_display_list(md->doc.ctx, md->page.list, dev, &ctm, &bounds, &cookie);
#endif
    else
#if FZ_VERSION_MINOR >= 16
        fz_run_page(md->doc.ctx, md->page.page, dev, ctm, &cookie);
#else
        fz_run_page(md->doc.ctx, md->page.page, dev, &ctm, &cookie);
#endif
    fz_close_device(md->doc.ctx, dev);
    dev = NULL;
    md->page.image = image;
}

static void
_etui_pdf_page_render_end(void *d)
{
    Etui_Module_Data *md;
    int width;
    int height;

    if (!d)
        return;

    DBG("render end");

    md = (Etui_Module_Data *)d;

    evas_object_image_size_get(md->efl.obj, &width, &height);
    printf(" end $$ %dx%d\n", width, height);
    evas_object_size_hint_min_set(md->efl.obj, width, height);
    evas_object_image_data_set(md->efl.obj, md->efl.m);
    evas_object_image_data_update_add(md->efl.obj, 0, 0, width, height);
    fz_drop_pixmap(md->doc.ctx, md->page.image);
}

static const void *
_etui_pdf_api_get(void *d)
{
    Etui_Module_Data *md;

    if (!d)
        return NULL;

    md = (Etui_Module_Data *)d;

    return md->doc.api;
}

static Etui_Module_Func _etui_module_func_pdf =
{
    /* .init              */ _etui_pdf_init,
    /* .shutdown          */ _etui_pdf_shutdown,
    /* .evas_object_add   */ _etui_pdf_evas_object_add,
    /* .evas_object_del   */ _etui_pdf_evas_object_del,
    /* .info_get          */ _etui_pdf_info_get,
    /* .title_get         */ _etui_pdf_title_get,
    /* .pages_count       */ _etui_pdf_pages_count,
    /* .toc_get           */ _etui_pdf_toc_get,
    /* .page_set          */ _etui_pdf_page_set,
    /* .page_get          */ _etui_pdf_page_get,
    /* .page_size_get     */ _etui_pdf_page_size_get,
    /* .page_rotation_set */ _etui_pdf_page_rotation_set,
    /* .page_rotation_get */ _etui_pdf_page_rotation_get,
    /* .page_scale_set    */ _etui_pdf_page_scale_set,
    /* .page_scale_get    */ _etui_pdf_page_scale_get,
    /* .page_render_pre   */ _etui_pdf_page_render_pre,
    /* .page_render       */ _etui_pdf_page_render,
    /* .page_render_end   */ _etui_pdf_page_render_end,
    /* .api_get           */ _etui_pdf_api_get
};

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/**** module API access ****/

static Eina_Bool
module_open(Etui_Module *em)
{
    if (_etui_module_pdf_init_count > 0)
    {
        _etui_module_pdf_init_count++;
        return EINA_TRUE;
    }

    if (!em)
        return EINA_FALSE;

    _etui_module_pdf_log_domain = eina_log_domain_register("etui-pdf",
                                                           ETUI_MODULE_PDF_DEFAULT_LOG_COLOR);
    if (_etui_module_pdf_log_domain < 0)
    {
        EINA_LOG_ERR("Can not create a module log domain.");
        return EINA_FALSE;
    }

    /* inititialize external libraries here */

    em->functions = (void *)(&_etui_module_func_pdf);

    _etui_module_pdf_init_count = 1;
    return EINA_TRUE;
}

static void
module_close(Etui_Module *em)
{
    if (_etui_module_pdf_init_count > 1)
    {
        _etui_module_pdf_init_count--;
        return;
    }
    else if (_etui_module_pdf_init_count == 0)
    {
        ERR("Too many etui_module_pdf_shutdown() calls");
        return;
    }

    DBG("shutdown pdf module");

    /* shutdown module here */
    em->functions->shutdown(em->data);

    /* shutdown external libraries here */

    /* shutdown EFL here */

    eina_log_domain_unregister(_etui_module_pdf_log_domain);
    _etui_module_pdf_log_domain = -1;
    _etui_module_pdf_init_count = 0;
}

static Etui_Module_Api _etui_modapi =
{
    "pdf",
    {
        module_open,
        module_close
    }
};

ETUI_MODULE_DEFINE(pdf)

#ifndef ETUI_BUILD_STATIC_PDF
ETUI_EINA_MODULE_DEFINE(pdf);
#endif

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
