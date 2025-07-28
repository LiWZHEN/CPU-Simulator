#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <unordered_map>
#include <queue>
#include <cstdint>
#include <string>

class Memory {
  struct ModifyRecordUnit {
    int32_t index;
    int32_t value;
    ModifyRecordUnit() = default;
    ModifyRecordUnit(const int32_t ind, const int32_t val) {
      index = ind;
      value = val;
    }
  };
  std::unordered_map<int32_t, uint8_t> old_memories;
  std::unordered_map<int32_t, uint8_t> new_memories;
  std::queue<ModifyRecordUnit> modify_record;
public:
  Memory() = default;
  void Add(const int32_t address, const uint8_t data);
  int32_t Get(const int32_t address) const;
  void Update();
};
#endif