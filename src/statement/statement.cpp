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
#include "create_table.hpp"
#include "delete_from.hpp"
#include "insert.hpp"
#include "parser_def.h"
#include "select.hpp"
#include "statement.hpp"
#include "copy.hpp"

std::unique_ptr<db2::statement::statement>
	db2::statement::from_tree(const sql_statement_tree& tree)
{
	switch(tree.type)
	{
		case SQL_CREATE_TABLE:
			return std::unique_ptr<statement>(new(std::nothrow) create_table(tree));

		case SQL_CREATE_INDEX:
			return std::unique_ptr<statement>(new(std::nothrow) create_index(tree));

		case SQL_SELECT:
			return std::unique_ptr<statement>(new(std::nothrow) select(tree));

		case SQL_INSERT:
			return std::unique_ptr<statement>(new(std::nothrow) insert(tree));

		case SQL_DELETE_FROM:
			return std::unique_ptr<statement>(new(std::nothrow) delete_from(tree));

		case SQL_COPY:
			return std::unique_ptr<statement>(new(std::nothrow) copy(tree));

		default:
			return nullptr;
	}
}

std::unique_ptr<db2::statement::statement>
	db2::statement::from_string(std::string_view str)
{
	if(sql_statement_tree* tree = parse(str.data()))
	{
		auto statement = from_tree(*tree);

		sql_statement_tree_free(tree);
		return statement;
	}

	return nullptr;
}

std::ostream& db2::statement::operator<<(std::ostream& os, const statement& i)
{
	return i.print(os);
}
