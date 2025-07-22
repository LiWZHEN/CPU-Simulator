#ifndef PROGRAM_COUNTER_HPP
#define PROGRAM_COUNTER_HPP

class ProgramCounter {
  int PC = 0;
public:
  ProgramCounter() = default;
  ProgramCounter(int pc) : PC(pc) {}
  int GetPC() const;
  void SetPC(int to);
  void NextPC();
};
#endif