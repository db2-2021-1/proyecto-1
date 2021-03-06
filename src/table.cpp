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
#include <fstream>
#include <map>
#include <system_error>

#ifndef PIPE_BUF
#define PIPE_BUF 1
#endif

#include <rapidjson/error/en.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>

#include "index.hpp"
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

std::filesystem::path db2::table::data_path() const
{
	return std::filesystem::path(table_name) / "data.dat";
}

std::filesystem::path db2::table::index_path() const
{
	return std::filesystem::path(table_name) / "index.dat";
}

ssize_t db2::table::name2index(std::string_view column_name) const
{
	size_t column_index = 0;
	bool column_found = false;
	for(const auto& [name, type]: columns)
	{
		if(name == column_name)
		{
			column_found = true;
			break;
		}
		column_index++;
	}

	if(!column_found)
	{
		fprintf(stderr, "Bad index\n");
		return -1;
	}
	return column_index;
}

ssize_t db2::table::key_index() const
{
	if(!table_index.has_value())
		return -1;

	return name2index(table_index->column_name);
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

bool db2::table::has_index() const
{
	return table_index.has_value();
}

bool db2::table::read_csv(std::string_view csv_name)
{
	if(columns.empty())
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
	char* buffer_copy;
	bool line_consumed;

	/// Get line and trim the new line character.
	auto get_csv_line = [&]() -> bool
	{
		line_size = getline(&buffer, &buffer_size, csv_file);

		if(line_size == -1)
			return false;

		if(line_size > 0)
		{
			buffer[line_size-- - 1] = '\0';
			if(line_size > 0 && buffer[line_size-1] == '\r')
			{
				buffer[line_size-- - 1] = '\0';
			}
		}

		line_consumed = false;
		buffer_copy = buffer;
		return true;
	};

	// Get a token from the csv line
	auto get_token = [&]() -> bool
	{
		enum class state
		{
			not_escaped,
			escaped
		};

		if(buffer_copy == nullptr)
			return false;

		token = buffer_copy;
		state s = state::not_escaped;
		while(!line_consumed)
		{
			switch(s)
			{
				case state::not_escaped:
					switch(buffer_copy[0])
					{
						case ',':
							buffer_copy[0] = '\0';
							buffer_copy++;
							return true;

						case '\0':
							line_consumed = true;
							return true;

						case '"':
							token = buffer_copy+1;
							s = state::escaped;
							break;
					}
					break;

				case state::escaped:
					switch(buffer_copy[0])
					{
						case '"':
							buffer_copy[0] = '\0';
							s = state::not_escaped;
							break;
					}
					break;
			}
			buffer_copy++;
		}

		return false;
	};

	auto push_token = [&](
		statement::row& row,
		size_t column,
		const char* str
		) -> bool
	{
		if(column >= columns.size())
			return false;

		switch(columns[column].second.t)
		{
			case statement::type::_type::INT:
				row.values.emplace_back(atoi(str));
				break;

			case statement::type::_type::REAL:
				row.values.emplace_back(strtof(str, nullptr));
				break;

			case statement::type::_type::VARCHAR:
				row.values.emplace_back(std::string(str,
					std::min(columns[column].second.size, strlen(str))
				));
				break;

			default:
				fprintf(stderr, "Type not supported\n");
				return false;
		}

		return true;
	};

	auto clean = [&]() -> bool
	{
		free(buffer);
		return fclose(csv_file) == 0;
	};

	if(!get_csv_line())
	{
		clean();
		fprintf(stderr, "No header\n");
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

	std::vector<statement::row> new_rows;

	while(get_csv_line())
	{
		statement::row new_row;

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

	if(!append_data(new_rows))
	{
		clean();
		fprintf(stderr, "Can't write data\n");
		return false;
	}

	if(!clean())
		return false;

	return update_index(new_rows);
}

bool db2::table::append_data(std::vector<statement::row>& data)
{
	using namespace db2::statement;

	if(!check_data(data))
		return false;

	std::ofstream ofs(data_path(), std::ios::app | std::ios::binary);

	if(!ofs.is_open())
	{
		perror(data_path().c_str());
		return false;
	}

	for(auto& r: data)
	{
		write(ofs, r);
	}

	return true;
}

bool db2::table::write_data(std::vector<statement::row>& data)
{
	if(!check_data(data))
		return false;

	std::fstream ofs(data_path(), std::ios::in | std::ios::out | std::ios::binary);

	if(!ofs.is_open())
	{
		perror(data_path().c_str());
		return false;
	}

	for(auto& row: data)
	{
		write(ofs, row);
	}

	return true;
}

bool db2::table::update_hash_index(const std::vector<statement::row>& data)
{
	static const size_t default_D = 8;
	static const size_t default_max_bucket_size = 32;

	auto hash_index = std::filesystem::exists(index_path())?
		db2::index::extendible_hash(index_path()):
		db2::index::extendible_hash(index_path(), default_D, default_max_bucket_size)
	;

	if(!hash_index.is_open())
		return false;

	assert(table_index.has_value());
	assert(table_index->type == statement::index_type::e_hash);

	ssize_t column_index = key_index();

	if(column_index == -1)
		return false;

	auto hash = std::hash<literal>{};
	for(const auto& r: data)
	{
		size_t h = hash(r.values[column_index]);

		if(r.valid)
		{
			if(!hash_index.insert(h, r.pos))
				return false;
		}
		else
		{
			if(!hash_index.delete_from(h, r.pos))
				return false;
		}
	}

	return true;
}

bool db2::table::update_isam_index(const std::vector<statement::row>& data)
{
	ssize_t column_index = key_index();

	auto index = db2::index::isam(index_path(), columns[column_index].second);

	for(const auto& r: data)
	{
		const literal& key = r.values[column_index];

		if(r.valid)
		{
			if(!index.insert(key, r.pos))
				return false;
		}
		else
		{
			if(!index.delete_from(key, r.pos))
				return false;
		}
	}

	return true;
}

bool db2::table::update_bptree_index(const std::vector<statement::row>& data)
{
	b_plus_tree bp_tree(index_path(), columns[key_index()].second);

	ssize_t column_index = key_index();

	for(const auto& r: data)
	{
		const literal& key = r.values[column_index];

		if(r.valid)
		{
			if(!bp_tree.insert(key, r.pos))
				return false;
		}
		else
		{
			if(!bp_tree.delete_from(key, r.pos))
				return false;
		}
	}

	return true;
}

bool db2::table::check_data(std::vector<statement::row>& data)
{
	for(auto& row: data)
	{
		// Check size
		if(row.values.size() != columns.size())
			return false;

		// Check types and size
		for(size_t i = 0; i < columns.size(); i++)
		{
			switch(columns[i].second.t)
			{
				case statement::type::_type::INT:
					if(!std::holds_alternative<int>(row.values[i]))
						return false;
					break;

				case statement::type::_type::VARCHAR:
					if(!std::holds_alternative<std::string>(row.values[i]))
						return false;
					else
					{
						std::string& str = std::get<std::string>(row.values[i]);

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
		// The varchar is null terminated.
		if(type.t == statement::type::_type::VARCHAR)
			size++;
	}

	return size + sizeof(bool);
}

void db2::table::write(std::ostream& os, statement::row& r) const
{
	size_t i = 0;
	if(r.pos != -1)
		os.seekp(r.pos);
	else
		r.pos = os.tellp();

	for(const auto& cell: r.values)
	{
		size_t size = columns[i++].second.size;

		db2::write(cell, size, os);
	}

	os.write((char*)&r.valid, sizeof(r.valid));
}

db2::statement::row db2::table::read(std::istream& is, char* buffer) const
{
	statement::row new_row;

	new_row.pos = is.tellg();

	for(const auto& [name, type]: columns)
	{
		// Add one byte for the VARCHAR's '\0'
		is.read(buffer, type.size + (type.t == statement::type::_type::VARCHAR? 1 : 0));
		if(is.eof())
			return statement::row{false,-1,{}};

		new_row.values.push_back(db2::read(buffer, type));
	}

	is.read((char*)&new_row.valid, sizeof(new_row.valid));

	return new_row;
}

std::vector<size_t> db2::table::select_equals(const literal& key)
{
	assert(table_index.has_value());

	switch(table_index->type)
	{
		case statement::index_type::bp_tree:
		{
			b_plus_tree bp_tree(index_path(), columns[key_index()].second);

			return bp_tree.get_positions(key);
		}

		case statement::index_type::e_hash:
		{
			db2::index::extendible_hash eh(index_path());
			if(!eh.is_open())
				return {};

			return eh.get_positions(std::hash<literal>{}(key));
		}

		case statement::index_type::isam:
		{
			db2::index::isam isam(index_path(), columns[key_index()].second);

			return isam.get_positions(key);
		}

		case statement::index_type::none:
			break;
	}
	return {};
}

std::vector<size_t> db2::table::select_range(
	const literal& ge,
	const literal& le
	)
{
	switch(table_index->type)
	{
		case statement::index_type::bp_tree:
		{
			b_plus_tree bp_tree(index_path(), columns[key_index()].second);

			return bp_tree.get_positions(ge, le);
		}

		case statement::index_type::e_hash:
		case statement::index_type::none:
			break;

		case statement::index_type::isam:
		{
			db2::index::isam isam(index_path(), columns[key_index()].second);

			return isam.get_positions(ge, le);
		}
	}

	return {};
}

std::vector<db2::statement::row> db2::table::select_all(
	const std::optional<statement::expression>& expr)
{
	ssize_t expr_index = expr.has_value() ? name2index(expr->column) : -1;
	auto valid = [&](const statement::row& r) -> bool
	{
		if(!r.valid)
			return false;

		if(!expr.has_value())
			return true;

		assert(expr_index != -1);

		switch(expr->t)
		{
			case statement::expression::type::between:
				return expr->value[0] <= r.values[expr_index] &&
					r.values[expr_index] <= expr->value[1]
					;

			case statement::expression::type::is:
				return r.values[expr_index] == expr->value[0];
		}

		return false;
	};

	std::vector<statement::row> rows;

	std::ifstream ifs(data_path());


	if(!ifs.is_open())
	{
		perror(data_path().c_str());
		return rows;
	}

	char* buffer = (char*)malloc(tuple_size());

	while(!ifs.eof())
	{
		auto row = read(ifs, buffer);
		if(valid(row))
			rows.push_back(std::move(row));
	}

	free(buffer);
	return rows;
}

std::vector<db2::statement::row> db2::table::get_data(
	const std::optional<statement::expression>& expr
	)
{
	if(!expr.has_value() || expr->column != get_table_index_name())
		return select_all(expr);

	auto read_positions = [this](const std::vector<size_t>& positions)
	{
		std::vector<statement::row> rows;

		std::ifstream ifs(data_path());

		if(!ifs.is_open())
		{
			perror(data_path().c_str());
			return rows;
		}

		char* buffer = (char*)malloc(tuple_size());

		for(size_t pos: positions)
		{
			ifs.seekg(pos);
			auto row = read(ifs, buffer);
			if(row.valid)
				rows.push_back(std::move(row));
		}

		free(buffer);

		return rows;
	};

	switch(expr->t)
	{
		case statement::expression::type::between:
			if(get_index_type() == statement::index_type::e_hash)
				return select_all(expr);
			return read_positions(select_range(expr->value[0], expr->value[1]));

		case statement::expression::type::is:
			return read_positions(select_equals(expr->value[0]));

		default:
			return {};
	}
}

void db2::table::print_columns(std::ostream& os) const
{
	for(const auto& [name, type]: columns)
	{
		os << name << (name == columns.rbegin()->first? '\n' : ',');
	}
}

std::string db2::table::get_table_index_name() const
{
	if(table_index.has_value())
		return table_index->column_name;
	return "";
}

db2::statement::index_type db2::table::get_index_type() const
{
	if(table_index.has_value())
		return table_index->type;
	return statement::index_type::none;
}

bool db2::table::update_index(std::vector<statement::row>& data)
{
	if(table_index.has_value())
	{
		switch(table_index->type)
		{
			case statement::index_type::e_hash:
				return update_hash_index(data);

			case statement::index_type::bp_tree:
				return update_bptree_index(data);

			case statement::index_type::isam:
				return update_isam_index(data);

			case statement::index_type::none:
				break;
		}
	}

	return true;
}

bool db2::table::reset_index()
{
	std::error_code e;

	std::filesystem::remove(index_path(), e);

	if(!std::filesystem::exists(data_path()))
		return true;

	auto data = get_data();

	return update_index(data);
}
