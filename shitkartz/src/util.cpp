#include "util.h"
std::optional<std::vector<char>> util::readFile(const std::string filename){
    std::fstream f(filename, std::ios::ate | std::ios::binary);
    size_t size = f.tellg();
    std::vector<char> buffer(size);
    if(!f.is_open()){
      return std::nullopt;
    }
    f.seekg(0);
    f.read(buffer.data(), size);
    f.close();
    return buffer;
  }
