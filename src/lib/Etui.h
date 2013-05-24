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

#ifndef ETUI_H
#define ETUI_H

#include <Eina.h>
#include <Evas.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef ECRIN_ETUI_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif


typedef enum
{
    ETUI_ROTATION_0,
    ETUI_ROTATION_90,
    ETUI_ROTATION_180,
    ETUI_ROTATION_270
} Etui_Rotation;

typedef enum
{
    ETUI_LINK_KIND_NONE,
    ETUI_LINK_KIND_GOTO,
    ETUI_LINK_KIND_GOTO_REMOTE,
    ETUI_LINK_KIND_URI,
    ETUI_LINK_KIND_LAUNCH,
    ETUI_LINK_KIND_NAMED,
} Etui_Link_Kind;

typedef struct
{
    int page;
    /* int flags; */
    /* fz_point lt; */
    /* fz_point rb; */
    unsigned int new_window : 1;
} Etui_Link_Goto;

typedef struct
{
    int page;
    /* int flags; */
    /* fz_point lt; */
    /* fz_point rb; */
    char *filename;
    unsigned int new_window : 1;
} Etui_Link_Goto_Remote;

typedef struct
{
    char *uri;
    unsigned int is_map : 1;
} Etui_Link_Uri;

typedef struct
{
    char *filename;
    unsigned int new_window : 1;
} Etui_Link_Launch;

typedef struct
{
    char *named;
} Etui_Link_Named;

typedef union
{
    Etui_Link_Goto goto_;
    Etui_Link_Goto_Remote goto_remote;
    Etui_Link_Uri uri;
    Etui_Link_Launch launch;
    Etui_Link_Named named;
} Etui_Link_Dest;

typedef struct
{
    Etui_Link_Kind kind;
    Eina_Rectangle rect;
    Etui_Link_Dest dest;
} Etui_Link_Item;

typedef struct
{
    Etui_Link_Kind kind;
    char *title;
    Etui_Link_Dest dest;
    Eina_Array *child;
} Etui_Toc_Item;


EAPI int etui_init(void);
EAPI int etui_shutdown(void);

EAPI Evas_Object *etui_object_add(Evas *evas);
EAPI Eina_Bool etui_object_file_set(Evas_Object *obj, const char *filename);
EAPI const char *etui_object_filename_get(Evas_Object *obj);

EAPI void etui_object_version_get(Evas_Object *obj, int *maj, int *min);
EAPI char *etui_object_title_get(Evas_Object *obj);
EAPI char *etui_object_author_get(Evas_Object *obj);
EAPI char *etui_object_subject_get(Evas_Object *obj);
EAPI char *etui_object_keywords_get(Evas_Object *obj);
EAPI char *etui_object_creator_get(Evas_Object *obj);
EAPI char *etui_object_producer_get(Evas_Object *obj);
EAPI char *etui_object_creation_date_get(Evas_Object *obj);
EAPI char *etui_object_modification_date_get(Evas_Object *obj);
Eina_Bool etui_object_is_printable(Evas_Object *obj);
Eina_Bool etui_object_is_changeable(Evas_Object *obj);
Eina_Bool etui_object_is_copyable(Evas_Object *obj);
Eina_Bool etui_object_is_notable(Evas_Object *obj);

EAPI Eina_Bool etui_object_document_password_needed(Evas_Object *obj);
EAPI Eina_Bool etui_object_document_password_set(Evas_Object *obj, const char *password);
EAPI int etui_object_document_pages_count(Evas_Object *obj);
EAPI const Eina_Array *etui_object_toc_get(Evas_Object *obj);

EAPI void etui_object_page_use_display_list_set(Evas_Object *obj, Eina_Bool on);
EAPI Eina_Bool etui_object_page_use_display_list_get(Evas_Object *obj);
EAPI void etui_object_page_set(Evas_Object *obj, int page_num);
EAPI int etui_object_page_get(Evas_Object *obj);
EAPI void etui_object_page_size_get(Evas_Object *obj, int *width, int *height);
EAPI void etui_object_page_rotation_set(Evas_Object *obj, Etui_Rotation rotation);
EAPI Etui_Rotation etui_object_page_rotation_get(Evas_Object *obj);
EAPI void etui_object_page_scale_set(Evas_Object *obj, float hscale, float vscale);
EAPI void etui_object_page_scale_get(Evas_Object *obj, float *hscale, float *vscale);
EAPI const Eina_Array *etui_object_page_links_get(Evas_Object *obj);


#endif /* ETUI_H */
