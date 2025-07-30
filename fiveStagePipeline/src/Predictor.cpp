#include "../include/Predictor.hpp"

void Predictor::SubmitResult(int32_t pc, bool should_jump) {
  if (strategy.count(pc) == 0) {
    strategy[pc] = should_jump ? jump_history(1, 0) : jump_history(0, 1);
  } else if (should_jump) {
    ++strategy[pc].jump;
  } else {
    ++strategy[pc].not_jump;
  }
}

bool Predictor::QueryJump(int32_t pc) {
  if (strategy.count(pc) == 0) {
    return false;
  }
  if (strategy[pc].jump > strategy[pc].not_jump) {
    return true;
  }
  return false;
}