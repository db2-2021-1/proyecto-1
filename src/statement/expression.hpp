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

#include "literal.hpp"
#include "tree.h"

namespace db2::statement
{

struct expression
{
	enum class type
	{
		// name IS literal
		is,

		// name BETWEEN literal AND literal
		between
	};

	type t;

	std::string column;
	literal value[2];

	expression(std::string column, literal value);
	expression(std::string column, literal value_ge, literal value_le);
	expression(const sql_expr& expr);
	expression();
};

std::ostream& operator<<(std::ostream& os, const expression& l);

}
