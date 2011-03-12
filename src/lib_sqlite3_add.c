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
  Inserts a new element (just one) in the db (in the specified table).
  "position" value represents the numner of the column

  Returns 0                  - success
  Returns sqlite3 error code - in event of error
*/
int
lib_sqlite3_add_set_position (char *name_db, char *name_table, char *key, char *key_value, char *element_to_add, int position)
{
	sqlite3 *database;
	sqlite3_stmt *pStmt;
	int rc;

  /* Open the sqlite db */
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_open() failed in lib_sqlite3_add_set_position().\n", rc);
    return rc;
  }

  /* Prepare the SQL statement */
  if ((rc = sqlite3_prepare_v2 (database, (const char *) g_strdup_printf ("INSERT INTO %s VALUES (?,?,?,?,NULL)", name_table), -1, &pStmt, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_prepare_v2() failed in lib_sqlite3_add_set_position().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  if ((rc = sqlite3_bind_text (pStmt, position, element_to_add, -1, NULL)) != SQLITE_OK)
  {
		printf ("* ERROR (%d) - sqlite3_bind_text() failed in lib_sqlite3_add_set_position().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  /* Evaluate the SQL statement */
	if ((rc = sqlite3_step (pStmt)) != SQLITE_DONE)
  {
    printf ("* ERROR (%d) - sqlite3_step() failed in lib_sqlite3_add_set_position().\n", rc);
    sqlite3_close (database);
    return rc;
	}

  /* Reset the bindings on a prepared statement - maybe useless! */
  rc = sqlite3_clear_bindings (pStmt);

  if ((rc = sqlite3_reset (pStmt))!= SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_reset() failed in lib_sqlite3_add_set_position().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  sqlite3_close (database);

  return 0;
}


/*
  Inserts a new element (just one) in the db (in the specified table).
  "position" value represents the numner of the column.
  The new value is inserted only if the key_value doesn't exixt in the db.

  Returns 0                  - success
  Returns sqlite3 error code - in event of error
*/
int
lib_sqlite3_add_set_position_check_presence (char *name_db, char *name_table, char *key, char *key_value, char *element_to_add, int position)
{
	sqlite3 *database;
	sqlite3_stmt *pStmt;
	int rc;

  /* Open the sqlite db */
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_open() failed in lib_sqlite3_add_set_position_check_presence().\n", rc);
    return rc;
  }

  /* Search the element in the db */
  if ((rc = lib_sqlite3_search_string (name_db, name_table, key, key_value)) == 0)
  {
    sqlite3_close (database);
    return rc;
  }

  /* Prepare the SQL statement */
  if ((rc = sqlite3_prepare_v2 (database, (const char *) g_strdup_printf ("INSERT INTO %s VALUES (?,?,?,?,NULL)", name_table), -1, &pStmt, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_prepare_v2() failed in lib_sqlite3_add_set_position_check_presence().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  if ((rc = sqlite3_bind_text (pStmt, position, element_to_add, -1, NULL)) != SQLITE_OK)
  {
		printf ("* ERROR (%d) - sqlite3_bind_text() failed in lib_sqlite3_add_set_position_check_presence().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  /* Evaluate the SQL statement */
	if ((rc = sqlite3_step (pStmt)) != SQLITE_DONE)
  {
    printf ("* ERROR (%d) - sqlite3_step() failed in lib_sqlite3_add_set_position_check_presence().\n", rc);
    sqlite3_close (database);
    return rc;
	}

  /* Reset the bindings on a prepared statement - maybe useless! */
  rc = sqlite3_clear_bindings (pStmt);

  if ((rc = sqlite3_reset (pStmt))!= SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_reset() failed in lib_sqlite3_add_set_position_check_presence().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  sqlite3_close (database);

  return 0;
}


/*
  Inserts new elements (the full row, char**) in the db (in the specified table).

  Returns 0                  - success
  Returns sqlite3 error code - in event of error
*/
int
lib_sqlite3_add_with_list (char *name_db, char *name_table, char **elements_to_add, int num_of_elem)
{
	sqlite3 *database;
	sqlite3_stmt *pStmt;
	int rc, i;
  char *query;

  /* Create SQL statement */
  query = (char *) g_strdup_printf ("INSERT INTO %s VALUES (?,", name_table);

  for (rc = 0; rc < num_of_elem - 1; rc++)
    query = (char *) g_strdup_printf ("%s?,", query);
  
  /* Last element to add, without coma */
  query = (char *) g_strdup_printf ("%s?)", query);

  /* Open the sqlite db */
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_open() failed in lib_sqlite3_add_with_list().\n", rc);
    return rc;
  }

  /* Prepare the SQL statement */
  if ((rc = sqlite3_prepare_v2 (database, query, -1, &pStmt, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_prepare_v2() failed in lib_sqlite3_add_with_list().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  for (i = 0; i <= num_of_elem; i++)
    printf ("*  adding: %s (pos %d)\n", elements_to_add[i], i + 1);

  for (i = 0; i <= num_of_elem; i++)
  {
    if ((rc = sqlite3_bind_text (pStmt, i + 1, elements_to_add[i], -1, NULL)) != SQLITE_OK)
    {
		  printf ("* ERROR (%d) - sqlite3_bind_text() failed in lib_sqlite3_add_with_list().\n", rc);
      sqlite3_close (database);
      return rc;
    }
  }

  /* Evaluate the SQL statement */
	if ((rc = sqlite3_step (pStmt)) != SQLITE_DONE)
  {
    printf ("* ERROR (%d) - sqlite3_step() failed in lib_sqlite3_add_with_list().\n", rc);
    sqlite3_close (database);
    return rc;
	}

  /* Reset the bindings on a prepared statement - maybe useless! */
  rc = sqlite3_clear_bindings (pStmt);

  if ((rc = sqlite3_reset (pStmt))!= SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_reset() failed in lib_sqlite3_add_with_list().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  if ((rc = sqlite3_close (database))!= SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_close() failed in lib_sqlite3_add_with_list().\n", rc);
    return rc;
  }

  return 0;
}


/*
  Inserts new elements (the full row, char**) in the db (in the table specified) only if key_value doesn't exist yet.

  Returns 0                   - success
  Returns sqlite3 error code  - in event of error
*/
int
lib_sqlite3_add_with_list_check_presence (char *name_db, char *name_table, char *key, char *key_value, char **elements_to_add, int num_of_elem)
{
  sqlite3 *database;
  sqlite3_stmt *pStmt;
  int rc, i;
  char *query;

  /* Create SQL statement */
  query = (char *) g_strdup_printf ("INSERT INTO %s VALUES (?,", name_table);

  for (rc = 0; rc < num_of_elem - 1; rc++)
    query = (char *) g_strdup_printf ("%s?,", query);
  
  /* Last element to add, without coma */
  query = (char *) g_strdup_printf ("%s?)", query);

  /* Open the sqlite db */
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_open() failed in lib_sqlite3_add_with_list_check_presence().\n", rc);
    return rc;
  }

  /* Search for the element in the db | if it doesn't exist we can't continue -> return */
  if ((rc = lib_sqlite3_search_string (name_db, name_table, key, key_value)) == 0)
  {
    printf ("* The element still exists in the db.\n");
    sqlite3_close (database);
    return rc;
  }

  /* Prepare the SQL statement */
  if ((rc = sqlite3_prepare_v2 (database, query, -1, &pStmt, NULL)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_prepare_v2() failed in lib_sqlite3_add_with_list_check_presence().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  for (i = 0; i <= num_of_elem; i++)
  {
    printf ("*  adding: %s (pos %d)\n", elements_to_add[i], i + 1);
    if ((rc = sqlite3_bind_text (pStmt, i + 1, elements_to_add[i], -1, NULL)) != SQLITE_OK)
    {
		  printf ("* ERROR (%d) - sqlite3_bind_text() failed in lib_sqlite3_add_with_list_check_presence().\n", rc);
      sqlite3_close (database);
      return rc;
    }
  }

  /* Evaluate the SQL statement */
	if ((rc = sqlite3_step (pStmt)) != SQLITE_DONE)
  {
    printf ("* ERROR (%d) - sqlite3_step() failed in lib_sqlite3_add_with_list_check_presence().\n", rc);
    sqlite3_close (database);
    return rc;
	}

  /* Reset the bindings on a prepared statement - maybe useless! */
  rc = sqlite3_clear_bindings (pStmt);

  if ((rc = sqlite3_reset (pStmt))!= SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_reset() failed in lib_sqlite3_add_with_list_check_presence().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  if ((rc = sqlite3_close (database))!= SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_close() failed in lib_sqlite3_add_with_list_check_presence().\n", rc);
    return rc;
  }

  return 0;
}


/*
  Looks for the given key_value in the database and update its fields; if it doesn't exist yet, the func inserts them in the db as a new row.

  Returns 0                   - success
  Returns sqlite3 error code  - in event of error
*/
int
lib_sqlite3_add_or_update_with_list (char *name_db, char *name_table, char *key, char *key_value, char **elements_to_add, char **name_columns, int num_of_elem)
{
  sqlite3 *database;
  sqlite3_stmt *pStmt;
  int rc, i = 0;
  char *query;	

  /* Open the sqlite db */
  if ((rc = sqlite3_open (name_db, &database)) != SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_open() failed in lib_sqlite3_add_or_update().\n", rc);
    return rc;
  }

  /* Search the element in the db | return in event of error */
  if ((rc = lib_sqlite3_search_string (name_db, name_table, key, key_value)) > 0)
  {
    printf ("* ERROR (%d) - lib_sqlite3_search_string() failed in lib_sqlite3_add_or_update().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  /* Add new elements */
  if (rc == -1)
  {
    printf ("\n * Create...\n");

    /* Create SQL statement */
    query = (char *) g_strdup_printf ("INSERT INTO %s VALUES (?,", name_table);

    for (rc = 0; rc < num_of_elem - 1; rc++)
      query = (char *) g_strdup_printf ("%s?,", query);
    
    /* Last element to add, without coma */
    query = (char *) g_strdup_printf ("%s?)", query);

    if ((rc = sqlite3_prepare_v2 (database, query, -1, &pStmt, NULL)) != SQLITE_OK)
    {
      printf ("* ERROR (%d) - sqlite3_prepare_v2() in lib_sqlite3_add_or_update().\n", rc);
      sqlite3_close (database);
      return rc;
    }

    for (i = 0; i <= num_of_elem; i++)
    {
      printf ("*  adding: %s (pos %d)\n", elements_to_add[i], i + 1);
      if ((rc = sqlite3_bind_text (pStmt, i + 1, elements_to_add[i], -1, NULL)) != SQLITE_OK)
      {
		    printf ("* ERROR (%d) - sqlite3_bind_text() failed in lib_sqlite3_add().\n", rc);
        sqlite3_close (database);
        return rc;
      }
    }
  }

  /* Update the elements */
  else if (rc == 0)
  {

    /* Create SQL statement */
    /* UPDATE prova SET UID=?, DATA=?, INFO=?, CONT=?, NUM=? WHERE UID=? */

    for (i = 0; i < num_of_elem; i++)
      printf ("%s\n", name_columns[i]);


    query = (char *) g_strdup_printf ("UPDATE %s SET %s=?,", name_table, name_columns[0]);

    for (i = 1; i < num_of_elem - 1; i++)
      query = (char *) g_strdup_printf ("%s %s=?,", query, name_columns[i]);

    query = (char *) g_strdup_printf ("%s %s=?", query, name_columns[i]);

    query = (char *) g_strdup_printf ("%s WHERE %s=?;", query, key);

    printf ("\n* query: %s\n", query);

    if ((rc = sqlite3_prepare_v2 (database, query, -1, &pStmt, NULL)) != SQLITE_OK)
    {
      printf ("* ERROR (%d) - sqlite3_prepare_v2() #2 failed in lib_sqlite3_add_or_update().\n", rc);
      sqlite3_close (database);
      return -1;
    }

    /* Set columns' names */
    for (i = 0; i <= num_of_elem; i++)
    {
      if ((rc = sqlite3_bind_text (pStmt, i + 1, elements_to_add[i], -1, NULL)) != SQLITE_OK)
      {
        printf ("* ERROR (%d) - sqlite3_bind_text() failed in lib_sqlite3_add_or_update().\n", rc);
        sqlite3_close (database);
        return -1;
      }
    }

    /* Set key's value */
    if ((rc = sqlite3_bind_text (pStmt, i + 1, (const char*) key_value, -1, NULL)) != SQLITE_OK)
    {
      printf ("* ERROR (%d) - sqlite3_bind_text() failed in lib_sqlite3_add_or_update().\n", rc);
      sqlite3_close (database);
      return -1;
    }

  }

  if ((rc = sqlite3_step (pStmt)) != SQLITE_DONE)
  {
    printf ("* ERROR (%d) - sqlite3_step() in lib_sqlite3_add_or_update().\n", rc);
    sqlite3_close (database);
    return -1;
  }
	
  i = sqlite3_clear_bindings (pStmt);
	
  if ((rc = sqlite3_reset (pStmt))!= SQLITE_OK)
  {
    printf ("* ERROR (%d) - sqlite3_reset() failed in lib_sqlite3_add_or_update().\n", rc);
    sqlite3_close (database);
    return rc;
  }

  sqlite3_close (database);
  return 0;
}
