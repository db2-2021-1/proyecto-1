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

db2::literal db2::from_union(sql_literal l)
{
	switch(l.type)
	{
		case SQL_NUMBER:
			return l.value.number;

		case SQL_REAL_NUMBER:
			return l.value.real_number;

		case SQL_STRING:
			return l.value.str;

		default:
			return {};
	}
}

std::ostream& std::operator<<(std::ostream& os, const db2::literal& l)
{
	std::visit(db2::overload{
		[&os](int number)
		{
			os << number;
		},
		[&os](float number)
		{
			os << number;
		},
		[&os](const std::string& str)
		{
			bool escaped = str.find(',') != std::string::npos;

			if(escaped)
				os << '"';
			os << str;
			if(escaped)
				os << '"';
		},
	}, l);

	return os;
}
