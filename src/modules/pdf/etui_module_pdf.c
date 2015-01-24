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

#include "mupdf/fitz.h"
#include "mupdf/pdf.h"

#include "Etui.h"
#include "etui_module.h"
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

typedef struct _Etui_Provider_Data Etui_Provider_Data;

struct _Etui_Provider_Data
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
        char *filename;
        fz_context *ctx;
        fz_document *doc;
        Eina_Array toc;
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
        float hscale;
        float vscale;
        float duration;
        fz_transition *transition;
        unsigned int use_display_list :1;
    } page;
};

static int _etui_module_pdf_init_count = 0;
static int _etui_module_pdf_log_domain = -1;

static char
_etui_pdf_to_lowercase(char c)
{
    return ((c >= 'A') && (c <= 'Z')) ? c + 32  : c;
}

/* code borrowed from doc_search.c */
static int
_etui_pdf_page_text_charat(fz_text_page *page, int idx)
{
    fz_char_and_box cab;
    fz_text_char_at(&cab, page, idx);
    return cab.c;
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

static char *
_etui_pdf_document_property_get(fz_document *doc, const char *prop)
{
    char buf[1024];

    memcpy(buf, &prop, sizeof(char *));
    if (fz_meta(doc, FZ_META_INFO, buf, sizeof(buf)) <= 0)
        return NULL;

    return strdup(buf);
}

static char *
_etui_pdf_document_date_get(fz_document *doc, const char *prop)
{
    char *date;
    char *fmt;

    date = _etui_pdf_document_property_get(doc, prop);
    if (!date)
            return NULL;

    /* FIXME: manage minutes and seconds ? */

    fmt = (char *)malloc(21);
    if (fmt)
    {
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

    return fmt;
}

static Etui_Link_Goto
_etui_pdf_link_goto_fill(fz_link_dest *dest)
{
    Etui_Link_Goto l;

    l.page = dest->ld.gotor.page;
    /* l.page = dest->ld.gotor.page; */
    /* l.page = dest->ld.gotor.page; */
    /* l.page = dest->ld.gotor.page; */
    l.new_window = !!dest->ld.gotor.new_window;

    return l;
}

static Etui_Link_Goto_Remote
_etui_pdf_link_goto_remote_fill(fz_link_dest *dest)
{
    Etui_Link_Goto_Remote l;

    l.page = dest->ld.gotor.page;
    /* l.page = dest->ld.gotor.page; */
    /* l.page = dest->ld.gotor.page; */
    /* l.page = dest->ld.gotor.page; */
    l.filename = strdup(dest->ld.gotor.file_spec);
    l.new_window = !!dest->ld.gotor.new_window;

    return l;
}

static Etui_Link_Uri
_etui_pdf_link_uri_fill(fz_link_dest *dest)
{
    Etui_Link_Uri l;

    l.uri = strdup(dest->ld.uri.uri);
    l.is_map = !!dest->ld.uri.is_map;

    return l;
}

static Etui_Link_Launch
_etui_pdf_link_launch_fill(fz_link_dest *dest)
{
    Etui_Link_Launch l;

    l.filename = strdup(dest->ld.launch.file_spec);
    l.new_window = !!dest->ld.launch.new_window;

    return l;
}

static Etui_Link_Named
_etui_pdf_link_named_fill(fz_link_dest *dest)
{
    Etui_Link_Named l;

    l.named = strdup(dest->ld.named.named);

    return l;
}

static Eina_Array *
_etui_pdf_page_links_fill(Eina_Array *items, fz_link *links)
{
    fz_link *iter;
    fz_irect rect;
    Etui_Link_Item *item;

    for (iter = links; iter; iter = iter->next)
    {
        item = (Etui_Link_Item *)calloc(1, sizeof(Etui_Link_Item));
        if (!item)
            continue;

        switch (iter->dest.kind)
        {
            case FZ_LINK_GOTO:
                item->kind = ETUI_LINK_KIND_GOTO;
                item->dest.goto_ = _etui_pdf_link_goto_fill(&iter->dest);
                break;
            case FZ_LINK_GOTOR:
                item->kind = ETUI_LINK_KIND_GOTO_REMOTE;
                item->dest.goto_remote = _etui_pdf_link_goto_remote_fill(&iter->dest);
                break;
            case FZ_LINK_URI:
                item->kind = ETUI_LINK_KIND_URI;
                item->dest.uri = _etui_pdf_link_uri_fill(&iter->dest);
                break;
            case FZ_LINK_LAUNCH:
                item->kind = ETUI_LINK_KIND_LAUNCH;
                item->dest.launch = _etui_pdf_link_launch_fill(&iter->dest);
                break;
            case FZ_LINK_NAMED:
                item->kind = ETUI_LINK_KIND_NAMED;
                item->dest.named = _etui_pdf_link_named_fill(&iter->dest);
                break;
            default:
                item->kind = ETUI_LINK_KIND_NONE;
                break;
        }

        /* FIXME: take into account the rotation and the zoom ? */
        fz_irect_from_rect(&rect, &links->rect);
        item->rect.x = rect.x0;
        item->rect.y = rect.y0;
        item->rect.w = rect.x1 - rect.x0;
        item->rect.h = rect.y1 - rect.y0;

        eina_array_push(items, item);
    }

    return items;
}

static void
_etui_pdf_links_unfill(Eina_Array *items)
{
    Etui_Link_Item *item;
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
            case ETUI_LINK_KIND_GOTO_REMOTE:
                free(item->dest.goto_remote.filename);
                break;
            case ETUI_LINK_KIND_URI:
                free(item->dest.uri.uri);
                break;
            case ETUI_LINK_KIND_LAUNCH:
                free(item->dest.launch.filename);
                break;
            case ETUI_LINK_KIND_NAMED:
                free(item->dest.named.named);
                break;
            default:
                break;
        }

        free(item);
    }
}

