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

#include "columns.h"

sql_columns* sql_columns_alloc(char* _name)
{
	sql_columns* columns = (sql_columns*)malloc(sizeof(sql_columns));

	if(columns)
	{
		columns->name = _name;
		columns->next = NULL;
	}

	return columns;
}

void sql_columns_free(sql_columns* columns)
{
	if(columns)
	{
		free(columns->name);
		sql_columns_free(columns->next);
	}
	free(columns);
}

void sql_columns_print(sql_columns* columns, FILE* file)
{
	if(columns)
	{
		fprintf(file, "%s%s", columns->name, columns->next ? ", " : "\n");
		sql_columns_print(columns->next, file);
	}
}
