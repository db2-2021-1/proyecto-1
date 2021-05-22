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

#ifdef __cplusplus
extern "C" {
#endif

enum sql_statement_type
{
	SQL_INVALID,

	SQL_CREATE,
	SQL_SELECT,
	SQL_INSERT,
	SQL_DELETE
};

typedef struct _sql_statement_tree
{
	int type;
	sql_columns* columns;
	char* table_name;
	sql_expr* expr;
} sql_statement_tree;

sql_statement_tree* sql_statement_tree_alloc();

/// Creates a CREATE TABLE tree.
/// table_name must be allocated with malloc, and its ownership its
/// transfered to the tree.
sql_statement_tree* sql_create(char* table_name);

/// Creates a SELECT tree.
/// table_name must be allocated with malloc, and its ownership its
/// transfered to the tree.
sql_statement_tree* sql_select(sql_columns* columns, char* table_name);
sql_statement_tree* sql_select_where(sql_columns* columns, char* table_name, sql_expr* expr);

/// Creates a INSERT INTO tree.
/// table_name must be allocated with malloc, and its ownership its
/// transfered to the tree.
sql_statement_tree* sql_insert(char* table_name);

/// Creates a DELETE FROM tree.
/// table_name must be allocated with malloc, and its ownership its
/// transfered to the tree.
sql_statement_tree* sql_delete(char* table_name, sql_expr* expr);

void sql_statement_tree_free(sql_statement_tree* tree);

void sql_statement_tree_print(sql_statement_tree* tree, FILE* file);

#ifdef __cplusplus
} // extern "C"
#endif
