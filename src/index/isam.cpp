#include "isam.hpp"

db2::index::isam::isam(
	const std::filesystem::path& index_path,
	db2::statement::type key_type
)
{
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
