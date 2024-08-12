#pragma once
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <cassert>
namespace fs = boost::filesystem;

std::string file_read(fs::path __file){
    std::string ret;
    assert(fs::exists(__file));
    const size_t element_size = sizeof(typename std::string::value_type);
    std::ifstream in{__file.string(), std::ifstream::binary};
    assert(in.is_open());
    in.seekg(0, in.end);
    std::streamoff length = in.tellg();
    if (length == 0)
    {
        return ret;
    }
    ret.resize((static_cast<size_t>(length) + element_size - 1) / element_size);
    in.read(const_cast<char*>(reinterpret_cast<const char*>(ret.data())), static_cast<std::streamsize>(length));
    
    return ret;

}