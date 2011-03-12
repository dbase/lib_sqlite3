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
  Returns a char** with the columns's names of the given table

  Returns NULL - in event of error
*/
char **
lib_sqlite3_get_column_name (char *name_db, char *name_table)
{
  sqlite3 *database;
  char *err = 0;
  int i, rc, j;
  char **result;
  char **name_columns;
  int nrow, ncol;

  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf("* ERROR (%d) - sqlite3_open() failed in lib_sqlite3_get_table().\n", rc);
    sqlite3_close (database);
    return NULL;
  }

  if ((rc = sqlite3_get_table (database, (const char *) g_strdup_printf ("SELECT * from %s", name_table), &result, &nrow, &ncol, &err)) != SQLITE_OK)
  {
    printf("* ERROR (%d) - sqlite3_get_table() failed in lib_sqlite3_get_table().\n", rc);
    sqlite3_close (database);
    return NULL;
  }

  name_columns = malloc (ncol * sizeof(char **));

  for (i = 0 ; i < ncol; i++)
    name_columns[i] = result[i];

  sqlite3_free_table (result);

  if ((rc = sqlite3_close (database)) != SQLITE_OK)
  {
    printf("* ERROR (%d) - sqlite3_close() failed in lib_sqlite3_get_table().\n", rc);
    sqlite3_close (database);
    return NULL;
  }

	return name_columns;
}


/* 
  Saves in the given int variables the number of rows and columns

  Returns 0                   - success
  Returns sqlite3 error code - in event of error
*/
int
lib_sqlite3_get_nrow_and_ncol (char *name_db, char *name_table, int *nrow, int *ncol)
{
  sqlite3 *database;
  char *err = 0;
  int i, rc;
  char **result;

  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf("* ERROR (%d) - sqlite3_open() failed in lib_sqlite3_get_nrow_and_ncol().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  if ((rc = sqlite3_get_table (database, (const char *) g_strdup_printf ("SELECT * from %s", name_table), &result, nrow, ncol, &err)) != SQLITE_OK)
  {
    printf("* ERROR (%d) - sqlite3_get_table() failed in lib_sqlite3_get_nrow_and_ncol().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  sqlite3_free_table (result);
	
  if ((rc = sqlite3_close (database)) != SQLITE_OK)
  {
    printf("* ERROR (%d) - sqlite3_close() failed in lib_sqlite3_get_nrow_and_ncol().\n", rc);
    sqlite3_close (database);
    return rc;
  }

	return 0;
}
