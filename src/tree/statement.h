// Proyecto 1
// Copyright © 2021 otreblan
//
// proyecto-1 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// proyecto-1 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with proyecto-1.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <stdio.h>

#include "columns.h"
#include "expresion.h"
#include "insert_values.h"
#include "new_columns.h"

#ifdef __cplusplus
extern "C" {
#endif

enum sql_statement_type
{
	SQL_INVALID,

	SQL_CREATE_TABLE,
	SQL_CREATE_INDEX,
	SQL_SELECT,
	SQL_INSERT,
	SQL_DELETE_FROM,
	SQL_COPY
};

enum sql_index_type
{
	SQL_BPTREE,
	SQL_HASH,
	SQL_ISAM
};

typedef struct _sql_statement_tree sql_statement_tree;
struct _sql_statement_tree
{
	int type;
	int index_type;
	sql_columns* columns;
	char* table_name;
	sql_insert_values* insert_values;
	sql_expr* expr;
	sql_new_columns* new_columns;
	char* csv_name;
	sql_statement_tree* next;
};

sql_statement_tree* sql_statement_tree_alloc();

/// Creates a CREATE TABLE tree.
sql_statement_tree* sql_create_table(char* table_name, sql_new_columns* new_columns);

/// Creates a CREATE INDEX tree.
sql_statement_tree* sql_create_index(char* table_name, sql_columns* columns);

/// Creates a CREATE INDEX tree with a custom index
sql_statement_tree* sql_create_index_using(char* table_name, const char* index_type, sql_columns* columns);

/// Creates a SELECT tree.
sql_statement_tree* sql_select(sql_columns* columns, char* table_name);
sql_statement_tree* sql_select_where(sql_columns* columns, char* table_name, sql_expr* expr);

/// Creates a INSERT INTO tree.
sql_statement_tree* sql_insert(char* table_name, sql_insert_values* insert_values);

/// Creates a DELETE FROM tree.
sql_statement_tree* sql_delete(char* table_name, sql_expr* expr);

/// Creates a COPY tree.
sql_statement_tree* sql_copy(char* table_name, char* csv_name);

void sql_statement_tree_free(sql_statement_tree* tree);

void sql_statement_tree_print(sql_statement_tree* tree, FILE* file);

#ifdef __cplusplus
} // extern "C"
#endif
