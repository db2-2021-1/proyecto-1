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
#include "table.hpp"

std::string db2::statement::index_type2str(index_type type)
{
	switch(type)
	{
		case index_type::bp_tree:
			return "bp_tree";

		case index_type::e_hash:
			return "e_hash";

		case index_type::isam:
			return "isam";

		default:
			return "";
	}
}

db2::statement::index_type db2::statement::str2index_type(std::string_view str)
{
	// Add a map if there are more than two index types.
	const std::unordered_map<std::string_view, index_type> parse_map =
	{
		{"bp_tree", index_type::bp_tree},
		{"e_hash",  index_type::e_hash},
		{"isam",    index_type::isam},
	};

	auto it = parse_map.find(str);

	if(it == parse_map.end())
		return index_type::none;

	return it->second;
}

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

		case SQL_ISAM:
			type = index_type::isam;
			break;
	}

	// Only use one, for now.
	column_name = tree.columns->name;
}

db2::statement::create_index::create_index(){};

std::string db2::statement::create_index::get_name() const
{
	return "CREATE INDEX";
}

bool db2::statement::create_index::execute()
{
	// create_index{
	//     table_name  // CREATE INDEX ON name
	//     type        // [ USING hash ]
	//     column_name // (a)
	// }

	table t(table_name);

	if(!t.read_metadata())
		return false;

	t.set_index({column_name, type});

	if(!t.write_metadata())
		return false;

	return t.reset_index();
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

		case index_type::isam:
			os << "Isam";
			break;

		case index_type::none:
			os << "None";
			break;
	}

	os << '(' << c.column_name << ")\n";

	return os;
}
