#ifndef ARITHMETIC_LOGIC_UNIT_HPP
#define ARITHMETIC_LOGIC_UNIT_HPP

#include <cstdint>
#include "ReorderBuffer.hpp"
#include "ReservationStation.hpp"
#include "LoadStoreBuffer.hpp"
#include "ProgramCounter.hpp"

struct ALUTask {
  bool predict_failed = false;
  int32_t destination = 0;
  InstructionType type = InstructionType::NONE;
  int32_t v1 = 0, v2 = 0;
};

class ALU {
  ROB *rob = nullptr;
  RS *rs = nullptr;
  LSB *lsb = nullptr;
  ProgramCounter *pc = nullptr;

  ALUTask task;

  bool predict_failed = false;
  int32_t destination = 0;
  InstructionType type = InstructionType::NONE;
  int32_t v1 = 0, v2 = 0;
public:
  ALU() = default;
  void Connect(ROB *rob, RS *rs, LSB *lsb, ProgramCounter *pc);
  void SetTask(int32_t destination, InstructionType type, int32_t v1, int32_t v2);
  void Update();
  void SetPredictFault();
  void Run();
};

#endif