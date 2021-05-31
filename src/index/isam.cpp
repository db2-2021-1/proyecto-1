#include <fstream>

#include "isam.hpp"

db2::index::isam::isam(
	const std::filesystem::path& index_path,
	db2::statement::type key_type
):
	index_path(index_path),
	key_type(key_type)
{
	std::ifstream ifs(index_path, std::ios::binary);

	if(!ifs.is_open())
		return;

	size_t key_size = key_type.size +
		(key_type.t == statement::type::_type::VARCHAR? 1 : 0);

	size_t pair_size = key_size + sizeof(size_t);

	char* buffer = (char*)malloc(pair_size);

	std::pair<db2::literal, size_t> new_pair;
	while(!ifs.eof())
	{
		ifs.read(buffer, pair_size);

		new_pair.first = read(buffer, key_type);
		new_pair.second = *(size_t*)(buffer+key_size);

		index.insert(std::move(new_pair));
	}

	free(buffer);
}

db2::index::isam::~isam()
{
	std::ofstream ofs(index_path, std::ios::binary);

	if(!ofs.is_open())
	{
		perror(index_path.c_str());
		return;
	}

	for(const auto& [key, pos]: index)
	{
		write(key, key_type.size, ofs);
		ofs.write((char*)pos, sizeof(pos));
	}
}

std::vector<size_t> db2::index::isam::get_positions(const db2::literal& key)
{
	std::vector<size_t> positions;

	auto it = std::lower_bound(index.begin(), index.end(), std::make_pair(key, 0UL));

	for(; it != index.end(); it++)
	{
		if(it->first != key)
			break;

		positions.push_back(it->second);
	}

	return positions;
}

std::vector<size_t> db2::index::isam::get_positions(
	const db2::literal& key_ge,
	const db2::literal& key_le
)
{
	std::vector<size_t> positions;

	auto it = std::lower_bound(index.begin(), index.end(), std::make_pair(key_ge, 0UL));

	for(; it != index.end(); it++)
	{
		if(it->first > key_le)
			break;

		positions.push_back(it->second);
	}

	return positions;
}

bool db2::index::isam::insert(const db2::literal& key, size_t position)
{
	index.insert({key, position});
	return true;
}

bool db2::index::isam::delete_from(const db2::literal& key, size_t position)
{
	index.erase({key, position});
	return true;
}
