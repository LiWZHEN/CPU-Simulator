#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include "ProgramCounter.hpp"
#include <unordered_map>

struct jump_history {
  int32_t jump = 0;
  int32_t not_jump = 0;
  jump_history() = default;
  jump_history(int32_t jump, int32_t not_jump) {
    this->jump = jump;
    this->not_jump = not_jump;
  }
};

class Predictor {
  std::unordered_map<int32_t, jump_history> strategy;
public:
  Predictor() = default;
  void SubmitResult(int32_t pc, bool should_jump);
  bool QueryJump(int32_t pc);
};
#endif