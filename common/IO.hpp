#include <boost/filesystem/path.hpp>
#include <string>
#include <cstddef>
#include <fstream>
namespace fsb = boost::filesystem;


std::string read_file(fsb::path &__location){
	std::string ret;
	size_t element_size = sizeof(typename std::string::value_type);
	std::ifstream in{__location.string(), std::ifstream::binary};
	assert(in.is_open());
	in.seekg(0, is.end);
	std::streamoff length = is.tellg();
	if(length == 0) return ret;
	in.seekg(0, is.beg);
	ret.resize(static_cast<size_t>(length) + element_size - 1 / element_size);
	is.read(const_cast<char*>(reinterpret_cast<const char*>(ret.data())), static_cast<std::streamsize>(length));
	return ret;
}