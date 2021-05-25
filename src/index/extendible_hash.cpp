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

	bucket_p default_bucket[2] = {
		get_first_bucket(),
		get_first_bucket()+bucket_size()
	};

	for(size_t i = 0; i < (1LU<<D); i++)
	{
		index_file.write((char*)&default_bucket[i%2], sizeof(default_bucket[0]));
	}

	write_empty_bucket(1);
	write_empty_bucket(1);
}

bool db2::index::extendible_hash::is_open() const
{
	return index_file.is_open();
}

void db2::index::extendible_hash::write_empty_bucket(size_t d)
{
	bucket_header bh{d, 0};
	key_position kp{0,0};
	bucket_p no_bucket = 0;

	index_file.write((char*)&bh, sizeof(bh));
	for(size_t i = 0; i < h.bucket_max_elements; i++)
	{
		index_file.write((char*)&kp, sizeof(kp));
	}
	index_file.write((char*)&no_bucket, sizeof(no_bucket));
}

size_t db2::index::extendible_hash::bucket_size() const
{
	return
		sizeof(bucket_header) +
		sizeof(key_position)*h.bucket_max_elements +
		sizeof(bucket_p)
	;
}

size_t db2::index::extendible_hash::d_bit(size_t d, size_t key_hash) const
{
	return key_hash%(1LU<<d);
}

size_t db2::index::extendible_hash::D_bit(size_t key_hash) const
{
	return d_bit(h.D, key_hash);
}

size_t db2::index::extendible_hash::get_directory(size_t key_hash) const
{
	return sizeof(header) + sizeof(bucket_p)*D_bit(key_hash);
}

size_t db2::index::extendible_hash::get_first_bucket() const
{
	return sizeof(header) + sizeof(bucket_p)*(1LU<<h.D);
}

db2::index::extendible_hash::bucket_p
	db2::index::extendible_hash::get_bucket(size_t key_hash)
{
	// Go to the position of the bucket pointer.
	index_file.seekg(get_directory(key_hash));

	bucket_p bucket_pos;
	index_file.read((char*)&bucket_pos, sizeof(bucket_pos));

	return bucket_pos;
}

std::vector<size_t> db2::index::extendible_hash::get_positions(size_t key_hash)
{
	std::vector<size_t> positions;

	for(
		bucket_p bucket_pos = get_bucket(key_hash);
		bucket_pos != 0;
		index_file.read((char*)&bucket_pos, sizeof(bucket_pos))
	)
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

bool db2::index::extendible_hash::insert(size_t key_hash, size_t position)
{
	key_position kp{key_hash, position};

	bucket_p bucket_pos = get_bucket(key_hash);

	if(bucket_pos == 0)
	{
		fprintf(stderr, "Missing bucket pointer\n");
		return false;
	}

	bucket_header bh;
	index_file.seekg(bucket_pos);
	index_file.read((char*)&bh, sizeof(bh));

	if(bh.size < h.bucket_max_elements) // Bucket with enough space
	{
		bh.size++;

		index_file.seekp(bucket_pos);
		index_file.write((char*)&bh, sizeof(bh));

		index_file.seekp(sizeof(key_position)*(bh.size-1), std::ios::cur);
		index_file.write((char*)&kp, sizeof(kp));
	}
	else if(bh.d < h.D) // Split bucket
	{
		// TODO
		return false;
	}
	else // Overflow bucket
	{
		// TODO
		return false;
	}

	return true;
}
