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

#ifdef __cplusplus
extern "C" {
#endif

enum statement_type
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
} sql_statement_tree;

sql_statement_tree* sql_statement_tree_alloc();

sql_statement_tree* sql_create();
sql_statement_tree* sql_select();
sql_statement_tree* sql_insert();
sql_statement_tree* sql_delete();

void sql_statement_tree_free(sql_statement_tree* tree);

#ifdef __cplusplus
} // extern "C"
#endif
