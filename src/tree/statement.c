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

#include <stdlib.h>

#include "statement.h"

sql_statement_tree* sql_statement_tree_alloc()
{
	sql_statement_tree* tree =
		(sql_statement_tree*)malloc(sizeof(sql_statement_tree));

	if(!tree)
	{
		tree->type          = SQL_INVALID;
		tree->columns       = NULL;
		tree->table_name    = NULL;
		tree->insert_values = NULL;
		tree->expr          = NULL;
	}

	return tree;
};

sql_statement_tree* sql_create(char* table_name)
{
	sql_statement_tree* tree = sql_statement_tree_alloc();

	tree->type       = SQL_CREATE;
	tree->table_name = table_name;

	return tree;
}

sql_statement_tree* sql_select(sql_columns* columns, char* table_name)
{
	sql_statement_tree* tree = sql_statement_tree_alloc();

	tree->type       = SQL_SELECT;
	tree->columns    = columns;
	tree->table_name = table_name;

	return tree;
}

sql_statement_tree* sql_select_where(sql_columns* columns, char* table_name, sql_expr* expr)
{
	sql_statement_tree* tree = sql_select(columns, table_name);

	tree->expr = expr;

	return tree;
}

sql_statement_tree* sql_insert(char* table_name, sql_insert_values* insert_values)
{
	sql_statement_tree* tree = sql_statement_tree_alloc();

	tree->type          = SQL_INSERT;
	tree->table_name    = table_name;
	tree->insert_values = insert_values;

	return tree;
}

sql_statement_tree* sql_delete(char* table_name, sql_expr* expr)
{
	sql_statement_tree* tree = sql_statement_tree_alloc();

	tree->type       = SQL_DELETE;
	tree->table_name = table_name;
	tree->expr       = expr;

	return tree;
}

void sql_statement_tree_free(sql_statement_tree* tree)
{
	if(tree)
	{
		sql_columns_free(tree->columns);
		free(tree->table_name);
		sql_insert_values_free(tree->insert_values);
		sql_expr_free(tree->expr);
	}

	free(tree);
}

void sql_statement_tree_print(sql_statement_tree* tree, FILE* file)
{
	if(tree)
	{
		switch(tree->type)
		{
			case SQL_CREATE:
				fprintf(file, "CREATE %s\n", tree->table_name);
				break;

			case SQL_DELETE:
				fprintf(file, "DELETE %s\n", tree->table_name);
				sql_expr_print(tree->expr, file);
				break;

			case SQL_INSERT:
				fprintf(file, "INSERT %s\n", tree->table_name);
				sql_insert_values_print(tree->insert_values, file);
				break;

			case SQL_SELECT:
				fprintf(file, "SELECT %s\n", tree->table_name);
				sql_columns_print(tree->columns, file);
				sql_expr_print(tree->expr, file);
				break;
		}
	}
}
