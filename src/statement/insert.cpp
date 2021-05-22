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

#include "insert.hpp"

db2::statement::insert::insert(std::string table_name, std::vector<std::vector<literal>> data):
	statement(std::move(table_name)),
	data(std::move(data))
{};

db2::statement::insert::insert(const sql_statement_tree& tree)
{
	table_name = tree.table_name;

	for(auto* row = tree.insert_values; row != nullptr; row = row->next)
	{
		std::vector<literal> new_column;
		for(auto* column = row->data_list; column != nullptr; column = column->next)
		{
			new_column.push_back(from_union(column->literal));
		}

		data.push_back(std::move(new_column));
	}
}

db2::statement::insert::insert(){};

bool db2::statement::insert::execute()
{
	return false;
}

std::ostream& db2::statement::insert::print(std::ostream& os) const
{
	return os << *this;
}

std::ostream& db2::statement::operator<<(std::ostream& os, const insert& i)
{
	os << "INSERT " << i.table_name << '\n';

	return os;
}
