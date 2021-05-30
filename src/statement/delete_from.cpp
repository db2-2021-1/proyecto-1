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

#include "delete_from.hpp"
#include "table.hpp"

db2::statement::delete_from::delete_from(std::string table_name, expression expr):
	statement(std::move(table_name)),
	expr(std::move(expr))
{};

db2::statement::delete_from::delete_from(const sql_statement_tree& tree):
	statement(tree.table_name),
	expr(*tree.expr)
{};

db2::statement::delete_from::delete_from(){};

std::string db2::statement::delete_from::get_name() const
{
	return "DELETE FROM";
}

bool db2::statement::delete_from::execute()
{
	// delete_from{
	//     table_name // DELETE FROM name
	//     expr       // WHERE expr
	// }

	table t(table_name);

	if(!t.read_metadata())
		return false;

	auto rows = t.get_data(expr);

	for(auto& r: rows)
		r.valid = false;

	if(!t.write_data(rows))
		return false;

	return t.update_index(rows);
}

std::ostream& db2::statement::delete_from::print(std::ostream& os) const
{
	return os << *this;
}

std::ostream& db2::statement::operator<<(std::ostream& os, const delete_from& s)
{
	return os
		<< "DELETE FROM " << s.table_name << '\n'
		<< s.expr << '\n';
}
