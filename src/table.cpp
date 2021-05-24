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

#include <climits>
#include <cstdio>
#include <system_error>
#include <map>

#ifndef PIPE_BUF
#define PIPE_BUF 1
#endif

#include <rapidjson/error/en.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>

#include "table.hpp"

bool db2::table::json_handler::StartObject()
{
	switch(s)
	{
		case state::before_table:
			s = state::in_table;
			return true;

		case state::before_column:
			t.columns.emplace_back();
			s = state::in_column;
			return true;

		case state::after_column_type:
			s = state::in_column_type;
			return true;

		case state::after_index:
			t.table_index.emplace();
			s = state::in_index;
			return true;

		default:
			return false;
	}
}

bool db2::table::json_handler::EndObject(rapidjson::SizeType)
{
	switch(s)
	{
		case state::in_table:
			s = state::after_table;
			return true;

		case state::in_column:
			s = state::before_column;
			return true;

		case state::in_column_type:
			s = state::in_column;
			return true;

		case state::in_index:
			s = state::in_table;
			return true;

		default:
			return false;
	}
}

bool db2::table::json_handler::StartArray()
{
	switch(s)
	{
		case state::after_columns:
			s = state::before_column;
			return true;

		default:
			return false;
	}
}

bool db2::table::json_handler::EndArray(rapidjson::SizeType)
{
	switch(s)
	{
		case state::before_column:
			s = state::in_table;
			return true;

		default:
			return false;
	}
}

bool db2::table::json_handler::Key(const char* str, rapidjson::SizeType, bool)
{
	std::map<std::pair<state, std::string_view>, state> key_map
	{
		{{state::in_table, "columns"}, state::after_columns},
		{{state::in_column, "name"}, state::after_column_name},
		{{state::in_column, "type"}, state::after_column_type},
		{{state::in_column_type, "name"}, state::after_column_type_name},
		{{state::in_column_type, "size"}, state::after_column_type_size},
		{{state::in_table, "index"}, state::after_index},
		{{state::in_index, "column"}, state::after_index_column},
		{{state::in_index, "type"}, state::after_index_type},
	};

	auto it = key_map.find({s, str});

	if(it == key_map.end())
		return false;

	s = it->second;
	return true;
}

bool db2::table::json_handler::String(const char* str, rapidjson::SizeType, bool)
{
	switch(s)
	{
		case state::after_column_name:
			t.columns.back().first = str;
			s = state::in_column;
			return true;

		case state::after_column_type_name:
			t.columns.back().second.t = statement::type::str2type(str);
			s = state::in_column_type;
			return true;

		case state::after_column_type_size:
			t.columns.back().second.size = std::stoul(str);
			s = state::in_column_type;
			return true;

		case state::after_index_column:
			if(!t.table_index.has_value())
				return false;

			t.table_index->column_name = str;
			s = state::in_index;
			return true;

		case state::after_index_type:
			if(!t.table_index.has_value())
				return false;

			t.table_index->type = statement::str2index_type(str);
			s = state::in_index;
			return true;

		default:
			return false;
	}
}

bool db2::table::json_handler::Default()
{
	return false;
}

db2::table::table(
		std::string table_name,
		std::vector<std::pair<std::string, statement::type>> columns,
		std::optional<index> table_index
	):
	table_name(std::move(table_name)),
	columns(std::move(columns)),
	table_index(std::move(table_index))
{};

db2::table::table(){};

bool db2::table::check_and_create_directory() const
{
	std::error_code e;

	if(!std::filesystem::create_directory(table_name, e) && e.value())
	{
		std::cerr << table_name << ": " << e.message() << '\n';

		return false;
	}
	return true;
}

std::filesystem::path db2::table::metadata_path() const
{
	return std::filesystem::path(table_name) / "metadata.json";
}

bool db2::table::read_metadata()
{
	FILE* file = fopen(metadata_path().string().c_str(), "r");

	if(!file)
	{
		perror(table_name.c_str());
		return false;
	}

	char buffer[PIPE_BUF];

	json_handler handler(*this);
	rapidjson::Reader reader;
	rapidjson::FileReadStream frs(file, buffer, sizeof(buffer));

	bool return_value = true;
	if(!reader.Parse<rapidjson::ParseFlag::kParseNumbersAsStringsFlag>(frs, handler))
	{
		rapidjson::ParseErrorCode e = reader.GetParseErrorCode();
		fprintf(stderr, "%s\n", rapidjson::GetParseError_En(e));
		return_value = false;
	}

	if(fclose(file) != 0)
	{
		perror(metadata_path().string().c_str());
		return false;
	}

	return return_value;
}

