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
 Looks for key_value in the key column (of the given table/db).
 If it exists reads the value (TEXT) of the given column (INT) and saves it in the int* given.

 Returns 0                  - success
 Returns sqlite code error  - in event of error
*/
int
lib_sqlite3_read_int (char *name_db, char *name_table, char *key, char *key_value, int column, int *value)
{
  sqlite3 *database;
  sqlite3_stmt *pStmt;
  int rc, id;

  /* Open the sqlite db */	
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR - sqlite3_open() failed.\n");
    return -1;
  }

  /* Search the element in the db */
  if ((rc = lib_sqlite3_search_string (name_db, name_table, key, key_value)) != 0)
  {
    sqlite3_close (database);
    return rc;
  }

  /* Prepare the SQL statement */
	if ((rc = sqlite3_prepare_v2 (database, (const char *) g_strdup_printf ("SELECT * from %s WHERE %s=?", name_table, key), -1, &pStmt, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR - sqlite3_prepare_v2() failed in lib_sqlite3_read_int().\n");
    sqlite3_close (database); 
    return -1;
  }
		
  if ((rc = sqlite3_bind_text (pStmt, 1, key_value, -1, NULL)) != SQLITE_OK)
  {
    printf ("*ERROR - sqlite3_bind_text() failed in lib_sqlite3_read_int().\n");
    sqlite3_close (database); 
    return -1;
  }

  /* Evaluate the SQL statement */
  if ((rc = sqlite3_step (pStmt)) != SQLITE_ROW)
  {
    printf ("* ERROR - sqlite3_step() failed in lib_sqlite3_read_int().\n");
    return -1;
  }

  /* Get the value */
  *value = sqlite3_column_int (pStmt, column);

  rc = sqlite3_clear_bindings (pStmt);

  if ((rc = sqlite3_reset (pStmt)) != SQLITE_OK)
  {
    printf ("* ERROR - sqlite3_reset() failed in lib_sqlite3_read_int().\n");
    sqlite3_close (database);
    return -1;
  }

  sqlite3_close (database); 
  return 0;
}


/*
 Looks for key_value in the key column (of the given table/db).
 If it exists reads the value (TEXT) of the given column (INT) and returns it.

 Returns char* - success
 Returns NULL  - in event of error
*/
char *
lib_sqlite3_read_str (char *name_db, char *name_table, char *key, char *key_value, int column)
{
  sqlite3 *database;
  sqlite3_stmt *pStmt;
  int rc;
  char *value;

  /* Open the sqlite db */	
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_open() failed.\n", rc);
    return NULL;
  }

  /* Search the element in the db */
  if ((rc = lib_sqlite3_search_string (name_db, name_table, key, key_value)) != 0)
  {
    sqlite3_close (database);
    return NULL;
  }

  /* Prepare the SQL statement */
	if ((rc = sqlite3_prepare_v2 (database, (const char *) g_strdup_printf ("SELECT * from %s WHERE %s=?", name_table, key), -1, &pStmt, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_prepare_v2() failed in lib_sqlite3_read_int().\n", rc);
    sqlite3_close (database); 
    return NULL;
  }
		
  if ((rc = sqlite3_bind_text (pStmt, 1, key_value, -1, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_bind_text() failed in lib_sqlite3_read_int().\n", rc);
    sqlite3_close (database); 
    return NULL;
  }

  /* Evaluate the SQL statement */
  if ((rc = sqlite3_step (pStmt)) != SQLITE_ROW)
  {
    printf ("* ERROR (%d) - sqlite3_step() failed in lib_sqlite3_read_int().\n", rc);
    return NULL;
  }

  /* Get the value */
  value = (char *) sqlite3_column_text (pStmt, column);

  rc = sqlite3_clear_bindings (pStmt);

  if ((rc = sqlite3_reset (pStmt)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_reset() failed in lib_sqlite3_read_int().\n", rc);
    sqlite3_close (database);
    return NULL;
  }

  sqlite3_close (database); 
  return value;
}
