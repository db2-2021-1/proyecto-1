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

#include "literal.h"

#ifdef __cplusplus
extern "C" {
#endif

enum sql_expr_type
{
	SQL_EXPR_INVALID,

	SQL_EXPR_IS,
	SQL_EXPR_BETWEEN
};

typedef struct _sql_expr
{
	int type;
	char* column_name;
	sql_literal literals[2];
} sql_expr;

sql_expr* sql_expr_alloc();

sql_expr* sql_expr_is(char* column_name, sql_literal l);
sql_expr* sql_expr_between(char* column_name, sql_literal l_l, sql_literal l_r);

void sql_expr_free(sql_expr* expr);

void sql_expr_print(sql_expr* expr, FILE* file);

#ifdef __cplusplus
} // extern "C"
#endif
