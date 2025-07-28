#ifndef REGISTER_FILE_HPP
#define REGISTER_FILE_HPP

#include <cstdint>

class RegisterFile {
  int32_t old_reg[32] = {};
  int32_t new_reg[32] = {};
  int32_t old_dependence[32] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  int32_t new_dependence[32] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
public:
  RegisterFile() = default;
  void Modify(const int32_t ind, const int32_t x);
  int32_t GetData(const int32_t ind) const;
  int32_t GetDependence(const int32_t ind) const;
  void Update();
};
#endif