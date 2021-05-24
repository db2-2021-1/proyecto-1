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

#include <rapidjson/reader.h>

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

	class json_handler:
		public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, json_handler>
	{
	private:
		enum class state
		{
			before_table,
			in_table,
			after_columns,
			before_column,
			in_column,
			after_column_name,
			after_column_type,
			in_column_type,
			after_column_type_name,
			after_column_type_size,
			after_index,
			in_index,
			after_index_column,
			after_index_type,
			after_table
		};

		table& t;
		state s;
	public:
		json_handler(table& t):
			t(t),
			s(state::before_table)
		{};

		bool StartObject();
		bool EndObject(rapidjson::SizeType);
		bool StartArray();
		bool EndArray(rapidjson::SizeType);
		bool Key(const char* str, rapidjson::SizeType, bool);
		bool String(const char* str, rapidjson::SizeType, bool);
		bool Uint64(uint64_t n);

		bool Default();
	};

public:
	table(
		std::string table_name,
		std::vector<std::pair<std::string, statement::type>> columns = {},
		std::optional<index> table_index = std::nullopt
	);

	table();

	bool read_metadata();
	bool write_metadata() const;

	void set_index(index i);

	/// Copy data from csv.
	bool read_csv(std::string_view csv_name);

	/// Write data to the table.
	bool write_data(std::vector<std::vector<statement::literal>>& data);

	/// Check and trim data.
	bool check_data(std::vector<std::vector<statement::literal>>& data);

	/// The number of bytes in any row of the table.
	size_t tuple_size() const;
};

};
