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

typedef struct _sql_data_list
{
	sql_literal literal;
	struct _sql_data_list* next;
} sql_data_list;

sql_data_list* sql_data_list_alloc(sql_literal literal);
void sql_data_list_free(sql_data_list* data_list);

void sql_data_list_print(sql_data_list* data_list, FILE* file);

#ifdef __cplusplus
} // extern "C"
#endif
