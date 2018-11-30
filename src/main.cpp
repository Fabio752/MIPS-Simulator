#include <iostream>

#include "../include/mips_simulator.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: wrong number of paramters." << std::endl
                << "Syntax: 'bin/mips_simulator path/to/binary/to/execute'." << std::endl;
    }
    return Util::IO_ERROR;
  }
  const char* binary_path = argv[1];

  if (Util::DEBUG) {
    std::cerr << "Running in debug mode." << std::endl;
  }

  MipsSimulator mips_simulator;
  mips_simulator.LoadBinaryIntoMemory(binary_path);
  int exit_code = mips_simulator.SimulateBinary();

  if (Util::DEBUG) {
    std::cerr << std::endl << "++++++++++++++ SUCCESS ++++++++++++++" << std::endl;
  }

  return exit_code;
}