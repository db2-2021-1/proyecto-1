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

#ifdef __cplusplus
extern "C" {
#endif

enum sql_type_type
{
	SQL_INT,
	SQL_VARCHAR
};

typedef struct _sql_type
{
	int type;
	size_t size;
} sql_type;

sql_type sql_type_int();
sql_type sql_type_varchar(size_t size);

void sql_type_print(sql_type type, FILE* file);

#ifdef __cplusplus
} // extern "C"
#endif
