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
  Opens/creates the database and a new table (if it doesn't exist yet)
  Returns  sqlite3 error codes - in event of errors
  Returns  0 - success
*/
int
lib_sqlite3_create_table (char *name_db, char *name_table, char *sql_statement)
{
  sqlite3 *database;
  int rc;
  char *error;
	
  /* Open the sqlite db */
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_open() failed in lib_sqlite3_create_table().\n", rc);
    return rc;
  }

  /* Create a new table */
	if ((rc = sqlite3_exec (database,
                          (const char *) g_strdup_printf ("CREATE TABLE IF NOT EXISTS %s(%s)", name_table, sql_statement),
                          NULL,
                          NULL,
                          &error)) != SQLITE_OK)
  {
    sqlite3_close (database);
    printf ("* ERROR (%d) - sqlite3_exec() failed in lib_sqlite3_create_table().\n", rc);
    return rc;
  }

  /* Open the sqlite db */
  if ((rc = sqlite3_close (database)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_close() failed in lib_sqlite3_create_table().\n", rc);
    return rc;
  }
  
  return SQLITE_OK;
}

