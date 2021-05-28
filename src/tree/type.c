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

#include "type.h"

sql_type sql_type_int()
{
	sql_type type =
	{
		.type = SQL_INT,
		.size = sizeof(int)
	};

	return type;
}

sql_type sql_type_real()
{
	sql_type type =
	{
		.type = SQL_REAL,
		.size = sizeof(float)
	};

	return type;
}

sql_type sql_type_varchar(size_t size)
{
	sql_type type =
	{
		.type = SQL_VARCHAR,
		.size = size
	};

	return type;
}

void sql_type_print(sql_type type, FILE* file)
{
	switch(type.type)
	{
		case SQL_INT:
			fprintf(file, "%s", "INT");
			break;

		case SQL_REAL:
			fprintf(file, "%s", "REAL");
			break;

		case SQL_VARCHAR:
			fprintf(file, "%s(%ld)", "VARCHAR", type.size);
			break;
	}
}
