#ifndef DECODER_HPP
#define DECODER_HPP

#include "Classes.hpp"
#include <cstdint>
#include <string>
#include "ReorderBuffer.hpp"
#include "ReservationStation.hpp"
#include "LoadStoreBuffer.hpp"
#include "RegisterFile.hpp"
#include "Predictor.hpp"

struct CommitMessage {
  int32_t rob_ind;
  int32_t value;
};

struct DecoderTask {
  int32_t machine_code;
  bool predict_failed = false;
  bool rob_is_full = false;
  int32_t rob_tail = 0;
  int32_t rf_data[32];
  int32_t rf_dependence[32];
  CommitMessage commit_message[32];
  int32_t commit_message_len = 0;
  int32_t current_pc = -1;
  bool discard_this = false;
};

class Decoder {
  ROB *rob = nullptr;
  RS *rs = nullptr;
  LSB *lsb = nullptr;
  RegisterFile *rf = nullptr;
  ProgramCounter *pc = nullptr;
  Predictor *predictor;

  int32_t machine_code = 0;
  bool predict_falied = false;
  bool rob_is_full = false;
  int32_t rob_tail = 0;
  int32_t rf_data[32];
  int32_t rf_dependence[32];
  CommitMessage commit_message[32];
  int32_t commit_message_len = 0;
  int32_t current_pc = 0;

  DecoderTask task;

  void Decode_R();
  void Decode_IA();
  void Decode_IM();
  void Decode_IC();
  void Decode_S();
  void Decode_B();
  void Decode_J();
  void Decode_AUIPC();
  void Decode_LUI();
public:
  Decoder() = default;
  void Connect(ROB *rob, RS *rs, LSB *lsb, RegisterFile *rf, ProgramCounter *pc, Predictor *predictor);
  void Update();
  void SetFromPC(int32_t machine_code);
  void SetCurrentPC(int32_t current_pc);
  void PredictFailed();
  void CommitMessageFromROB(int32_t rob_ind, int32_t value);
  void PassRobTail(int32_t tail);
  void PassRF(int32_t rf_data[], int32_t rf_dependence[]);
  void ROBFull();
  void Run();
};

std::string IntToString(uint32_t x);
uint32_t StringToInt(const std::string &str);

#endif