static Eina_Array *
_etui_pdf_toc_fill(Eina_Array *items, fz_outline *outline)
{
    fz_outline *iter;
    Eina_Array *res;
    Etui_Toc_Item *item;

    if (!items)
        res = eina_array_new(4);
    else
        res = items;

    for (iter = outline; iter; iter = iter->next)
    {
        item = (Etui_Toc_Item *)calloc(1, sizeof(Etui_Toc_Item));
        if (!item)
            continue;

        if (iter->title)
            item->title = strdup(iter->title);

        switch (iter->dest.kind)
        {
            case FZ_LINK_GOTO:
                item->kind = ETUI_LINK_KIND_GOTO;
                item->dest.goto_ = _etui_pdf_link_goto_fill(&iter->dest);
                break;
            case FZ_LINK_GOTOR:
                item->kind = ETUI_LINK_KIND_GOTO_REMOTE;
                item->dest.goto_remote = _etui_pdf_link_goto_remote_fill(&iter->dest);
                break;
            case FZ_LINK_URI:
                item->kind = ETUI_LINK_KIND_URI;
                item->dest.uri = _etui_pdf_link_uri_fill(&iter->dest);
                break;
            case FZ_LINK_LAUNCH:
                item->kind = ETUI_LINK_KIND_LAUNCH;
                item->dest.launch = _etui_pdf_link_launch_fill(&iter->dest);
                break;
            case FZ_LINK_NAMED:
                item->kind = ETUI_LINK_KIND_NAMED;
                item->dest.named = _etui_pdf_link_named_fill(&iter->dest);
                break;
            default:
                item->kind = ETUI_LINK_KIND_NONE;
                break;
        }

        if (iter->down)
            item->child = _etui_pdf_toc_fill(item->child, iter->down);

        eina_array_push(res, item);
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
            case ETUI_LINK_KIND_GOTO_REMOTE:
                free(item->dest.goto_remote.filename);
                break;
            case ETUI_LINK_KIND_URI:
                free(item->dest.uri.uri);
                break;
            case ETUI_LINK_KIND_LAUNCH:
                free(item->dest.launch.filename);
                break;
            case ETUI_LINK_KIND_NAMED:
                free(item->dest.named.named);
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

/* Virtual functions */

static void *
_etui_pdf_init(Evas *evas)
{
    Etui_Provider_Data *pd;

    pd = (Etui_Provider_Data *)calloc(1, sizeof(Etui_Provider_Data));
    if (!pd)
        return NULL;

    DBG("init module");

    pd->efl.obj = evas_object_image_add(evas);
    if (!pd->efl.obj)
        goto free_pd;

    /* FIXME:
     * 1st parameter: custom memory allocator ?
     * 2nd parameter: locks/unlocks for multithreading
     */
    pd->doc.ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
    if (!pd->doc.ctx)
    {
        ERR("Could not create context");
        goto del_obj;
    }

    return pd;

  del_obj:
    evas_object_del(pd->efl.obj);
  free_pd:
    free(pd);

    return NULL;
}

static void
_etui_pdf_shutdown(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    DBG("shutdown module");

    pd = (Etui_Provider_Data *)d;

    if (pd->doc.doc)
        fz_close_document(pd->doc.doc);
    if (pd->doc.filename)
      free(pd->doc.filename);
    evas_object_del(pd->efl.obj);
    fz_free_context(pd->doc.ctx);
    free(pd);
}

static Evas_Object *
_etui_pdf_evas_object_get(void *d)
{
    if (!d)
        return NULL;

    return ((Etui_Provider_Data *)d)->efl.obj;
}

static Eina_Bool
_etui_pdf_file_open(void *d, const char *filename)
{
    Etui_Provider_Data *pd;
    fz_outline *outline;

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

    if (pd->doc.doc)
        fz_close_document(pd->doc.doc);

    fz_try(pd->doc.ctx)
    {
        fz_page *page;

        pd->doc.doc = fz_open_document(pd->doc.ctx, filename);
        if (!pd->doc.doc)
        {
            ERR("could not open document %s", filename);
            goto free_filename;
        }

        /* try to open the first page */
        /* if it's a damaged PDF, no error */
        /* if not, an error will be thrown */
        page = fz_load_page(pd->doc.doc, 0);
        if (!page)
        {
            ERR("could not open first page from the document");
            goto close_doc;
        }

        fz_free_page(pd->doc.doc, page);

        while (eina_array_count(&pd->doc.toc))
            free(eina_array_pop(&pd->doc.toc));
        eina_array_step_set(&pd->doc.toc, sizeof(Eina_Array), 4);

        outline = fz_load_outline(pd->doc.doc);
        if (outline)
        {
            pd->doc.toc = *_etui_pdf_toc_fill(&pd->doc.toc, outline);
            fz_free_outline(pd->doc.ctx, outline);
        }
    }
    fz_catch(pd->doc.ctx)
    {
        ERR("Could not open file %s", filename);
        goto free_filename;
    }

    pd->page.page_num = -1;

    return EINA_TRUE;

  close_doc:
    fz_close_document(pd->doc.doc);
    pd->doc.doc = NULL;
  free_filename:
    free(pd->doc.filename);
    pd->doc.filename = NULL;

  return EINA_FALSE;
}

static void
_etui_pdf_file_close(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    pd = (Etui_Provider_Data *)d;

    DBG("close file %s", pd->doc.filename);

    _etui_pdf_links_unfill(&pd->page.links);
    eina_array_flush(&pd->page.links);

    _etui_pdf_toc_unfill(&pd->doc.toc, EINA_FALSE);
    eina_array_flush(&pd->doc.toc);

    if (pd->page.page)
        fz_free_page(pd->doc.doc, pd->page.page);
    pd->page.page = NULL;

    if (pd->page.use_display_list)
    {
        if (pd->page.list)
            fz_free(pd->doc.ctx, pd->page.list);
    }
    pd->page.list = NULL;

    if (pd->doc.doc)
        fz_close_document(pd->doc.doc);
    pd->doc.doc = NULL;

    if (pd->doc.filename)
        free(pd->doc.filename);
    pd->doc.filename = NULL;
}

static void
_etui_pdf_version_get(void *d, int *maj, int *min)
{
    char buf[256];
    Etui_Provider_Data *pd;
    char *tmp;

    if (!d)
        goto _err;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        goto _err;
    }

    if (fz_meta(pd->doc.doc, FZ_META_FORMAT_INFO, buf, sizeof(buf)) < 0)
        goto _err;

    if (strlen(buf) < 5)
        goto _err;

    tmp = strchr(buf + 4, '.');
    *tmp = '\0';
    if (maj) *maj = atoi(buf + 4);
    if (min) *min = atoi(tmp + 1);

    return;

  _err:
    if (maj) *maj = -1;
    if (min) *min = -1;
}

static char *
_etui_pdf_title_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return _etui_pdf_document_property_get(pd->doc.doc, "Title");
}

