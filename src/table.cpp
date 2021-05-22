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

#ifndef PIPE_BUF
#define PIPE_BUF 1
#endif

#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>

#include "table.hpp"

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
	FILE* file =
#ifdef _MSC_VER
		fopen(metadata_path().c_str(), L"r");
#else
		fopen(metadata_path().c_str(), "r");
#endif

	if(!file)
	{
		perror(table_name.c_str());
		return false;
	}

	char buffer[PIPE_BUF];

	//TODO

	if(fclose(file) != 0)
	{
		perror(metadata_path().c_str());
		return false;
	}

	return true;
}

bool db2::table::write_metadata() const
{
	if(!check_and_create_directory())
		return false;

	FILE* file =
#ifdef _MSC_VER
		fopen(metadata_path().c_str(), L"w");
#else
		fopen(metadata_path().c_str(), "w");
#endif

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
		perror(metadata_path().c_str());
		return false;
	}

	return true;
}
