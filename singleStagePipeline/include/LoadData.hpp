#ifndef LOAD_DATA_HPP
#define LOAD_DATA_HPP

#include "Exception.hpp"
#include <cstdint>
#include "Memory.hpp"

class DataLoader {
public:
  DataLoader() = default;
  void LoadData(Memory &memory);
};

#endif