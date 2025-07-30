#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include "Memory.hpp"
#include "ArithmeticLogicUnit.hpp"
#include "Decoder.hpp"
#include "LoadData.hpp"
#include "LoadStoreBuffer.hpp"
#include "Predictor.hpp"
#include "ProgramCounter.hpp"
#include "RegisterFile.hpp"
#include "ReorderBuffer.hpp"
#include "ReservationStation.hpp"

class CPU {
  int32_t clock = 0;
  DataLoader dataloader;
  Memory memory;
  ALU alu;
  Decoder decoder;
  LSB lsb;
  Predictor predictor;
  RegisterFile rf;
  ROB rob;
  RS rs;
  ProgramCounter pc;
public:
  CPU() = default;
  void Initialize();
  void Update();
  void Step();
  void Halt();
};

#endif