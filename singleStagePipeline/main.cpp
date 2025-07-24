#include <iostream>
#include "include/Exception.hpp"
#include "include/LoadData.hpp"
#include "include/Run.hpp"


int main() {
  Memory memory;
  DataLoader data_loader;
  Interpreter interpreter;
  data_loader.LoadData(memory);
  interpreter.Run(memory);
  return 0;
}