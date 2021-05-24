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

#include "literal.hpp"

db2::statement::literal db2::statement::from_union(sql_literal l)
{
	switch(l.type)
	{
		case SQL_INT:
			return l.value.number;

		case SQL_STRING:
			return l.value.str;

		default:
			return {};
	}
}

std::ostream& db2::statement::operator<<(std::ostream& os, const literal& l)
{
	std::visit(overload{
		[&os](int number)
		{
			os << number;
		},
		[&os](const std::string& str)
		{
			os << str;
		},
	}, l);

	return os;
}
