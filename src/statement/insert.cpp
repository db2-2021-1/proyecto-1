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
#include "row.hpp"
#include "table.hpp"

db2::statement::insert::insert(std::string table_name, std::vector<row> data):
	statement(std::move(table_name)),
	data(std::move(data))
{};

db2::statement::insert::insert(const sql_statement_tree& tree)
{
	table_name = tree.table_name;

	for(auto* old_row = tree.insert_values; old_row != nullptr; old_row = old_row->next)
	{
		row new_row;
		for(auto* column = old_row->data_list; column != nullptr; column = column->next)
		{
			new_row.values.push_back(from_union(column->literal));
		}

		data.push_back(std::move(new_row));
	}
}

db2::statement::insert::insert(){};

std::string db2::statement::insert::get_name() const
{
	return "INSERT INTO";
}

bool db2::statement::insert::execute()
{
	// insert{
	//     table_name // INSERT INTO name VALUES
	//     data       // (1,2,"a"), (2,3,"b")
	// }

	table t(table_name);

	if(!t.read_metadata())
		return false;

	if(!t.append_data(data))
		return false;

	return t.update_index(data);
}

std::ostream& db2::statement::insert::print(std::ostream& os) const
{
	return os << *this;
}

std::ostream& db2::statement::operator<<(std::ostream& os, const insert& i)
{
	os << "INSERT " << i.table_name << '\n';

	for(const auto& row: i.data)
	{
		std::cout << "(";
		for(const auto& data: row.values)
		{
			std::cout << data << ", ";
		}
		std::cout << ")\n";
	}

	return os;
}
