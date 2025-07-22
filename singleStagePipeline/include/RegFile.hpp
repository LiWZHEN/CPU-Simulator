#ifndef REG_FILE_HPP
#define REG_FILE_HPP

#include <cstdint>

class RegFile {
  int32_t registers[32];
public:
  RegFile() = default;
  RegFile(const RegFile &other);
  void Modify(const int ind, const int x);
  int32_t GetData(const int ind);
};
#endif