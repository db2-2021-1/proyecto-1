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

#include "copy.hpp"
#include "table.hpp"

db2::statement::copy::copy(std::string table_name, std::string csv_name):
	statement(std::move(table_name)),
	csv_name(std::move(csv_name))
{};

db2::statement::copy::copy(const sql_statement_tree& tree)
{
	table_name = tree.table_name;
	csv_name   = tree.csv_name;
}

db2::statement::copy::copy(){};

bool db2::statement::copy::execute()
{
	// copy{
	//     table_name // COPY name
	//     csv_name   // FROM csv_name CSV HEADER
	// }

	table t(table_name);

	if(!t.read_metadata())
		return false;

	// read_csv() updates the index.
	return t.read_csv(csv_name);
}

std::ostream& db2::statement::copy::print(std::ostream& os) const
{
	return os << *this;
}

std::ostream& db2::statement::operator<<(std::ostream& os, const copy& c)
{
	return os << "COPY " << c.table_name << " FROM " << c.csv_name << '\n';
}
