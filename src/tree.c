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
		tree->type = SQL_INVALID;
	}

	return tree;
};

sql_statement_tree* sql_create()
{
	sql_statement_tree* tree = sql_statement_tree_alloc();

	tree->type = SQL_CREATE;

	return tree;
}

sql_statement_tree* sql_select()
{
	sql_statement_tree* tree = sql_statement_tree_alloc();

	tree->type = SQL_SELECT;

	return tree;
}

sql_statement_tree* sql_insert()
{
	sql_statement_tree* tree = sql_statement_tree_alloc();

	tree->type = SQL_INSERT;

	return tree;
}

sql_statement_tree* sql_delete()
{
	sql_statement_tree* tree = sql_statement_tree_alloc();

	tree->type = SQL_DELETE;

	return tree;
}

void sql_statement_tree_free(sql_statement_tree* tree)
{
	free(tree);
}
