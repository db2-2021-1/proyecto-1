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

#include <unordered_map>

#include "type.hpp"

db2::statement::type::type(sql_type sql_t)
{
	switch(sql_t.type)
	{
		case SQL_INT:
			t = _type::INT;
			break;

		case SQL_VARCHAR:
			t = _type::VARCHAR;
			break;

		default:
			t = _type::NONE;
			break;
	}

	size = sql_t.size;
}

db2::statement::type::type():
	t(_type::NONE),
	size(0)
{};

std::string db2::statement::type::type2str(_type t)
{
	switch(t)
	{
		case _type::INT:
			return "INT";

		case _type::VARCHAR:
			return "VARCHAR";

		default:
			return "";
	}
}

db2::statement::type::_type db2::statement::type::str2type(std::string_view str)
{
	static const std::unordered_map<std::string_view, _type> str2type_map
	{
		{"INT",     _type::INT},
		{"VARCHAR", _type::VARCHAR},
	};

	auto it = str2type_map.find(str);

	if(it == str2type_map.end())
	{
		return _type::NONE;
	}
	else
	{
		return it->second;
	}
}

std::ostream& db2::statement::operator<<(std::ostream& os, const type& t)
{
	switch(t.t)
	{
		case type::_type::INT:
			os << "INT";
			break;

		case type::_type::VARCHAR:
			os << "VARCHAR (" << t.size << ")";
			break;

		default:
			break;
	}

	return os;
}
