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


#include <Eina.h>
#include <Evas.h>
#include <Ecore.h>
#include <Efreet_Mime.h>
#include <Eio.h>
#include <Etui.h>

int main(void)
{
    /* eina_init(); */
    /* ecore_init(); */
    efreet_mime_init();
    eio_init();

    printf("shutting down\n");
    eio_shutdown();
    efreet_mime_shutdown();
    /* ecore_shutdown(); */
    /* eina_shutdown(); */

    return 0;
}
