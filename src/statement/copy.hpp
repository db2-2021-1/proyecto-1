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
#include <optional>
#include <vector>

#include "expression.hpp"
#include "statement.hpp"

namespace db2::statement
{

/// COPY name FROM csv_name CSV HEADER
class copy: public statement
{
private:
	std::string csv_name;

	copy(const sql_statement_tree& tree);
	virtual std::ostream& print(std::ostream& os) const;
public:
	copy(std::string table_name, std::string csv_name);
	copy();

	virtual bool execute();
	virtual ~copy(){};

	friend std::vector<std::unique_ptr<statement>> from_tree(const sql_statement_tree* tree);
	friend std::ostream& operator<<(std::ostream& os, const copy& c);
	friend class statement;
};

std::ostream& operator<<(std::ostream& os, const copy& c);

}
