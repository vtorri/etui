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
#include <Ecore.h>
#include <Efreet_Mime.h>
#include <Eio.h>

#include "Etui.h"
#include "etui_private.h"
#include "etui_module.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

static int _etui_init_count = 0;

/**
 * @endcond
 */


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

int etui_log_dom_global = -1;

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

EAPI int
etui_init(void)
{

    if (++_etui_init_count != 1)
        return _etui_init_count;

    if (!eina_init())
    {
        fprintf(stderr, "Etui: Could not initialize Eina.\n");
        return --_etui_init_count;
    }

    etui_log_dom_global = eina_log_domain_register("etui", ETUI_DEFAULT_LOG_COLOR);
    if (etui_log_dom_global < 0)
    {
        EINA_LOG_ERR("Etui: Could not register log domain 'etui'.");
        goto shutdown_eina;
    }

    if (!ecore_init())
    {
        ERR("Could not initialize Ecore.");
        goto unregister_log_domain;
    }

    if (!efreet_mime_init())
    {
        ERR("Could not initialize Efreet Mime.");
        goto shutdown_ecore;
    }

    if (!eio_init())
    {
        ERR("Could not initialize Eio.");
        goto shutdown_efreet_mime;
    }

    if (!etui_modules_init())
    {
        ERR("Could not initialize module system.");
        goto shutdown_eio;
    }

    return _etui_init_count;

  shutdown_eio:
    eio_shutdown();
  shutdown_efreet_mime:
    efreet_mime_shutdown();
  shutdown_ecore:
    ecore_shutdown();
  unregister_log_domain:
    eina_log_domain_unregister(etui_log_dom_global);
    etui_log_dom_global = -1;
  shutdown_eina:
    eina_shutdown();
    return --_etui_init_count;
}

EAPI int
etui_shutdown(void)
{
    if (_etui_init_count <= 0)
    {
        ERR("Init count not greater than 0 in shutdown.");
        return 0;
    }
    if (--_etui_init_count != 0)
        return _etui_init_count;

    etui_modules_shutdown();
    eio_shutdown();
    efreet_mime_shutdown();
    ecore_shutdown();
    eina_log_domain_unregister(etui_log_dom_global);
    etui_log_dom_global = -1;
    eina_shutdown();

    return _etui_init_count;
}
