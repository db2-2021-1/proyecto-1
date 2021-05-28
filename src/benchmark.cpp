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

#include <unistd.h>
#include <iostream>

#include <rapidjson/error/en.h>

#include "benchmark.hpp"

db2::benchmark::benchmark(std::filesystem::path file, bool _enabled):
	enabled(_enabled),
	output_file([](std::filesystem::path& file, bool enabled) -> FILE*
	{
		FILE* new_file = nullptr;

		if(enabled)
		{
			if(file.empty())
			{
				if(!(new_file = fdopen(dup(fileno(stderr)), "a")))
				{
					perror("stderr");
				}
			}
			else
			{
				if(!(new_file = fopen(file.c_str(), "a")))
				{
					perror(file.c_str());
				}
			}
		}
		else
		{
			return fopen("/dev/null", "w");
		}

		return new_file;
	}(file, enabled)),
	fws(output_file, buffer, sizeof(buffer)),
	writer(fws)
{}

bool db2::benchmark::is_open() const
{
	return output_file;
}

db2::benchmark::~benchmark()
{
	if(output_file)
	{
		if(fclose(output_file) != 0)
			perror("");
		output_file = nullptr;
	}
}

void db2::benchmark::start_benchmark()
{
	if(!is_open())
		return;

	writer.StartArray();
}

void db2::benchmark::end_benchmark()
{
	if(!is_open())
		return;

	writer.EndArray();
}

void db2::benchmark::before_transaction()
{
	if(!is_open())
		return;

	// TODO
	std::cerr << "Before\n";
}

void db2::benchmark::after_transaction()
{
	if(!is_open())
		return;

	// TODO
	std::cerr << "After\n";
}
