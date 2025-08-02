#include "../include/CPU.hpp"

void CPU::Initialize() {
  memory.Connect(&lsb, &decoder, &rob, &rs);
  alu.Connect(&rob, &rs, &lsb, &pc);
  decoder.Connect(&rob, &rs, &lsb, &rf, &pc, &predictor);
  lsb.Connect(&memory, &decoder, &pc);
  rf.Connect(&decoder, &lsb);
  rob.Connect(&rs, &lsb, &decoder, &rf, &pc, &predictor, &alu);
  rs.Connect(&alu);
  pc.Connect(&memory, &decoder);

  dataloader.LoadData(memory);
}

void CPU::Update() {
  memory.Update();
  alu.Update();
  decoder.Update();
  lsb.Update();
  rf.Update();
  rob.Update();
  rs.Update();
  pc.Update();
}

void CPU::Step() {
  ++clock;
  if (clock > 1000) {
    exit(0);
  }

  Update();

  memory.Run();
  alu.Run();
  decoder.Run();
  lsb.Run();
  rf.Run();
  rob.Run();
  rs.Run();
  pc.Run();
}

void CPU::Print() {
  pc.Print();
  rf.Print();
  rob.Print();
  lsb.Print();
  rs.Print();
}

int32_t CPU::GetClock() {
  return clock;
}