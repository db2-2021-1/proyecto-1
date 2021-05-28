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
#include <unordered_map>

#include "io_data.hpp"

void db2::io_data::read()
{
	if(FILE* file = fopen("/proc/self/io", "r"))
	{
		ssize_t line_size;
		size_t buffer_size;

		char* line = nullptr;

		while((line_size = getline(&line, &buffer_size, file)) != -1)
		{
			read_line(line);
		}

		free(line);
		fclose(file);
	}
}

void db2::io_data::read_line(char* line)
{
	enum class member
	{
		rchar,
		wchar,
		syscr,
		syscw,
		read_bytes,
		write_bytes,
		cancelled_write_bytes
	};

	static const char delim[] = " :\n";
	static const std::unordered_map<std::string_view, member> parse_map
	{
		{"rchar",                 member::rchar},
		{"wchar",                 member::wchar},
		{"syscr",                 member::syscr},
		{"syscw",                 member::syscw},
		{"read_bytes",            member::read_bytes},
		{"write_bytes",           member::write_bytes},
		{"cancelled_write_bytes", member::cancelled_write_bytes},
	};

	char* buffer;
	for(char* token = strtok_r(line, delim, &buffer);
		token;
		token = strtok_r(nullptr, delim, &buffer)
	)
	{
		auto it = parse_map.find(token);
		if(it == parse_map.end())
			continue;

		size_t number;
		sscanf(token, "%zu", &number);

		switch(it->second)
		{
			case member::rchar:
				rchar = number;
				break;

			case member::wchar:
				wchar = number;
				break;

			case member::syscr:
				syscr = number;
				break;

			case member::syscw:
				syscw = number;
				break;

			case member::read_bytes:
				read_bytes = number;
				break;

			case member::write_bytes:
				write_bytes = number;
				break;

			case member::cancelled_write_bytes:
				cancelled_write_bytes = number;
				break;
		}
	}
}

db2::io_data db2::io_data::operator-(const io_data& other) const
{
	return
	{
		rchar                 - other.rchar,
		wchar                 - other.wchar,
		syscr                 - other.syscr,
		syscw                 - other.syscw,
		read_bytes            - other.read_bytes,
		write_bytes           - other.write_bytes,
		cancelled_write_bytes - other.cancelled_write_bytes
	};
}
