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

#include <iostream>

#include <readline/history.h>
#include <readline/readline.h>

#include "args.hpp"
#include "completion.hpp"
#include "parser_def.h"

int main(int argc, char* argv[])
{
	db2::args a;

	a.parse(argc, argv);

	init_readline();
	init_history();

	while(char* line = readline("SQL> "))
	{
		if(strlen(line) > 0)
		{
			add_history(line);
			sql_statement_tree* tree = parse(line);
			if(tree)
			{
				switch(tree->type)
				{
					case SQL_CREATE:
						puts("CREATE");
						break;

					case SQL_DELETE:
						puts("DELETE");
						break;

					case SQL_INSERT:
						puts("INSERT");
						break;

					case SQL_SELECT:
						puts("SELECT");
						break;
				}
			}
			sql_statement_tree_free(tree);
		}

		free(line);
	}

	exit(EXIT_SUCCESS);
}