static char *
_etui_pdf_author_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return _etui_pdf_document_property_get(pd->doc.doc, "Author");
}

static char *
_etui_pdf_subject_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return _etui_pdf_document_property_get(pd->doc.doc, "Subject");
}

static char *
_etui_pdf_keywords_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return _etui_pdf_document_property_get(pd->doc.doc, "Keywords");
}

static char *
_etui_pdf_creator_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return _etui_pdf_document_property_get(pd->doc.doc, "Creator");
}

static char *
_etui_pdf_producer_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return _etui_pdf_document_property_get(pd->doc.doc, "Producer");
}

static char *
_etui_pdf_creation_date_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return _etui_pdf_document_date_get(pd->doc.doc, "CreationDate");
}

static char *
_etui_pdf_modification_date_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    return _etui_pdf_document_date_get(pd->doc.doc, "ModDate");
}

static Eina_Bool
_etui_pdf_is_printable(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return EINA_FALSE;
    }

    return fz_meta(pd->doc.doc, FZ_META_HAS_PERMISSION, NULL, FZ_PERMISSION_PRINT) == 0;
}

static Eina_Bool
_etui_pdf_is_changeable(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return EINA_FALSE;
    }

    return fz_meta(pd->doc.doc, FZ_META_HAS_PERMISSION, NULL, FZ_PERMISSION_CHANGE) == 0;
}

