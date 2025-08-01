#ifndef LOAD_STORE_BUFFER_HPP
#define LOAD_STORE_BUFFER_HPP

#include "Classes.hpp"
#include "Memory.hpp"
#include "Decoder.hpp"
#include "ProgramCounter.hpp"

struct LSBEntry {
  InstructionType type = InstructionType::NONE;
  int32_t address = -1, cycle = -1, rob_ind = -1;
  bool committed = false;
};

struct LSBStructure {
  int32_t head = 0, tail = 0;
  LSBEntry lsb_entries[32];
};

struct StoreFromRF {
  int32_t address = -1, value = 0, rob_ind = -1;
};

struct LSBTask {
  InstructionType type_from_decoder = InstructionType::NONE;
  int32_t address_from_decoder = -1, cycle_from_decoder = -1, rob_index_from_decoder = -1;
  bool committed_from_decoder = false;
  InstructionType type_from_alu = InstructionType::NONE;
  int32_t result_from_alu = 0, rob_ind_from_alu = -1;
  StoreFromRF store_from_rf[32];
  int32_t store_rf_num = 0;
  int32_t loaded_rob_ind = -1, loaded_value = 0;
  bool predict_failed = false;
  int32_t rob_ind[32], value[32], size = 0;
  int32_t rob_tail = 0;
};

class LSB {
  Memory *memory = nullptr;
  Decoder *decoder = nullptr;
  ProgramCounter *pc = nullptr;

  LSBTask task;

  InstructionType type_from_decoder = InstructionType::NONE;
  int32_t address_from_decoder = -1, cycle_from_decoder = -1, rob_index_from_decoder = -1;
  bool committed_from_decoder = false;
  InstructionType type_from_alu = InstructionType::NONE;
  int32_t result_from_alu = 0, rob_ind_from_alu = -1;
  StoreFromRF store_from_rf[32];
  int32_t store_rf_num = 0;
  int32_t loaded_rob_ind = -1, loaded_value = 0;
  bool predict_failed = false;
  int32_t rob_ind[32], value[32], size = 0;
  int32_t rob_tail = 0;

  LSBStructure lsb_structure;
public:
  LSB() = default;
  void Connect(Memory *memory, Decoder *decoder, ProgramCounter *pc);
  void SetFromDecoder(InstructionType type, int32_t address, int32_t cycle, int32_t rob_index, bool is_committed);
  void SetFromALU(InstructionType type, int32_t result, int32_t rob_ind);
  void SetStoreFromRF(int32_t address, int32_t value, int32_t rob_ind);
  void GetLoadedData(int32_t rob_ind, int32_t value);
  void PredictFailed();
  void GetROBTable(int32_t rob_ind[], int32_t value[], int32_t size);
  void PassROBTail(int32_t tail);
  void Update();
  void Run();
};
#endif