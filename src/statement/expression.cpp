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

#include "expression.hpp"

db2::statement::expression::expression(std::string column, literal value):
	t(type::is),
	column(std::move(column))
{
	this->value[0] = std::move(value);
}

db2::statement::expression::expression(std::string column, literal value_ge, literal value_le):
	t(type::between),
	column(std::move(column))
{
	this->value[0] = std::move(value_ge);
	this->value[1] = std::move(value_le);
}

std::ostream& db2::statement::operator<<(std::ostream& os, const expression& l)
{
	os << l.column << ' ';
	switch(l.t)
	{
		case expression::type::is:
			os << "IS " << l.value[0];
			break;

		case expression::type::between:
			os << "BETWEEN " << l.value[0] << " AND " << l.value[1];
			break;
	}
	return os << '\n';
}
