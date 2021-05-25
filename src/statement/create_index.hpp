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
#include <string_view>

#include "statement.hpp"

namespace db2::statement
{

enum class index_type
{
	none,

	/// Extendible hash.
	e_hash,

	/// B+ tree.
	bp_tree
};

std::string index_type2str(index_type type);
index_type str2index_type(std::string_view str);

/// CREATE INDEX ON name [ USING index_type ] (name)
class create_index: public statement
{
private:
	std::string name;
	index_type type;
	std::string column_name;

	create_index(const sql_statement_tree& tree);
	virtual std::ostream& print(std::ostream& os) const;
public:
	create_index(std::string name, index_type type, std::string column_name);
	create_index();

	virtual bool execute();
	virtual ~create_index(){};

	friend std::unique_ptr<statement> from_tree(const sql_statement_tree& tree);
	friend std::ostream& operator<<(std::ostream& os, const create_index& c);
	friend class statement;
};

std::ostream& operator<<(std::ostream& os, const create_index& i);

}
