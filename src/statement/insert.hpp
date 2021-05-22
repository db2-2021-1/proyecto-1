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

#include <vector>

#include "statement.hpp"
#include "literal.hpp"

namespace db2::statement
{

class insert: public statement
{
private:
	std::vector<std::vector<literal>> data;

public:
	insert(std::string table_name, std::vector<std::vector<literal>> data);
	insert();

	virtual bool execute();
	virtual ~insert(){};
};

}
