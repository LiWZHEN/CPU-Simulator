#include "../include/RegisterFile.hpp"

void RegisterFile::Modify(const int32_t ind, const int32_t x) {
  new_reg[ind] = x;
  if (ind == 0) {
    new_reg[0] = 0;
  }
}

int32_t RegisterFile::GetData(const int32_t ind) const {
  return old_reg[ind];
}

int32_t RegisterFile::GetDependence(const int32_t ind) const {
  return old_dependence[ind];
}

void RegisterFile::Update() {
  for (int i = 0; i < 32; ++i) {
    old_reg[i] = new_reg[i];
    old_dependence[i] = new_dependence[i];
  }
}