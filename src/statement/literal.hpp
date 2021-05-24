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

#include <string>
#include <variant>
#include <functional>

#include "overload.hpp"
#include "tree.h"

namespace db2::statement
{

using literal = std::variant<int, std::string>;

literal from_union(sql_literal l);

std::ostream& operator<<(std::ostream& os, const literal& l);

}

namespace std
{

template<> struct hash<db2::statement::literal>
{
	size_t operator()(const db2::statement::literal& l)
	{
		size_t h;

		std::visit(db2::overload{
			[&h](int number)
			{
				h = std::hash<int>{}(number);
			},
			[&h](const std::string& str)
			{
				h = std::hash<std::string>{}(str);
			},
		}, l);

		return h;
	}
};

}
