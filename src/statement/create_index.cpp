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

#include "create_index.hpp"

db2::statement::create_index::create_index(std::string name, index_type type, std::string column_name):
	statement(std::move(name)),
	type(type),
	column_name(column_name)
{};

db2::statement::create_index::create_index(const sql_statement_tree& tree)
{
	table_name = tree.table_name;
	switch(tree.index_type)
	{
		case SQL_HASH:
			type = index_type::e_hash;
			break;

		case SQL_BPTREE:
			type = index_type::bp_tree;
			break;
	}

	// Only use one, for now.
	column_name = tree.columns->name;
}

db2::statement::create_index::create_index(){};

bool db2::statement::create_index::execute()
{
	// create_index{
	//     table_name  // CREATE INDEX ON name
	//     type        // [ USING hash ]
	//     column_name // (a)
	// }

	// TODO
	std::cout << *this;

	return false;
}

std::ostream& db2::statement::create_index::print(std::ostream& os) const
{
	return os << *this;
}

std::ostream& db2::statement::operator<<(std::ostream& os, const create_index& c)
{
	os << "CREATE INDEX " << c.table_name << " USING ";
	switch(c.type)
	{
		case index_type::bp_tree:
			os << "B+ tree";
			break;

		case index_type::e_hash:
			os << "Hash";
			break;
	}

	os << '(' << c.column_name << ")\n";

	return os;
}
