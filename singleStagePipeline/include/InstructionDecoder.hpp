#ifndef INSTRUCTION_DECODER_HPP
#define INSTRUCTION_DECODER_HPP

#include <string>
#include <cstdint>

class InstructionDecoder {

public:
  InstructionDecoder() = default;
  std::string Decode(const int32_t code);
  std::string Decode_R(const int32_t code);
  std::string Decode_IA(const int32_t code);
  std::string Decode_IM(const int32_t code);
  std::string Decode_IC(const int32_t code);
  std::string Decode_IO(const int32_t code);
  std::string Decode_S(const int32_t code);
  std::string Decode_B(const int32_t code);
  std::string Decode_J(const int32_t code);
  std::string Decode_AUIPC(const int32_t code);
  std::string Decode_LUI(const int32_t code);
};


std::string IntToString(int32_t x, int8_t len);
int32_t StringToInt(const std::string &str);

#endif