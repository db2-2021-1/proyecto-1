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

#include <iostream>

#include "tree.h"

namespace db2::statement
{

struct type
{
	enum class _type
	{
		INT,
		VARCHAR
	};

	_type t;
	size_t size;

	type(sql_type sql_t);
	static std::string type2str(_type t);
};

std::ostream& operator<<(std::ostream& os, const type& t);

}
