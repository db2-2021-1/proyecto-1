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

#include <cstdio>
#include <filesystem>
#include <optional>
#include <string>
#include <utility>

#include "statement.hpp"

namespace db2
{

/// This class manages the operations on a table.
class table
{
public:
	struct index
	{
		std::string column_name;
		statement::index_type type;
	};
private:
	std::string table_name;
	std::vector<std::pair<std::string, statement::type>> columns;
	std::optional<index> table_index;

	bool check_and_create_directory() const;
	std::filesystem::path metadata_path() const;

public:
	table(
		std::string table_name,
		std::vector<std::pair<std::string, statement::type>> columns,
		std::optional<index> table_index = std::nullopt
	);

	table();

	bool read_metadata();
	bool write_metadata() const;
};

};
