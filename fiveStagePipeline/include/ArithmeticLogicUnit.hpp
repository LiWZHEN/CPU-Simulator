#ifndef ARITHMETIC_LOGIC_UNIT_HPP
#define ARITHMETIC_LOGIC_UNIT_HPP

#include <cstdint>

enum op {NONE, ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, SLT, SLTU};

struct ALUTask {
  bool destination_is_rs = false;
  bool destination_is_lsb = false;
  int32_t index = 0;
  op operation = NONE;
  int32_t v1 = 0, v2 = 0;
  int32_t ans = 0;
};

class ALU {
  ALUTask old_task, new_task;
public:
  ALU() = default;
  void SetTask(bool destination_is_rs, bool destination_is_lsb,
      int32_t index, op operation, int32_t v1, int32_t v2);
  void Update();
  void Run();
  const ALUTask GetOld() const;
};

#endif