bool db2::table::write_metadata() const
{
	if(!check_and_create_directory())
		return false;

	FILE* file = fopen(metadata_path().string().c_str(), "w");

	if(!file)
	{
		perror(table_name.c_str());
		return false;
	}

	char buffer[PIPE_BUF];
	rapidjson::FileWriteStream fws(file, buffer, sizeof(buffer));
	rapidjson::Writer writer(fws);

	writer.StartObject();

	writer.Key("columns");
	writer.StartArray();
	for(const auto& [name, type]: columns)
	{
		writer.StartObject();

		writer.Key("name");
		writer.String(name);

		writer.Key("type");
		writer.StartObject();

		writer.Key("name");
		writer.String(statement::type::type2str(type.t));

		writer.Key("size");
		writer.Uint64(type.size);

		writer.EndObject();

		writer.EndObject();
	}
	writer.EndArray();

	if(table_index.has_value())
	{
		writer.Key("index");

		writer.StartObject();

		writer.Key("column");
		writer.String(table_index->column_name);

		writer.Key("type");
		writer.String(statement::index_type2str(table_index->type));

		writer.EndObject();
	}

	writer.EndObject();

	if(fclose(file) != 0)
	{
		perror(metadata_path().string().c_str());
		return false;
	}

	return true;
}

void db2::table::set_index(index i)
{
	table_index.emplace(std::move(i));
}

bool db2::table::read_csv(std::string_view csv_name)
{
	if(columns.empty() || !table_index.has_value())
		return false;

	FILE* csv_file = fopen(csv_name.data(), "r");

	if(!csv_file)
	{
		perror(csv_name.data());
		return false;
	}

	char* buffer = nullptr;
	size_t buffer_size = 0;
	ssize_t line_size;

	char* token;

	/// Get line and trim the new line character.
	auto get_csv_line = [&]() -> bool
	{
		line_size = getline(&buffer, &buffer_size, csv_file);

		if(line_size == -1)
			return false;

		if(line_size > 0)
			buffer[line_size-- - 1] = '\0';

		return true;
	};

	// Get a token from the csv line
	auto get_token = [&]() -> bool
	{
		return (token = strsep(&buffer, ",")) != nullptr;
	};

	auto push_token = [&](
		std::vector<statement::literal>& row,
		size_t column,
		const char* str
		) -> bool
	{
		if(column >= columns.size())
			return false;

		switch(columns[column].second.t)
		{
			case statement::type::_type::INT:
				row.emplace_back(atoi(str));
				break;

			case statement::type::_type::VARCHAR:
				row.emplace_back(std::string(str,
					std::min(columns[column].second.size, strlen(str))
				));
				break;

			default:
				return false;
		}

		return true;
	};

	auto clean = [&]() -> bool
	{
		free(buffer);
		return fclose(csv_file);
	};

	if(!get_csv_line())
	{
		// No header
		clean();
		return false;
	}

	for(size_t column = 0; get_token(); column++)
	{
		if(column >= columns.size())
			fprintf(stderr, "Extra columns.\n");
		else if(columns[column].first != token)
			fprintf(stderr, "\"%s\" column out of order.\n", token);
		else
			continue;

		clean();
		return false;
	}

	std::vector<std::vector<statement::literal>> new_rows;

	while(get_csv_line())
	{
		std::vector<statement::literal> new_row;

		for(size_t column = 0; get_token(); column++)
		{
			if(!push_token(new_row, column, token))
			{
				clean();
				fprintf(stderr, "Extra columns.\n");
				return false;
			}
		}

		new_rows.push_back(std::move(new_row));
	}

	if(!write_data(new_rows))
	{
		clean();
		return false;
	}

	return clean();
}

bool db2::table::write_data(std::vector<std::vector<statement::literal>>& data)
{
	using namespace db2::statement;

	if(!check_data(data))
		return false;

	printf("Rows: %lu\n", data.size());
	for(const auto& row: data)
	{
		for(const auto& cell: row)
		{
			std::cout << cell << ' ';
		}
		std::cout << '\n';
	}

	// TODO

	return false;
}

bool db2::table::check_data(std::vector<std::vector<statement::literal>>& data)
{
	for(auto& row: data)
	{
		// Check size
		if(row.size() != columns.size())
			return false;

		// Check types and size
		for(size_t i = 0; i < columns.size(); i++)
		{
			switch(columns[i].second.t)
			{
				case statement::type::_type::INT:
					if(!std::holds_alternative<int>(row[i]))
						return false;
					break;

				case statement::type::_type::VARCHAR:
					if(!std::holds_alternative<std::string>(row[i]))
						return false;
					else
					{
						std::string& str = std::get<std::string>(row[i]);

						if(str.size() > columns[i].second.size)
							str = std::string(str.c_str(), columns[i].second.size);
					}
					break;

				default:
					break;
			}
		}
	}

	return true;
}

size_t db2::table::tuple_size() const
{
	size_t size = 0;

	for(const auto& [str, type]: columns)
	{
		size += type.size;
	}

	return size;
}
