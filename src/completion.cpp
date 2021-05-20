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

#include <cstdio>
#include <cstring>

#include <readline/readline.h>

#include <completion.hpp>

// https://thoughtbot.com/blog/tab-completion-in-gnu-readline

void initialize_readline()
{
	rl_readline_name = "proyecto-1";
	rl_attempted_completion_function = sql_completion;
}

char** sql_completion(const char* text, int, int)
{
	return rl_completion_matches(text, key_word_generator);
}

char* key_word_generator(const char* text, int state)
{
	thread_local static int list_index, len;
	static const char* key_words[] =
	{
		"AND",
		"BETWEEN",
		"CREATE",
		"DELETE",
		"FROM",
		"INSERT",
		"INT",
		"INTO",
		"IS",
		"SELECT",
		"TABLE",
		"VALUES",
		"VARCHAR",
		"WHERE",

		nullptr
	};

	const char* name;

	if(!state)
	{
		list_index = 0;
		len = strlen(text);
	}

	while((name = key_words[list_index++]))
	{
		if(strncmp(name, text, len) == 0)
			return strdup(name);
	}

	return nullptr;
}
