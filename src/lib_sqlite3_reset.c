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
  Reset the given database

  Returns 0  - success
  Returns -1 - in event of error
*/
int
lib_sqlite3_reset_db (char *name_db, char *name_table)
{
  sqlite3 *database;
  sqlite3_stmt *pStmt;
  int rc;

  /* Open the sqlite db */
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_open () failed.\n", rc);
    return rc;
  }

  /* Prepare the SQL statement */
	if ((rc = sqlite3_prepare_v2 (database, (const char *) g_strdup_printf ("DELETE from %s", name_table), -1, &pStmt, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_prepare_v2() failed in lib_sqlite3_reset_db().\n", rc);
    sqlite3_close (database); 
    return rc;
	}

  /* Evaluate the SQL statement */	
  if ((rc = sqlite3_step (pStmt)) != SQLITE_DONE)
  {
    printf ("* ERROR (%d) - sqlite3_step() in lib_sqlite3_reset_db().\n", rc);
    return rc;
  }
		
  if ((rc = sqlite3_reset (pStmt))!= SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_reset() failed in lib_sqlite3_reset_db().\n", rc);
    sqlite3_close (database);
    return rc;
  }
		
  sqlite3_close (database);

  return 0;
}
