#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <unordered_map>
#include <cstdint>
#include <string>
class Memory {
  std::unordered_map<int32_t, int8_t> memories;
public:
  Memory() = default;
  void Add(const int32_t address, const int8_t data);
  int8_t Get(const int32_t address) const;
};
#endif