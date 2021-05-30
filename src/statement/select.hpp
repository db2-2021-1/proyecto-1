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

/// SELECT * FROM name [ WHERE expr ]
class select: public statement
{
private:
	std::vector<std::string> columns;
	std::optional<expression> expr;

	select(const sql_statement_tree& tree);
	virtual std::ostream& print(std::ostream& os) const;

	void print_row(std::ostream& os) const;
public:
	select(std::string table_name, std::vector<std::string> columns, std::optional<expression> expr = std::nullopt);
	select();

	virtual std::string get_name() const;
	virtual bool execute();
	virtual ~select(){};

	friend std::vector<std::unique_ptr<statement>> from_tree(const sql_statement_tree* tree);
	friend std::ostream& operator<<(std::ostream& os, const select& i);
	friend class statement;
};

std::ostream& operator<<(std::ostream& os, const select& s);

}
