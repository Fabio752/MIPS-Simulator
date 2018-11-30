#ifndef MIPS_SIMULATOR_H
#define MIPS_SIMULATOR_H

#include "instruction_set.hpp"
#include "util.hpp"

#include <string>

class MipsSimulator{
 public:
  // Load the binary to execute in the executable memory of the simulator.
  void LoadBinaryIntoMemory(const char* binary_path);

  // Simulate the execution of the binary and return the exit code in case of success.
  int SimulateBinary();

 private:
  Memory memory_;
  Context context_;

  // Checks wether the program counter reaches ADDR_NULL.
  bool ExecutionIsOver();
};

#endif
