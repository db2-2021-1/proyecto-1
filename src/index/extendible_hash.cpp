#include "extendible_hash.hpp"

db2::index::extendible_hash::extendible_hash(const std::filesystem::path& index_path):
	index_file(index_path, std::ios::binary | std::ios::in | std::ios::out)
{
	if(!index_file.is_open())
	{
		perror(index_path.c_str());
		return;
	}

	index_file.read((char*)&h, sizeof(h));
}

db2::index::extendible_hash::extendible_hash(
	const std::filesystem::path& index_path,
	size_t D,
	size_t bucket_max_elements):
	index_file(index_path, std::ios::binary | std::ios::in | std::ios::out)
{
	if(!index_file.is_open())
	{
		perror(index_path.c_str());
		return;
	}

	h.D                   = D;
	h.bucket_max_elements = bucket_max_elements;

	index_file.write((char*)&h, sizeof(h));

	bucket_p default_pointer = 0;
	for(size_t i = 0; i < D*D; i++)
	{
		index_file.write((char*)&default_pointer, sizeof(default_pointer));
	}
}

bool db2::index::extendible_hash::is_open() const
{
	return index_file.is_open();
}

db2::index::extendible_hash::~extendible_hash()
{
	if(index_file.is_open())
		index_file.close();
}
