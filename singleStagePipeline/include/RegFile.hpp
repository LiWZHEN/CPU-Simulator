#ifndef REG_FILE_HPP
#define REG_FILE_HPP

#include <cstdint>

class RegFile {
  int32_t registers[32] = {};
public:
  RegFile() = default;
  void Modify(const int32_t ind, const int32_t x);
  int32_t GetData(const int32_t ind) const;
  void Print();
};
#endif