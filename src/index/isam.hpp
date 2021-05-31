#include <filesystem>
#include <set>
#include <vector>

#include "literal.hpp"
#include "statement.hpp"

#pragma once

namespace db2::index
{

class isam
{
private:
	std::set<std::pair<db2::literal, size_t>> index;
	std::filesystem::path index_path;
	db2::statement::type key_type;

public:
	/// Use existing index.
	isam(const std::filesystem::path& index_path, db2::statement::type key_type);
	~isam();

	std::vector<size_t> get_positions(const db2::literal& key);
	std::vector<size_t> get_positions(const db2::literal& key_ge, const db2::literal& key_le);
	bool insert(const db2::literal& key, size_t position);
	bool delete_from(const db2::literal& key, size_t position);
};

};
