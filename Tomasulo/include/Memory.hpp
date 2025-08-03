#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "Classes.hpp"
#include <unordered_map>
#include <cstdint>

#include "LoadStoreBuffer.hpp"
#include "Decoder.hpp"
#include "ReorderBuffer.hpp"
#include "ReservationStation.hpp"

struct MemoryTask {
  InstructionType type = InstructionType::NONE;
  int32_t address = -1, value = 0;
};

class Memory {
  LSB *lsb = nullptr;
  Decoder *decoder = nullptr;
  ROB *rob = nullptr;
  RS *rs = nullptr;

  MemoryTask task;

  InstructionType type = InstructionType::NONE;
  int32_t address = -1, value = 0;
  std::unordered_map<int32_t, uint8_t> memories;
public:
  Memory() = default;
  void Connect(LSB *lsb, Decoder *decoder, ROB *rob, RS *rs);
  void Add(const int32_t address, const uint8_t data);
  int32_t Get(const int32_t address) const;
  void Update();
  void Run();
  void SetTask(InstructionType type, int32_t address, int32_t value);
};
#endif