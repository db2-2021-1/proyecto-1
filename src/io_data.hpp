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

#pragma once

#include <cstdlib>

#include "statement.hpp"

namespace db2
{

struct io_data
{
	size_t rchar = 0;
	size_t wchar = 0;
	size_t syscr = 0;
	size_t syscw = 0;
	size_t read_bytes = 0;
	size_t write_bytes = 0;
	size_t cancelled_write_bytes = 0;

	void read();

	io_data operator-(const io_data& other) const;

private:
	void read_line(char* line);
};

};
