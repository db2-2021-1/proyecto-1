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

#include "new_columns.h"

sql_new_columns* sql_new_columns_alloc(sql_name_type name_type)
{
	sql_new_columns* new_columns = (sql_new_columns*)malloc(sizeof(sql_new_columns));

	if(new_columns)
	{
		new_columns->name_type = name_type;
		new_columns->next    = NULL;
	}

	return new_columns;
}

void sql_new_columns_free(sql_new_columns* new_columns)
{
	if(new_columns)
	{
		sql_name_type_free(new_columns->name_type);
		sql_new_columns_free(new_columns->next);
	}
	free(new_columns);
}

void sql_new_columns_print(sql_new_columns* new_columns, FILE* file)
{
	if(new_columns)
	{
		sql_name_type_print(new_columns->name_type, file);
		fprintf(file, "%s\n", new_columns->next ? "," : "");
		sql_new_columns_print(new_columns->next, file);
	}
}