static Eina_Bool
_etui_pdf_is_copyable(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return EINA_FALSE;
    }

    return fz_meta(pd->doc.doc, FZ_META_HAS_PERMISSION, NULL, FZ_PERMISSION_COPY) == 0;
}

static Eina_Bool
_etui_pdf_is_notable(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return EINA_FALSE;
    }

    return fz_meta(pd->doc.doc, FZ_META_HAS_PERMISSION, NULL, FZ_PERMISSION_NOTES) == 0;
}

static Eina_Bool
_etui_pdf_password_needed(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return EINA_FALSE;
    }

    return fz_needs_password(pd->doc.doc) ? EINA_TRUE : EINA_FALSE;
}

static Eina_Bool
_etui_pdf_password_set(void *d, const char *password)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return EINA_FALSE;
    }

    return fz_authenticate_password(pd->doc.doc, (char *)password) ? EINA_TRUE : EINA_FALSE;
}

static int
_etui_pdf_pages_count(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return -1;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return -1;
    }

    return fz_count_pages(pd->doc.doc);
}

static const Eina_Array *
_etui_pdf_toc_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    return &pd->doc.toc;
}

static void
_etui_pdf_page_use_display_list_set(void *d, Eina_Bool on)
{
    Etui_Provider_Data *pd;

    if (!d)
        return;

    pd = (Etui_Provider_Data *)d;

    pd->page.use_display_list = !!on;
}

static Eina_Bool
_etui_pdf_page_use_display_list_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    return pd->page.use_display_list;
}

static Eina_Bool
_etui_pdf_page_set(void *d, int page_num)
{
    Etui_Provider_Data *pd;
    fz_page *page;
    fz_link *links;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return EINA_FALSE;
    }

    if ((page_num < 0) || (page_num >= fz_count_pages(pd->doc.doc)))
        return EINA_FALSE;

    if (page_num == pd->page.page_num)
        return EINA_FALSE;

    page = fz_load_page(pd->doc.doc, page_num);
    if (!page)
    {
        ERR("could not set page %d from the document", page_num);
        return EINA_FALSE;
    }

    if (pd->page.page)
        fz_free_page(pd->doc.doc, pd->page.page);

    _etui_pdf_links_unfill(&pd->page.links);
    if (pd->page.links.data)
        eina_array_flush(&pd->page.links);

    eina_array_step_set(&pd->page.links, sizeof(Eina_Array), 4);

    links = fz_load_links(pd->doc.doc, page);
    if (links)
    {
        pd->page.links = *_etui_pdf_page_links_fill(&pd->page.links, links);
        fz_drop_link(pd->doc.ctx, links);
    }

    pd->page.page = page;
    pd->page.page_num = page_num;
    pd->page.rotation = ETUI_ROTATION_0;
    pd->page.hscale = 1.0f;
    pd->page.vscale = 1.0f;
    pd->page.duration = 0.0;
    pd->page.transition = fz_page_presentation(pd->doc.doc, pd->page.page, &pd->page.duration);

    return EINA_TRUE;
}

static int
_etui_pdf_page_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return -1;

    pd = (Etui_Provider_Data *)d;

    return pd->page.page_num;
}

static void
_etui_pdf_page_size_get(void *d, int *width, int *height)
{
    Etui_Provider_Data *pd;
    fz_rect rect;

    if (!d)
        goto _err;

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        goto _err;
    }

    fz_bound_page(pd->doc.doc, pd->page.page, &rect);
    if (width) *width = (int)(rect.x1 - rect.x0);
    if (height) *height = (int)(rect.y1 - rect.y0);

    return;

  _err:
    if (width) *width = 0;
    if (height) *height = 0;
}

static Eina_Bool
_etui_pdf_page_rotation_set(void *d, Etui_Rotation rotation)
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
_etui_pdf_page_rotation_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return ETUI_ROTATION_0;

    pd = (Etui_Provider_Data *)d;
    return pd->page.rotation;
}

static Eina_Bool
_etui_pdf_page_scale_set(void *d, float hscale, float vscale)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if ((pd->page.hscale == hscale) && (pd->page.vscale == vscale))
        return EINA_TRUE;

    pd->page.hscale = hscale;
    pd->page.vscale = vscale;

    return EINA_TRUE;
}

