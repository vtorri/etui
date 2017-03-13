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
    ETUI_ROTATION_0 = 0,
    ETUI_ROTATION_90 = 90,
    ETUI_ROTATION_180 = 180,
    ETUI_ROTATION_270 = 270
} Etui_Rotation;


typedef struct Etui_File_s Etui_File;


EAPI int etui_init(void);
EAPI int etui_shutdown(void);

EAPI Etui_File *etui_file_new (const char *filename);
EAPI void etui_file_del(Etui_File *ef);
EAPI const char *etui_file_filename_get(const Etui_File *ef);

EAPI Evas_Object *etui_object_add(Evas *evas);

EAPI const char *etui_object_module_name_get(Evas_Object *obj);

EAPI const void *etui_object_info_get(Evas_Object *obj);

EAPI const char *etui_object_title_get(Evas_Object *obj);

EAPI int etui_object_document_pages_count(Evas_Object *obj);
EAPI const Eina_Array *etui_object_toc_get(Evas_Object *obj);

EAPI void etui_object_page_set(Evas_Object *obj, int page_num);
EAPI int etui_object_page_get(Evas_Object *obj);
EAPI void etui_object_page_size_get(Evas_Object *obj, int *width, int *height);
EAPI void etui_object_page_rotation_set(Evas_Object *obj, Etui_Rotation rotation);
EAPI Etui_Rotation etui_object_page_rotation_get(Evas_Object *obj);
EAPI void etui_object_page_scale_set(Evas_Object *obj, float hscale, float vscale);
EAPI void etui_object_page_scale_get(Evas_Object *obj, float *hscale, float *vscale);

/*** specific module features ***/

/* cb */

typedef enum
{
    ETUI_CB_CBZ,
    ETUI_CB_CBR,
    ETUI_CB_CBA,
    ETUI_CB_CB7,
    ETUI_CB_CBT,
} Etui_Cb_Type;

typedef struct
{
    Etui_Cb_Type type;
} Etui_Module_Cb_Info;

/* djvu */

typedef enum
{
    ETUI_DJVU_PAGE_TYPE_UNKNOWN,
    ETUI_DJVU_PAGE_TYPE_BITONAL,
    ETUI_DJVU_PAGE_TYPE_PHOTO,
    ETUI_DJVU_PAGE_TYPE_COMPOUND
} Etui_Djvu_Page_Type;

typedef struct
{
    double page_gamma;
    int page_dpi;
    Etui_Djvu_Page_Type page_type;
} Etui_Module_Djvu_Info;

/* pdf */

typedef enum
{
    ETUI_TRANSITION_NONE, /* aka 'R' or 'REPLACE' */
    ETUI_TRANSITION_SPLIT,
    ETUI_TRANSITION_BLINDS,
    ETUI_TRANSITION_BOX,
    ETUI_TRANSITION_WIPE,
    ETUI_TRANSITION_DISSOLVE,
    ETUI_TRANSITION_GLITTER,
    ETUI_TRANSITION_FLY,
    ETUI_TRANSITION_PUSH,
    ETUI_TRANSITION_COVER,
    ETUI_TRANSITION_UNCOVER,
    ETUI_TRANSITION_FADE
} Etui_Transition;

typedef enum
{
    ETUI_LINK_KIND_GOTO,
    ETUI_LINK_KIND_URI
} Etui_Link_Kind;

typedef struct
{
    int page;
    float page_x;
    float page_y;
} Etui_Link_Goto;

typedef struct
{
    char *uri;
    unsigned int is_open : 1;
} Etui_Link_Uri;

typedef union
{
    Etui_Link_Goto goto_;
    Etui_Link_Uri uri;
} Etui_Link_Dest;

typedef struct
{
    Etui_Link_Kind kind;
    Etui_Link_Dest dest;
} Etui_Link_Item;

typedef struct
{
    Etui_Link_Kind kind;
    char *title;
    Etui_Link_Dest dest;
    Eina_Array *child;
} Etui_Toc_Item;

typedef struct
{
    char *author;
    char *subject;
    char *keywords;
    char *creator;
    char *producer;
    char *creation_date;
    char *modification_date;
    char *encryption;
} Etui_Module_Pdf_Info;

/* tiff */

typedef struct
{
    char *artist;
    char *copyright;
    char *date_time;
    char *document_name;
    char *image_description;
    char *make;
    char *model;
    char *software;
    float resolution_x;
    float resolution_y;
    unsigned int rows_per_strip;
    unsigned short bits_per_sample;
    unsigned short compression;
    unsigned short orientation;
    unsigned short photometric;
    unsigned short planar_configuration;
    unsigned short resolution_unit;
    unsigned short sample_format;
    unsigned short samples_per_pixel;
    unsigned short ycbcr_subssampling[2];
} Etui_Module_Tiff_Info;


#endif /* ETUI_H */
