#include "include/LoadData.hpp"
#include "include/Run.hpp"
#include <iostream>

int main() {
  freopen("record.txt", "w", stderr);
  freopen("../testcases/array_test1.data", "r", stdin);
  Memory memory;
  DataLoader data_loader;
  Interpreter interpreter;
  data_loader.LoadData(memory);
  interpreter.Run(memory);
  return 0;
}