static void
_etui_pdf_page_scale_get(void *d, float *hscale, float *vscale)
{
    Etui_Provider_Data *pd;

    if (!d)
    {
        if (hscale) *hscale = 1.0f;
        if (vscale) *vscale = 1.0f;
        return;
    }

    pd = (Etui_Provider_Data *)d;

    if (hscale) *hscale = pd->page.hscale;
    if (vscale) *vscale = pd->page.vscale;
}

static const Eina_Array *
_etui_pdf_page_links_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    return &pd->page.links;
}

static void
_etui_pdf_page_render_pre(void *d)
{
    Etui_Provider_Data *pd;
    fz_matrix ctm;
    fz_rect bounds;
    fz_irect ibounds;
    int width;
    int height;

    if (!d)
        return;

    DBG("render pre");

    pd = (Etui_Provider_Data *)d;

    if (!pd->doc.doc)
    {
        ERR("no opened document");
        return;
    }

    fz_bound_page(pd->doc.doc, pd->page.page, &bounds);
    fz_pre_scale(fz_rotate(&ctm, pd->page.rotation), pd->page.hscale, pd->page.vscale);
    fz_round_rect(&ibounds, fz_transform_rect(&bounds, &ctm));

    width = ibounds.x1 - ibounds.x0;
    height = ibounds.y1 - ibounds.y0;
    printf(" pre 0 $$ %dx%d\n", width, height);

    evas_object_image_size_set(pd->efl.obj, width, height);
    printf(" pre 1 $$ %dx%d\n", width, height);
    evas_object_image_fill_set(pd->efl.obj, 0, 0, width, height);
    printf(" pre 2 $$ %dx%d\n", width, height);
    pd->efl.m = evas_object_image_data_get(pd->efl.obj, 1);
    pd->page.width = width;
    pd->page.height = height;

    evas_object_resize(pd->efl.obj, width, height);
    printf(" pre 3 $$ %dx%d\n", width, height);
}

static void
_etui_pdf_page_render(void *d)
{
    Etui_Provider_Data *pd;
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

    pd = (Etui_Provider_Data *)d;

    if (pd->page.use_display_list)
    {
        if (pd->page.list)
            fz_free(pd->doc.ctx, pd->page.list);
        pd->page.list = fz_new_display_list(pd->doc.ctx);

        dev = fz_new_list_device(pd->doc.ctx, pd->page.list);
        fz_run_page(pd->doc.doc, pd->page.page, dev, &fz_identity, &cookie);
        fz_free_device(dev);
    }
    fz_bound_page(pd->doc.doc, pd->page.page, &bounds);
    fz_pre_scale(fz_rotate(&ctm, pd->page.rotation), pd->page.hscale, pd->page.vscale);
    fz_round_rect(&ibounds, fz_transform_rect(&bounds, &ctm));
    width = ibounds.x1 - ibounds.x0;
    height = ibounds.y1 - ibounds.y0;
    image = fz_new_pixmap_with_data(pd->doc.ctx, NULL,
                                    width, height,
                                    (unsigned char *)pd->efl.m);

    fz_clear_pixmap_with_value(pd->doc.ctx, image, 0xff);
    dev = fz_new_draw_device(pd->doc.ctx, image);
    if (pd->page.use_display_list)
        fz_run_display_list(pd->page.list, dev, &ctm, &bounds, &cookie);
    else
        fz_run_page(pd->doc.doc, pd->page.page, dev, &ctm, &cookie);
    fz_free_device(dev);
    dev = NULL;
    pd->page.image = image;
}

static void
_etui_pdf_page_render_end(void *d)
{
    Etui_Provider_Data *pd;
    int width;
    int height;

    if (!d)
        return;

    DBG("render end");

    pd = (Etui_Provider_Data *)d;

    evas_object_image_size_get(pd->efl.obj, &width, &height);
    printf(" end $$ %dx%d\n", width, height);
    evas_object_image_data_set(pd->efl.obj, pd->efl.m);
    evas_object_image_data_update_add(pd->efl.obj, 0, 0, width, height);
    fz_drop_pixmap(pd->doc.ctx, pd->page.image);
}

