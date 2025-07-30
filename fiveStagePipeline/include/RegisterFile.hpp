#ifndef REGISTER_FILE_HPP
#define REGISTER_FILE_HPP

#include <cstdint>
#include "Decoder.hpp"

struct TaskUnitFromROB {
  InstructionType type = InstructionType::NONE;
  int32_t rd = -1, value = 0, rob_ind = -1;
};

struct RFTask {
  bool predict_failed = false;
  int32_t ind_from_decoder = 0;
  int32_t dependence_from_decoder = -1;
  TaskUnitFromROB task_from_rob[32];
  int32_t task_from_rob_len = 0;
};

class RegisterFile {
  Decoder *decoder = nullptr;
  LSB *lsb = nullptr;

  RFTask task;
  
  bool predict_failed = false;
  int32_t ind_from_decoder = 0;
  int32_t dependence_from_decoder = -1;
  TaskUnitFromROB task_from_rob[32];
  int32_t task_from_rob_len = 0;
  int32_t rf[32] = {0};
  int32_t dependence[32] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
public:
  RegisterFile() = default;
  void Connect(Decoder *decoder, LSB *lsb);
  void SetNewDepenence(int32_t rd, int32_t dependence);
  void SetFromROB(InstructionType type, int32_t rd, int32_t value, int32_t rob_ind);
  void PredictFailed();
  void Update();
  void Run();
};
#endif