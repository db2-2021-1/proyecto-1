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

#include "tree.h"

sql_statement_tree* sql_statement_tree_alloc()
{
	sql_statement_tree* tree =
		(sql_statement_tree*)malloc(sizeof(sql_statement_tree));

	if(!tree)
	{
		tree->type       = SQL_INVALID;
		tree->table_name = NULL;
		tree->expr       = NULL;
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

sql_statement_tree* sql_select(char* table_name)
{
	sql_statement_tree* tree = sql_statement_tree_alloc();

	tree->type       = SQL_SELECT;
	tree->table_name = table_name;

	return tree;
}

sql_statement_tree* sql_insert(char* table_name)
{
	sql_statement_tree* tree = sql_statement_tree_alloc();

	tree->type       = SQL_INSERT;
	tree->table_name = table_name;

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
		free(tree->table_name);
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
				break;

			case SQL_SELECT:
				fprintf(file, "SELECT %s\n", tree->table_name);
				break;
		}
	}
}

sql_expr* sql_expr_alloc()
{
	sql_expr* expr = (sql_expr*)malloc(sizeof(sql_expr));

	if(expr)
	{
		expr->type        = SQL_EXPR_INVALID;
		expr->column_name = NULL;
	}

	return expr;
}

void sql_expr_free(sql_expr* expr)
{
	if(expr)
	{
		free(expr->column_name);
		sql_literal_free(expr->literals[0]);
		sql_literal_free(expr->literals[1]);
	}

	free(expr);
}

sql_expr* sql_expr_is(char* column_name, sql_literal l)
{
	sql_expr* expr = sql_expr_alloc();

	if(expr)
	{
		expr->type        = SQL_EXPR_IS;
		expr->column_name = column_name;
		expr->literals[0] = l;
	}

	return expr;
}

sql_expr* sql_expr_between(char* column_name, sql_literal l_l, sql_literal l_r)
{
	sql_expr* expr = sql_expr_alloc();

	if(expr)
	{
		expr->type        = SQL_EXPR_BETWEEN;
		expr->column_name = column_name;
		expr->literals[0] = l_l;
		expr->literals[1] = l_r;
	}

	return expr;
}

void sql_expr_print(sql_expr* expr, FILE* file)
{
	if(expr)
	{
		switch(expr->type)
		{
			case SQL_EXPR_IS:
				fprintf(file, "%s IS ", expr->column_name);
				sql_literal_print(expr->literals[0], file);
				fprintf(file, "%s", "\n");
				break;

			case SQL_EXPR_BETWEEN:
				fprintf(file, "%s BETWEEN ", expr->column_name);
				sql_literal_print(expr->literals[0], file);
				fprintf(file, "%s", " AND ");
				sql_literal_print(expr->literals[1], file);
				fprintf(file, "%s", "\n");
				break;
		}
	}
}

sql_literal sql_literal_number(int number)
{
	sql_literal literal =
	{
		.type         = SQL_NUMBER,
		.value.number = number
	};

	return literal;
}

sql_literal sql_literal_string(char* str)
{
	sql_literal literal =
	{
		.type         = SQL_STRING,
		.value.str    = str
	};

	return literal;
}

void sql_literal_free(sql_literal literal)
{
	if(literal.type == SQL_STRING)
	{
		free(literal.value.str);
		literal.value.str = NULL;
	}
}

void sql_literal_print(sql_literal literal, FILE* file)
{
	switch(literal.type)
	{
		case SQL_NUMBER:
			fprintf(file, "%d", literal.value.number);
			break;

		case SQL_STRING:
			fprintf(file, "%s", literal.value.str);
			break;
	}
}