/* code borrowed from doc_search.c */
static char *
_etui_pdf_page_text_extract(void *d, const Eina_Rectangle *rect)
{
    Etui_Provider_Data *pd;
    fz_text_sheet *sheet;
    fz_text_page *text;
    fz_device *dev;
    fz_cookie cookie = { 0 };
    fz_rect bounds;
    char *res;

    fz_rect hitbox;
    fz_text_block *block;
    fz_text_line *line;
    fz_text_span *span;
    char *iter;
    size_t len;
    int seen;
    int c;
    int i;

    if (!d)
        return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->page.use_display_list && !pd->doc.doc)
    {
        ERR("no opened document");
        return NULL;
    }

    bounds.x0 = rect->x;
    bounds.y0 = rect->y;
    bounds.x1 = rect->x + rect->w;
    bounds.y1 = rect->y + rect->h;

    text = fz_new_text_page(pd->doc.ctx);
    if (!text)
        return NULL;

    sheet = fz_new_text_sheet(pd->doc.ctx);
    if (!sheet)
    {
        fz_free_text_page(pd->doc.ctx, text);
        return NULL;
    }

    if (pd->page.use_display_list)
    {
        if (pd->page.list)
            fz_free(pd->doc.ctx, pd->page.list);
        pd->page.list = fz_new_display_list(pd->doc.ctx);

        dev = fz_new_list_device(pd->doc.ctx, pd->page.list);
        fz_run_page(pd->doc.doc, pd->page.page, dev, &fz_identity, &cookie);
        fz_run_display_list(pd->page.list, dev, &fz_identity, &bounds, &cookie);
    }
    else
    {
        dev = fz_new_text_device(pd->doc.ctx, sheet, text);
        fz_run_page(pd->doc.doc, pd->page.page, dev, &fz_identity, &cookie);
    }
    fz_free_device(dev);

    return fz_copy_selection(pd->doc.ctx, text, bounds);
}

/* stext-search.c */
static int textlen(fz_text_page *page)
{
        int len = 0;
        int block_num;

        for (block_num = 0; block_num < page->len; block_num++)
        {
                fz_text_block *block;
                fz_text_line *line;
                fz_text_span *span;

                if (page->blocks[block_num].type != FZ_PAGE_BLOCK_TEXT)
                        continue;
                block = page->blocks[block_num].u.text;
                for (line = block->lines; line < block->lines + block->len; line++)
                {
                        for (span = line->first_span; span; span = span->next)
                        {
                                len += span->len;
                        }
                        len++; /* pseudo-newline */
                }
        }
        return len;
}

/* code borrowed from doc_search.c */
static Eina_Array *
_etui_pdf_page_text_find(void *d, const char *needle)
{
    Etui_Provider_Data *pd;
    Eina_Array *rects = NULL;
    fz_text_page *text;
    fz_text_sheet *sheet;
    fz_device *dev;
    fz_cookie cookie = { 0 };
    fz_rect bounds;
    fz_text_block *block;
    fz_text_line *line;
    fz_text_span *span;
    size_t text_len;
    int n;
    size_t pos;
    int i;

    if (!d)
        return NULL;

    if (!needle || !*needle)
      return NULL;

    pd = (Etui_Provider_Data *)d;

    if (!pd->page.use_display_list && !pd->doc.doc)
    {
        ERR("no opened document");
        goto free_rects;
    }

    text = fz_new_text_page(pd->doc.ctx);
    if (!text)
        goto free_rects;

    sheet = fz_new_text_sheet(pd->doc.ctx);
    if (!sheet)
        goto free_text_page;

    if (pd->page.use_display_list)
    {
        if (pd->page.list)
            fz_free(pd->doc.ctx, pd->page.list);
        pd->page.list = fz_new_display_list(pd->doc.ctx);

        dev = fz_new_list_device(pd->doc.ctx, pd->page.list);
        fz_run_page(pd->doc.doc, pd->page.page, dev, &fz_identity, &cookie);
        fz_run_display_list(pd->page.list, dev, &fz_identity, &bounds, &cookie);
    }
    else
    {
        dev = fz_new_text_device(pd->doc.ctx, sheet, text);
        fz_run_page(pd->doc.doc, pd->page.page, dev, &fz_identity, &cookie);
    }
    fz_free_device(dev);

    text_len = textlen(text);

    for (pos = 0; pos < text_len; pos++)
    {
        n = _etui_pdf_page_text_match(text, needle, pos);
        if (n)
        {
            fz_rect linebox = fz_empty_rect;

            for (i = 0; i < n; i++)
            {
                fz_rect charbox;
                fz_char_and_box cab;

                charbox = fz_text_char_at(&cab, text, pos + i)->bbox;
                if (!fz_is_empty_rect(&charbox))
                {
                    if (charbox.y0 != linebox.y0 || fz_abs(charbox.x0 - linebox.x1) > 5)
                    {
                        if (!fz_is_empty_rect(&linebox))
                        {
                            Eina_Rectangle *r;

                            r = (Eina_Rectangle *)malloc(sizeof(Eina_Rectangle));
                            if (r)
                            {
                                r->x = linebox.x0;
                                r->y = linebox.y0;
                                r->w = linebox.x1 - linebox.x0;
                                r->h = linebox.y1 - linebox.y0;
                                if (!rects)
                                    rects = eina_array_new(4);
                                if (!rects)
                                    free(r);
                                else
                                {
                                    eina_array_push(rects, r);
                                }
                            }
                        }
                        linebox = charbox;
                    }
                    else
                    {
                        fz_union_rect(&linebox, &charbox);
                    }
                }
            }
            if (!fz_is_empty_rect(&linebox))
            {
                Eina_Rectangle *r;

                r = (Eina_Rectangle *)malloc(sizeof(Eina_Rectangle));
                if (r)
                {
                    r->x = linebox.x0;
                    r->y = linebox.y0;
                    r->w = linebox.x1 - linebox.x0;
                    r->h = linebox.y1 - linebox.y0;
                    if (!rects)
                        rects = eina_array_new(4);
                    if (!rects)
                        free(r);
                    else
                    {
                        eina_array_push(rects, r);
                    }
                }
            }
        }
    }

    fz_free_text_sheet(pd->doc.ctx, sheet);
    fz_free_text_page(pd->doc.ctx, text);

    return rects;

  free_text_page:
    fz_free_text_page(pd->doc.ctx, text);
  free_rects:
    if (rects)
        eina_array_free(rects);

    return NULL;
}

