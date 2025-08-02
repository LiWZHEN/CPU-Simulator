#ifndef RESERVATION_STATION_HPP
#define RESERVATION_STATION_HPP

#include "Classes.hpp"
#include "ArithmeticLogicUnit.hpp"
#include "Decoder.hpp"

struct RSEntry {
  bool busy = false;
  InstructionType type = InstructionType::NONE;
  int32_t V1, V2, Q1, Q2, destination;
};

struct IndAndValue {
  int32_t rob_ind = 0;
  int32_t value = 0;
};

struct RSTask {
  bool predict_failed = false;
  RSEntry entry_from_decoder;
  InstructionType type_from_alu = InstructionType::NONE;
  int32_t value_from_alu = 0, rob_ind_from_alu = -1;
  int32_t loaded_ind = -1, loaded_value = 0;
  int32_t rob_ind[32], rob_value[32], rob_table_size = 0;
  int32_t rob_tail = 0;
  int32_t new_dependence_rd = -1;
  int32_t new_dependence_rob_ind = -1;
  int32_t rs1 = -1, rs2 = -1;
};

class RS {
  ALU *alu;

  bool predict_failed = false;
  RSEntry entry_from_decoder;
  InstructionType type_from_alu = InstructionType::NONE;
  int32_t value_from_alu = 0, rob_ind_from_alu = -1;
  int32_t loaded_ind = -1, loaded_value = 0;
  int32_t rob_ind[32], rob_value[32], rob_table_size = 0;
  int32_t rob_tail = -1;
  int32_t new_dependence_rd = -1;
  int32_t new_dependence_rob_ind = -1;
  int32_t rs1 = -1, rs2 = -1;

  RSTask task;

  RSEntry rs_entries[32];
public:
  RS() = default;
  void Connect(ALU *alu);
  void SetFromDecoder(InstructionType type, int32_t V1, int32_t V2, int32_t Q1, int32_t Q2, int32_t destination);
  void SetFromALU(InstructionType type, int32_t result, int32_t rob_ind);
  void GetLoadedData(int32_t rob_ind, int32_t value);
  void PredictFailed();
  void GetROBTable(int32_t rob_ind[], int32_t value[], int32_t size);
  void PassROBTail(int32_t tail);
  void Update();
  void Run();
  void SetNewDependence(int32_t rd, int32_t dependence);
  void PassRS(int32_t rs1, int32_t rs2);
  void Print();
};
#endif