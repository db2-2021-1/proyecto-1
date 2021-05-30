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

#include "data_list.h"

sql_data_list* sql_data_list_alloc(sql_literal literal)
{
	sql_data_list* data_list = (sql_data_list*)malloc(sizeof(sql_data_list));

	if(data_list)
	{
		data_list->literal = literal;
		data_list->next    = NULL;
	}

	return data_list;
}

void sql_data_list_free(sql_data_list* data_list)
{
	if(data_list)
	{
		sql_literal_free(data_list->literal);
		sql_data_list_free(data_list->next);
	}
	free(data_list);
}

void sql_data_list_print(sql_data_list* data_list, FILE* file)
{
	if(data_list)
	{
		sql_literal_print(data_list->literal, file);
		fprintf(file, "%s", data_list->next ? ", " : "");
		sql_data_list_print(data_list->next, file);
	}
}
