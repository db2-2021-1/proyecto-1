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

#include "create_table.hpp"

db2::statement::create_table::create_table(std::string table_name, std::vector<std::pair<std::string, type>> data_types):
	statement(std::move(table_name)),
	data_types(std::move(data_types))
{};

db2::statement::create_table::create_table(const sql_statement_tree& tree)
{
	table_name = tree.table_name;

	for(auto* dt = tree.new_columns; dt != nullptr; dt = dt->next)
	{
		data_types.emplace_back(dt->name_type.name, dt->name_type.type);
	}
}

db2::statement::create_table::create_table(){};

bool db2::statement::create_table::execute()
{
	// TODO
	std::cout << *this;
	return false;
}

std::ostream& db2::statement::create_table::print(std::ostream& os) const
{
	return os << *this;
}

std::ostream& db2::statement::operator<<(std::ostream& os, const create_table& c)
{
	os << "CREATE TABLE " << c.table_name << '\n';

	for(const auto& data_type: c.data_types)
	{
		os << data_type.first << " " << data_type.second << ",\n";
	}

	return os;
}
