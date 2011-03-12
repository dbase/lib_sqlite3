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

#ifndef __LIB_SQLITE3_H__
#define __LIB_SQLITE3_H__

#define VERSION_LIB_SQLITE3 "0.0.1"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>


/* lib_sqlite3_create.c */

/* 
  Opens/creates the database and a new table (if it doesn't exist yet)
  Returns  sqlite3 error codes - in event of errors
  Returns  0 - success
*/
int
lib_sqlite3_create_table (char *name_db, char *name_table, char *sql_statement);


/* lib_sqlite3_search.c */

/*
  Looks in the given db for key_value in the key column
  Returns sqlite3 error codes - in event of error
  Returns  0                  - if the element exists
  Returns -1                  - if the element doesn't exist
*/
int lib_sqlite3_search_string (char *name_db, char *name_table, char *key, char *key_value);


/* lib_sqlite3_misc.c */

/* 
  Returns a char** with the columns's names of the given table

  Returns NULL - in event of error
*/
char **lib_sqlite3_get_column_name (char *name_db, char *name_table);

/* 
  Saves in the given int variables the number of rows and columns

  Returns 0                   - success
  Returns sqlite3 error codes - in event of error
*/
int lib_sqlite3_get_nrow_and_ncol (char *name_db, char *name_table, int *nrow, int *ncol);


/* lib_sqlite3_add.c */

/*
  Inserts a new element (just one) in the db (in the specified table).
  "position" value represents the numner of the column

  Returns 0                  - success
  Returns sqlite3 error code - in event of error
*/
int lib_sqlite3_add_set_position (char *name_db, char *name_table, char *key, char *key_value, char *element_to_add, int position);

/*
  Inserts a new element (just one) in the db (in the specified table).
  "position" value represents the numner of the column.
  The new value is inserted only if the key_value doesn't exixt in the db.

  Returns 0                  - success
  Returns sqlite3 error code - in event of error
*/
int lib_sqlite3_add_set_position_check_presence (char *name_db, char *name_table, char *key, char *key_value, char *element_to_add, int position);

/*
  Inserts new elements (the full row, char**) in the db (in the specified table).

  Returns 0                  - success
  Returns sqlite3 error code - in event of error
*/
int lib_sqlite3_add_with_list (char *name_db, char *name_table, char **elements_to_add, int num_of_elem);

/*
  Inserts new elements (the full row, char**) in the db (in the table specified) only if key_value doesn't exist yet.

  Returns 0                   - success
  Returns sqlite3 error code  - in event of error
*/
int lib_sqlite3_add_with_list_check_presence (char *name_db, char *name_table, char *key, char *key_value, char **elements_to_add, int num_of_elem);

/*
  Looks for the given key_value in the database and update its fields; if it doesn't exist yet, the func inserts them in the db as a new row.

  Returns 0                   - success
  Returns sqlite3 error code  - in event of error
*/
int lib_sqlite3_add_or_update_with_list (char *name_db, char *name_table, char *key, char *key_value, char **elements_to_add, char **name_columns, int num_of_elem);


/* lib_sqlite3_read.c */

/*
 Looks for key_value in the key column (of the given table/db).
 If it exists reads the value (TEXT) of the given column (INT) and saves it in the int* given.

 Returns 0                  - success
 Returns sqlite code error  - in event of error
*/
int lib_sqlite3_read_int (char *name_db, char *name_table, char *key, char *key_value, int column, int *value);

/*
 Looks for key_value in the key column (of the given table/db).
 If it exists reads the value (TEXT) of the given column (INT) and returns it.

 Returns char* - success
 Returns NULL  - in event of error
*/
char *lib_sqlite3_read_str (char *name_db, char *name_table, char *key, char *key_value, int column);

#endif /* __LIB_SQLITE3_H__ */
