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

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "tree.h"

namespace db2::statement
{

class statement
{
protected:
	std::string table_name;

	statement(std::string table_name):
		table_name(std::move(table_name))
	{};

	statement(){};

	virtual std::ostream& print(std::ostream&) const = 0;
public:
	virtual std::string get_name() const = 0;
	virtual bool execute() = 0;
	virtual ~statement(){};

	friend std::ostream& operator<<(std::ostream& os, const statement& i);
};

std::vector<std::unique_ptr<statement>> from_tree(const sql_statement_tree* tree);

/// Parses a SQL string to an abstract syntax tree. Then from_tree()
/// is called.
std::vector<std::unique_ptr<statement>> from_string(std::string_view str);
std::vector<std::unique_ptr<statement>> from_file(std::string_view file);

std::ostream& operator<<(std::ostream& os, const statement& i);
}
