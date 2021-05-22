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

#include "literal.h"

sql_literal sql_literal_number(int number)
{
	sql_literal literal =
	{
		.type         = SQL_NUMBER,
		.value.number = number
	};

	return literal;
}

sql_literal sql_literal_string(char* str)
{
	sql_literal literal =
	{
		.type         = SQL_STRING,
		.value.str    = str
	};

	return literal;
}

void sql_literal_free(sql_literal literal)
{
	if(literal.type == SQL_STRING)
	{
		free(literal.value.str);
		literal.value.str = NULL;
	}
}

void sql_literal_print(sql_literal literal, FILE* file)
{
	switch(literal.type)
	{
		case SQL_NUMBER:
			fprintf(file, "%d", literal.value.number);
			break;

		case SQL_STRING:
			fprintf(file, "%s", literal.value.str);
			break;
	}
}
