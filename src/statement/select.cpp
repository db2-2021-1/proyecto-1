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

#include <vector>

#include "select.hpp"
#include "table.hpp"

db2::statement::select::select(std::string table_name, std::vector<std::string> columns, std::optional<expression> expr):
	statement(std::move(table_name)),
	columns(std::move(columns)),
	expr(std::move(expr))
{};

db2::statement::select::select(const sql_statement_tree& tree)
{
	table_name = tree.table_name;

	for(auto* c = tree.columns; c != nullptr; c = c->next)
	{
		columns.emplace_back(c->name);
	}

	if(tree.expr)
		expr = expression(*(tree.expr));
}

db2::statement::select::select(){};

bool db2::statement::select::execute()
{
	// select{
	//     columns    // SELECT a,b,c
	//     table_name // FROM name
	//     expr?      // [ WHERE expr ]
	// }

	// TODO
	table t(table_name);

	if(!t.read_metadata())
		return false;

	// Only * is supported, by now.
	t.print_columns(std::cout);

	auto get_data = [&]() -> std::vector<row>
	{
		if(!expr.has_value())
			return t.select_all();

		switch(expr->t)
		{
			case expression::type::between:
				return t.select_range(expr->value[0], expr->value[1]);

			case expression::type::is:
				return t.select_equals(expr->value[0]);

			default:
				return {};
		}
	};

	for(const auto& r: get_data())
	{
		for(const auto& cell: r.values)
		{
			std::cout << cell << (cell == *r.values.rbegin() ? '\n' : ',');
		}
	}

	return true;
}

std::ostream& db2::statement::select::print(std::ostream& os) const
{
	return os << *this;
}

std::ostream& db2::statement::operator<<(std::ostream& os, const select& s)
{
	os << "SELECT " << s.table_name << '\n';

	for(const auto& c: s.columns)
	{
		os << c << ", ";
	}
	os << '\n';

	if(s.expr.has_value())
		os << s.expr.value() << '\n';

	return os;
}
