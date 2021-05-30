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
#include <iostream>

#include "statement.hpp"
#include "type.hpp"

namespace db2::statement
{

/// CREATE TABLE name(name type, name type)
class create_table: public statement
{
private:
	std::vector<std::pair<std::string, type>> data_types;

	create_table(const sql_statement_tree& tree);
	virtual std::ostream& print(std::ostream& os) const;
public:
	create_table(std::string table_name, std::vector<std::pair<std::string, type>> data_types);
	create_table();

	virtual std::string get_name() const;
	virtual bool execute();
	virtual ~create_table(){};

	friend std::vector<std::unique_ptr<statement>> from_tree(const sql_statement_tree* tree);
	friend std::ostream& operator<<(std::ostream& os, const create_table& c);
	friend class statement;
};

std::ostream& operator<<(std::ostream& os, const create_table& c);

}