static float
_etui_pdf_page_duration_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return 0.0;

    pd = (Etui_Provider_Data *)d;

    return pd->page.duration;
}

static Etui_Transition
_etui_pdf_page_transition_type_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return ETUI_TRANSITION_NONE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->page.transition)
        return ETUI_TRANSITION_NONE;

    switch (pd->page.transition->type)
    {
        case FZ_TRANSITION_NONE:
            return ETUI_TRANSITION_NONE;
        case FZ_TRANSITION_SPLIT:
            return ETUI_TRANSITION_SPLIT;
        case FZ_TRANSITION_BLINDS:
            return ETUI_TRANSITION_BLINDS;
        case FZ_TRANSITION_BOX:
            return ETUI_TRANSITION_BOX;
        case FZ_TRANSITION_WIPE:
            return ETUI_TRANSITION_WIPE;
        case FZ_TRANSITION_DISSOLVE:
            return ETUI_TRANSITION_DISSOLVE;
        case FZ_TRANSITION_GLITTER:
            return ETUI_TRANSITION_GLITTER;
        case FZ_TRANSITION_FLY:
            return ETUI_TRANSITION_FLY;
        case FZ_TRANSITION_PUSH:
            return ETUI_TRANSITION_PUSH;
        case FZ_TRANSITION_COVER:
            return ETUI_TRANSITION_COVER;
        case FZ_TRANSITION_UNCOVER:
            return ETUI_TRANSITION_UNCOVER;
        case FZ_TRANSITION_FADE:
            return ETUI_TRANSITION_FADE;
        default:
        return ETUI_TRANSITION_NONE;
    }
}

static float
_etui_pdf_page_transition_duration_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return 0.0;

    pd = (Etui_Provider_Data *)d;

    if (!pd->page.transition)
        return 0.0;

    return pd->page.transition->duration;
}

static Eina_Bool
_etui_pdf_page_transition_vertical_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->page.transition)
        return EINA_FALSE;

    return pd->page.transition->vertical ? EINA_TRUE : EINA_FALSE;
}

static Eina_Bool
_etui_pdf_page_transition_outwards_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return EINA_FALSE;

    pd = (Etui_Provider_Data *)d;

    if (!pd->page.transition)
        return EINA_FALSE;

    return pd->page.transition->outwards ? EINA_TRUE : EINA_FALSE;
}

static int
_etui_pdf_page_transition_direction_get(void *d)
{
    Etui_Provider_Data *pd;

    if (!d)
        return 0;

    pd = (Etui_Provider_Data *)d;

    if (!pd->page.transition)
        return 0;

    return pd->page.transition->direction;
}

