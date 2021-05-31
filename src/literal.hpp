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

namespace db2
{

namespace statement
{
	struct type;
};

using literal = std::variant<int, float, std::string>;

literal from_union(sql_literal l);

void write(const literal& l, size_t size, std::ostream& os);
literal read(char* buffer, const statement::type& t);

}

namespace std
{

/// Csv write
std::ostream& operator<<(std::ostream& os, const db2::literal& l);

inline bool operator<(const db2::literal& l, int i)
{
	return l < db2::literal(i);
}

inline bool operator<(const db2::literal& l, const std::string& str)
{
	return l < db2::literal(str);
}

inline bool operator>(const db2::literal& l, int i)
{
	return l > db2::literal(i);
}

inline bool operator>(const db2::literal& l, const std::string& str)
{
	return l > db2::literal(str);
}

inline bool operator>=(const db2::literal& l, int i)
{
	return !(l < i);
}

inline bool operator>=(const db2::literal& l, const std::string& str)
{
	return !(l < str);
}

inline bool operator<=(const db2::literal& l, int i)
{
	return !(l > i);
}

inline bool operator<=(const db2::literal& l, const std::string& str)
{
	return !(l > str);
}

inline bool operator==(const db2::literal& l, const std::string& str)
{
	return l == db2::literal(str);
}

inline bool operator!=(const db2::literal& l, const std::string& str)
{
	return !(l == str);
}

}
