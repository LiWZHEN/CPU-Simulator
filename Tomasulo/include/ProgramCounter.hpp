#ifndef PROGRAM_COUNTER_HPP
#define PROGRAM_COUNTER_HPP

#include "Classes.hpp"
#include "Memory.hpp"
#include "Decoder.hpp"

struct PCTask {
  bool predict_failed = false;
  bool wait_for_next = false;
  bool rob_is_full = false;
  bool force_jump = false;
  int32_t jump_to = -1;
};

class ProgramCounter {
  Memory *memory = nullptr;
  Decoder *decoder = nullptr;

  PCTask task;

  int32_t pc = 0;
  bool predict_failed = false;
  bool wait_for_next = false;
  bool rob_is_full = false;
  bool force_jump = false;
  int32_t jump_to = -1;

public:
  ProgramCounter() = default;
  void Connect(Memory *memory, Decoder *decoder);
  void SetPCTask(int32_t to);
  void SetPCWait();
  int32_t GetMachineCode();
  void PredictFailed(int32_t another_branch_pos);
  void ROBFull();
  void Update();
  void Run();
};
#endif