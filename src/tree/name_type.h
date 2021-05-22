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

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _sql_name_type
{
	char* name;
	sql_type type;
} sql_name_type;

/// Only frees the name pointer not the name_type.
void sql_name_type_free(sql_name_type name_type);
void sql_name_type_print(sql_name_type name_type, FILE* file);

#ifdef __cplusplus
} // extern "C"
#endif
