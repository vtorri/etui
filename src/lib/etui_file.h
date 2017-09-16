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

#ifndef ETUI_FILE_H
#define ETUI_FILE_H


const Etui_Module *etui_file_module_get(const Etui_File *ef);
EAPI const void *etui_file_base_get(const Etui_File *ef);
EAPI size_t etui_file_size_get(const Etui_File *ef);


#endif /* ETUI_FILE_H */