static Etui_Provider_Descriptor _etui_provider_descriptor_pdf =
{
    /* .name                          */ "pdf",
    /* .version                       */ ETUI_PROVIDER_DESCRIPTOR_VERSION,
    /* .priority                      */ ETUI_PROVIDER_DESCRIPTOR_PRIORITY_HIGH,
    /* .init                          */ _etui_pdf_init,
    /* .shutdown                      */ _etui_pdf_shutdown,
    /* .evas_object_get               */ _etui_pdf_evas_object_get,
    /* .file_open                     */ _etui_pdf_file_open,
    /* .file_close                    */ _etui_pdf_file_close,
    /* .version_get                   */ _etui_pdf_version_get,
    /* .title_get                     */ _etui_pdf_title_get,
    /* .author_get                    */ _etui_pdf_author_get,
    /* .subject_get                   */ _etui_pdf_subject_get,
    /* .keywords_get                  */ _etui_pdf_keywords_get,
    /* .creator_get                   */ _etui_pdf_creator_get,
    /* .producer_get                  */ _etui_pdf_producer_get,
    /* .creation_date_get             */ _etui_pdf_creation_date_get,
    /* .modification_date_get         */ _etui_pdf_modification_date_get,
    /* .is_printable                  */ _etui_pdf_is_printable,
    /* .is_changeable                 */ _etui_pdf_is_changeable,
    /* .is_copyable                   */ _etui_pdf_is_copyable,
    /* .is_notable                    */ _etui_pdf_is_notable,
    /* .password_needed               */ _etui_pdf_password_needed,
    /* .password_set                  */ _etui_pdf_password_set,
    /* .pages_count                   */ _etui_pdf_pages_count,
    /* .toc_get                       */ _etui_pdf_toc_get,
    /* .page_use_display_list_set     */ _etui_pdf_page_use_display_list_set,
    /* .page_use_display_list_get     */ _etui_pdf_page_use_display_list_get,
    /* .page_set                      */ _etui_pdf_page_set,
    /* .page_get                      */ _etui_pdf_page_get,
    /* .page_size_get                 */ _etui_pdf_page_size_get,
    /* .page_rotation_set             */ _etui_pdf_page_rotation_set,
    /* .page_rotation_get             */ _etui_pdf_page_rotation_get,
    /* .page_scale_set                */ _etui_pdf_page_scale_set,
    /* .page_scale_get                */ _etui_pdf_page_scale_get,
    /* .page_dpi_set                  */ NULL,
    /* .page_dpi_get                  */ NULL,
    /* .page_links_get                */ _etui_pdf_page_links_get,
    /* .page_render_pre               */ _etui_pdf_page_render_pre,
    /* .page_render                   */ _etui_pdf_page_render,
    /* .page_render_end               */ _etui_pdf_page_render_end,
    /* .page_text_extract             */ _etui_pdf_page_text_extract,
    /* .page_text_find                */ _etui_pdf_page_text_find,
    /* .page_duration_get             */ _etui_pdf_page_duration_get,
    /* .page_transition_type_get      */ _etui_pdf_page_transition_type_get,
    /* .page_transition_duration_get  */ _etui_pdf_page_transition_duration_get,
    /* .page_transition_vertical_get  */ _etui_pdf_page_transition_vertical_get,
    /* .page_transition_outwards_get  */ _etui_pdf_page_transition_outwards_get,
    /* .page_transition_direction_get */ _etui_pdf_page_transition_direction_get
};

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

Eina_Bool
etui_module_pdf_init(void)
{
    if (_etui_module_pdf_init_count > 0)
    {
        _etui_module_pdf_init_count++;
        return EINA_TRUE;
    }

    _etui_module_pdf_log_domain = eina_log_domain_register("etui-pdf",
                                                           ETUI_MODULE_PDF_DEFAULT_LOG_COLOR);
    if (_etui_module_pdf_log_domain < 0)
    {
        EINA_LOG_CRIT("Could not register log domain 'etui-pdf'");
        return EINA_FALSE;
    }

    if (!etui_module_register(&_etui_provider_descriptor_pdf))
    {
        ERR("Could not register module %p", &_etui_provider_descriptor_pdf);
        goto unregister_log;
    }

    _etui_module_pdf_init_count = 1;
    return EINA_TRUE;

  unregister_log:
    eina_log_domain_unregister(_etui_module_pdf_log_domain);
    _etui_module_pdf_log_domain = -1;

    return EINA_FALSE;
}

void
etui_module_pdf_shutdown(void)
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

    _etui_module_pdf_init_count = 0;

    etui_module_unregister(&_etui_provider_descriptor_pdf);

    eina_log_domain_unregister(_etui_module_pdf_log_domain);
    _etui_module_pdf_log_domain = -1;
}

#ifndef ETUI_BUILD_STATIC_PDF

EINA_MODULE_INIT(etui_module_pdf_init);
EINA_MODULE_SHUTDOWN(etui_module_pdf_shutdown);

#endif

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
