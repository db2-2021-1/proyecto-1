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
	for(size_t i = 0; i < (1LU<<D); i++)
	{
		index_file.write((char*)&default_pointer, sizeof(default_pointer));
	}
}

bool db2::index::extendible_hash::is_open() const
{
	return index_file.is_open();
}

std::vector<size_t> db2::index::extendible_hash::get_positions(size_t key_hash)
{
	std::vector<size_t> positions;

	// Go to the position of the bucket pointer.
	index_file.seekg(sizeof(header) + sizeof(bucket_p)*(key_hash%(1L<<h.D)));

	bucket_p bucket_pos;
	index_file.read((char*)&bucket_pos, sizeof(bucket_pos));

	for(;bucket_pos != 0; index_file.read((char*)&bucket_pos, sizeof(bucket_pos)))
	{
		// Go to the bucket.
		index_file.seekg(bucket_pos);

		// Read the bucket metadata.
		bucket_header bh;
		index_file.read((char*)&bh, sizeof(bh));

		key_position kp;
		for(size_t i = 0; i < bh.size; i++)
		{
			index_file.read((char*)&kp, sizeof(kp));
			if(kp.key == key_hash)
				positions.push_back(kp.position);
		}

		// Go to the overflow bucket pointer
		index_file.seekg(
			(h.bucket_max_elements-bh.size)*sizeof(key_position),
			std::ios::cur
		);
	}

	return positions;
}
