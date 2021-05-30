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

#include "insert_values.h"

sql_insert_values* sql_insert_values_alloc(sql_data_list* data_list)
{
	sql_insert_values* insert_values = (sql_insert_values*)malloc(sizeof(sql_insert_values));

	if(insert_values)
	{
		insert_values->data_list = data_list;
		insert_values->next      = NULL;
	}

	return insert_values;
}

void sql_insert_values_free(sql_insert_values* insert_values)
{
	if(insert_values)
	{
		sql_data_list_free(insert_values->data_list);
		sql_insert_values_free(insert_values->next);
	}
	free(insert_values);
}

void sql_insert_values_print(sql_insert_values* insert_values, FILE* file)
{
	if(insert_values)
	{
		fprintf(file, "%s", "(");
		sql_data_list_print(insert_values->data_list, file);
		fprintf(file, ")%s\n", insert_values->next ? "," : "");
		sql_insert_values_print(insert_values->next, file);
	}
}
