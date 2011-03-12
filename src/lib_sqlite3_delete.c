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
  Deletes a row from the db

  Returns 0                   - success
  Returns sqlite3 error code  - in event of error
*/
int
lib_sqlite3_delete_row (char *name_db, char *name_table, char *key, char *key_value)
{
  sqlite3 *database;
  sqlite3_stmt *pStmt;
  int rc;
  const unsigned char *id;

  /* Open the sqlite db */
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_open() failed in lib_sqlite3_delete_row().\n", rc);
    return rc;
  }

  /* Search the element in the db */
  if ((rc = lib_sqlite3_search_string (name_db, name_table, key, key_value)) != 0)
  {
    printf ("* ERROR (%d) - lib_sqlite3_search() failed in lib_sqlite_delete_row().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  /* Prepare the SQL statement */		
  if ((rc = sqlite3_prepare_v2 (database, (char *) g_strdup_printf ("DELETE from %s WHERE %s=?", name_table, key), -1, &pStmt, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_prepare_v2() failed in lib_sqlite3_delete_row().\n", rc);
    sqlite3_close (database); 
    return -1;
  }
		
  if ((rc = sqlite3_bind_text (pStmt, 1, key_value, -1, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_bind_text() failed in lib_sqlite3_delete_row().\n", rc);
    sqlite3_close (database); 
    return -1;
  }

  /* Evaluate the SQL statement */
  if ((rc = sqlite3_step (pStmt)) != SQLITE_DONE)
  {
    printf ("* ERROR (%d) - sqlite3_step() failed in lib_sqlite3_delete_row().\n", rc);
    return rc;
  }

  rc = sqlite3_clear_bindings (pStmt);

  if ((rc = sqlite3_reset (pStmt))!= SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_reset() failed in lib_sqlite3_delete().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  /* Close the sqlite db */
  if ((rc = sqlite3_close (database))!= SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_close() failed in lib_sqlite3_delete().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  return 0;
}

