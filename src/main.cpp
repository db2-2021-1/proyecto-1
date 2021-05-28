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
#include "statement.hpp"
#include "benchmark.hpp"

#define C_LBLUE  "\001\e[1;36m\002"
#define C_YELLOW "\001\e[1;33m\002"
#define C_RED    "\001\e[1;31m\002"
#define C_RESET  "\001\e[0m\002"

#define DEFAULT_PROMPT C_LBLUE "SQL" C_YELLOW ">" C_RESET " "
#define ERROR_PREFIX C_RED "!!!" C_RESET " "

int main(int argc, char* argv[])
{
	db2::args a;

	a.parse(argc, argv);

	db2::benchmark b(a.benchmark_file, a.benchmark);

	init_readline();
	init_history();

	int prompt_i = 0;
	const char* prompts[2] =
	{
		DEFAULT_PROMPT,
		ERROR_PREFIX DEFAULT_PROMPT
	};

	b.start_benchmark();
	while(char* line = readline(prompts[prompt_i]))
	{
		prompt_i = 0;
		if(strlen(line) > 0)
		{
			add_history(line);

			for(auto& statement: db2::statement::from_string(line))
			{
				b.before_transaction();
				if(!statement->execute())
					prompt_i = 1;
				b.after_transaction();
			}
		}

		free(line);
	}
	b.end_benchmark();

	exit(EXIT_SUCCESS);
}
