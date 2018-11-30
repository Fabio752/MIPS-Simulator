#include "../include/mips_simulator.hpp"

#include <fstream>
#include <iostream>

/// Public methods. ///

void MipsSimulator::LoadBinaryIntoMemory(const char* binary_path) {
  std::ifstream binary_file(binary_path, std::ios::in | std::ios::binary);
  if (!binary_file.is_open()) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: Binary file not found: '" << binary_path << "'." << std::endl;
    }
    exit(Util::IO_ERROR);
  }
  if (Util::DEBUG) {
    std::cerr << std::endl;
    std::cerr << "###########################" << std::endl;
    std::cerr << "Writing binary into memory." << std::endl;
  }
  char instruction_bytes[4];
  // Each iteration of the loop reads 4 bytes of the file and put them in
  // the array of 4 chars (4 bytes).
  // The loop terminates when the file is over.
  while (binary_file.read(instruction_bytes, 4)) {
    uint32_t instruction = (uint32_t)((unsigned char)(instruction_bytes[0]) << 24 |
                                      (unsigned char)(instruction_bytes[1]) << 16 |
                                      (unsigned char)(instruction_bytes[2]) << 8 |
                                      (unsigned char)(instruction_bytes[3]));
    if (Util::DEBUG) {
      std::cerr << "> (hex): " << std::hex << instruction << std::dec << std::endl;
    }
    memory_.PlaceInstructionInMemory(instruction);
  }
  binary_file.close();
}

int MipsSimulator::SimulateBinary() {
  if (Util::DEBUG) {
    std::cerr << std::endl;
    std::cerr << "#############################" << std::endl;
    std::cerr << "Starting simulator execution." << std::endl;
  }

  // Initially, clear the branch lock.
  Util::ReleaseBranchLock();

  while(!ExecutionIsOver()) {
    uint32_t instruction = memory_.ReadInstruction(context_.GetPC());
    if (Util::DEBUG) {
      //std::cerr << "Press enter for next instruction..."; std::getchar();
      std::cerr << std::endl;
      std::cerr << "==> PC (hex):    " << std::hex << context_.GetPC() << std::dec << std::endl;
      std::cerr << "==> Instr (hex): " << std::hex << instruction << std::dec << std::endl;
    }
    ParsedInstruction parsed_instruction = Util::ParseInstruction(instruction);
    InstructionSet::ExecuteInstructionAndUpdatePC(parsed_instruction, context_, memory_);
    if (Util::CheckBranchLock()) {
      // Execute extra instruction before continuing as normal.
      uint32_t extra_instruction = memory_.ReadInstruction(Util::GetPCBeforeBranch());
      if (Util::DEBUG) {
        //std::cerr << "Press enter for next instruction..."; std::getchar();
        std::cerr << std::endl;
        std::cerr << "==**> EXTRA PC (hex):    " << std::hex << Util::GetPCBeforeBranch() << std::dec << std::endl;
        std::cerr << "==**> EXTRA Instr (hex): " << std::hex << extra_instruction << std::dec << std::endl;
      }
      ParsedInstruction parsed_extra_instruction = Util::ParseInstruction(extra_instruction);
      InstructionSet::ExecuteInstructionAndUpdatePC(parsed_extra_instruction, context_, memory_);
      Util::ReleaseBranchLock();
    }
  }

  return context_.GetRegister(2) & 0x000000FF;
}

/// Private methods. ///

bool MipsSimulator::ExecutionIsOver() {
  return context_.GetPC() == Util::ADDR_NULL;
}
