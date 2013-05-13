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


EAPI int etui_init(void);
EAPI int etui_shutdown(void);

EAPI Evas_Object *etui_object_add(Evas *evas);
EAPI Eina_Bool etui_object_file_set(Evas_Object *obj, const char *filename);
EAPI const char *etui_object_file_get(Evas_Object *obj);
EAPI const char *etui_object_filename_get(Evas_Object *obj);
EAPI Eina_Bool etui_object_document_password_set(Evas_Object *obj, const char *password);
EAPI int etui_object_document_pages_count(Evas_Object *obj);


#endif /* ETUI_H */
