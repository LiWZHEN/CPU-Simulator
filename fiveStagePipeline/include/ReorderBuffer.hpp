#ifndef REORDER_BUFFER_HPP
#define REORDER_BUFFER_HPP

#include "ReservationStation.hpp"
#include "LoadStoreBuffer.hpp"
#include "Decoder.hpp"
#include "ProgramCounter.hpp"
#include "RegisterFile.hpp"
#include "Predictor.hpp"
#include "ArithmeticLogicUnit.hpp"

struct ROBEntry {
  InstructionType type = InstructionType::NONE;
  int32_t value;
  int32_t rd;
  bool is_ready = false;
};

struct ROBStructure {
  int32_t size = 0;
  int32_t head = 0;
  int32_t tail = 0;
  ROBEntry rob_entries[32];
};

struct ROBTask {
  bool halted = false;

  bool predict_failed = false;
  InstructionType alu_compute_type = InstructionType::NONE;
  int32_t alu_given_value = 0;
  int32_t alu_given_dependence = -1;
  InstructionType decoder_submit_type = InstructionType::NONE;
  int32_t decoder_given_rd = -1;
  int32_t decoder_given_value = -1;
  bool decoder_given_ready = false;
  int32_t load_rob_ind = -1, load_value = 0;
};

class ROB {
  RS *rs = nullptr;
  LSB *lsb = nullptr;
  Decoder *decoder = nullptr;
  RegisterFile *rf = nullptr;
  ProgramCounter *pc = nullptr;
  Predictor *predictor = nullptr;
  ALU *alu = nullptr;

  ROBTask task;

  bool halted = false;

  bool predict_failed = false;
  InstructionType alu_compute_type = InstructionType::NONE;
  int32_t alu_given_value = 0;
  int32_t alu_given_dependence = -1;
  InstructionType decoder_submit_type = InstructionType::NONE;
  int32_t decoder_given_rd = -1;
  int32_t decoder_given_value = -1;
  bool decoder_given_ready = false;
  int32_t load_rob_ind = -1, load_value = 0;

  ROBStructure rob_structure;
public:
  ROB() = default;
  void Connect(RS *rs, LSB *lsb, Decoder *decoder, RegisterFile *rf, ProgramCounter *pc, Predictor *predictor, ALU *alu);
  void Update();
  void SetPredictFault();
  void SetFromALU(InstructionType operation, int32_t value, int32_t rob_index);
  void SetFromDecoder(InstructionType instruction_type, int32_t rd, int32_t value, bool is_ready);
  void GetLoadedData(int32_t rob_ind, int32_t value);
  void Run();
  void Halt();
};
#endif