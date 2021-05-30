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
#include <cstdlib>
#include <cstring>
#include <filesystem>

#include <readline/history.h>
#include <readline/readline.h>

#include <completion.hpp>

// https://thoughtbot.com/blog/tab-completion-in-gnu-readline

const auto history_file =
	std::filesystem::path(getenv("HOME")) / ".proyecto-1_history";

void init_readline()
{
	rl_readline_name = "proyecto-1";
	rl_attempted_completion_function = sql_completion;
}

void init_history()
{
	using_history();

	if(FILE* file = fopen(history_file.string().c_str(), "a"))
	{
		fclose(file);

		if(read_history(history_file.string().c_str()) != 0)
			perror(history_file.string().c_str());

		if(atexit(end_history) != 0)
			exit(EXIT_FAILURE);
	}
	else
	{
		perror(history_file.string().c_str());
	}
}

void end_history()
{
	if(write_history(history_file.string().c_str()) != 0)
		perror(history_file.string().c_str());
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
		"COPY",
		"CREATE",
		"CSV",
		"DELETE",
		"FROM",
		"HEADER",
		"INDEX",
		"INSERT",
		"INT",
		"INTO",
		"IS",
		"ON",
		"REAL",
		"SELECT",
		"TABLE",
		"USING",
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
