/*
 * Copyright © 2011
 * Andrea Costa <nukemup@hotmail.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 * or write to the Free Software Foundation, Inc., 51 Franklin St
 * Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "lib_sqlite3.h"


/*
  Looks in the db for the tag ID (given)
  Returns  sqlite3 error codes - in event of errors
  Returns  0 - if the element exists in the db
  Returns -1 - if the element doesn't exist in the db
*/
int
lib_sqlite3_search_string (char *name_db, char *name_table, char *name_field, char *element_to_search)
{
  sqlite3 *database = NULL;
  sqlite3_stmt *pStmt = NULL;
  const char *tail;
  int rc;

  /* Open the sqlite db */
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR - sqlite3_open() failed.\n");
    return rc;
  }

  if ((rc = sqlite3_prepare_v2 (database, (const char *) g_strdup_printf ("SELECT * FROM %s WHERE %s=?", name_table, name_field), -1, &pStmt, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_prepare_v2() failed in lib_sqlite3_search_string().\n", rc);
  	sqlite3_close (database);
    return rc;
  }

  if ((rc = sqlite3_bind_text (pStmt, 1, element_to_search, -1, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_bind_text() failed in lib_sqlite3_search_string().\n", rc);
  	sqlite3_close (database);
    return rc;
  }
	
	if (sqlite3_step (pStmt) != SQLITE_ROW)
  {
    rc = sqlite3_clear_bindings (pStmt);

    if ((rc = sqlite3_reset (pStmt)) != SQLITE_OK)
    {
      printf ("* ERROR (%d) - sqlite3_reset() failed in lib_sqlite3_search_string().\n", rc);
    	sqlite3_close (database);
      return rc;
    }

    return -1;
  }

  rc = sqlite3_clear_bindings (pStmt);
	
  if ((rc = sqlite3_reset (pStmt))!= SQLITE_OK)
  {
    printf("* ERROR - sqlite3_reset() failed.\n");
  	sqlite3_close (database);
    return rc;
  }

  sqlite3_close (database);
  return 0;
}
