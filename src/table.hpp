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
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <optional>

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
	std::filesystem::path data_path() const;
	std::filesystem::path index_path() const;

	/// Get the index of a column by it's name.
	ssize_t name2index(std::string_view column_name) const;

	/// Get the index of the index key in the row. Returns -1 if no found.
	ssize_t key_index() const;

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

	/// Writes data to the file. It sets the row's pos if it doesn't have one.
	void write(std::ostream& os, statement::row& r) const;

	// The buffer must be big enough to hold the biggest value.
	statement::row read(std::istream& is, char* buffer) const;

	/// Updates the hash index. It assumes that all the data was written after
	/// write_start and that all rows are valid.
	bool update_hash_index(const std::vector<statement::row>& data);

	bool update_bptree_index(const std::vector<statement::row>& data);

	bool update_isam_index(const std::vector<statement::row>& data);

	/// Must be called when the table has an index.
	std::vector<size_t> select_equals(const literal& key);

	/// Must be called when the table has an index.
	std::vector<size_t> select_range(const literal& ge, const literal& le);

	std::vector<statement::row> select_all(
		const std::optional<statement::expression>& expr = std::nullopt);

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
	bool has_index() const;

	/// Copy data from csv.
	bool read_csv(std::string_view csv_name);

	/// Write data to the end of the table.
	bool append_data(std::vector<statement::row>& data);

	/// Write data to the table
	bool write_data(std::vector<statement::row>& data);

	/// Check and trim data.
	bool check_data(std::vector<statement::row>& data);

	/// The number of bytes in any row of the table plus the valid flag.
	size_t tuple_size() const;

	/// Get data.
	std::vector<statement::row> get_data(
		const std::optional<statement::expression>& expr = std::nullopt
	);

	void print_columns(std::ostream& os) const;

	std::string get_table_index_name() const;

	statement::index_type get_index_type() const;

	/// Inserts valid rows and deletes otherwise.
	bool update_index(std::vector<statement::row>& data);

	bool reset_index();
};

};
