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

#ifndef ETUI_MODULE_PS_UTILS_H
#define ETUI_MODULE_PS_UTILS_H

char *etui_strdup(const char *s);
int etui_strncasecmp (const char *s1, const char *s2, size_t n);
int etui_strcasecmp (const char *s1, const char *s2);
double etui_strtod(const char * pString, char ** ppEnd);

#endif
