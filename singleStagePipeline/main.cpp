#include "include/LoadData.hpp"
#include "include/Run.hpp"
#include <iostream>

int main() {
  Memory memory;
  DataLoader data_loader;
  Interpreter interpreter;
  data_loader.LoadData(memory);
  interpreter.Run(memory);
  return 0;
}