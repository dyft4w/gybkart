#pragma once
#include "includes.h"
#include <optional>
namespace util{
  std::optional<std::vector<char>> readFile(const std::string filename);
}
