#include <bitset>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#pragma once

namespace db2::index
{

/// Index file specficication:
/// [header]
/// [bucket_pointer]*2^D
/// [bucket_header [key_position]*bucket_max_elements bucket_pointer]...
class extendible_hash
{

private:
	struct header
	{
		/// The global depth.
		size_t D;

		/// The max number of key-position pairs on a bucket.
		size_t bucket_max_elements;
	};

	struct bucket_header
	{
		/// The local depth.
		size_t d;

		/// The number of key-position pairs on a bucket.
		size_t size;
	};

	struct key_position
	{
		/// The hash itself.
		size_t key;

		/// The position of the key in the data file.
		size_t position;
	};

	/// Pointer to a bucket position.
	using bucket_p = size_t;

	header h;

	std::fstream index_file;

public:
	/// Use existing index.
	extendible_hash(const std::filesystem::path& index_path);

	/// Create a new index.
	extendible_hash(
		const std::filesystem::path& index_path,
		size_t D,
		size_t bucket_max_elements
	);

	bool is_open() const;

	std::vector<size_t> get_positions(size_t key_hash);
};

};
