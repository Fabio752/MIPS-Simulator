/*
 * Class that simulates the memory.
 * There are 4 main blocks:
 * - Instructions
 * - Data
 * - Input
 * - Output
 */

#ifndef MEMORY_H
#define MEMORY_H

#include "util.hpp"

#include <vector>

class Memory {
 public:
  Memory();

  // Place 4 bytes read from the binary file into the executable memory.
  void PlaceInstructionInMemory(uint32_t instruction);

  // Read the instruction at the specified address.
  // If the address is invalid, an error is thrown.
  uint32_t ReadInstruction(uint32_t address) const;

  // Write word at the specified address, or at ADDR_PUTC.
  // If the address is invalid, an error is thrown.
  void WriteDataWord(uint32_t address, int32_t word);

  // Read a word of data at the specified address, or at ADDR_GETC.
  // If the address is invalid, an error is thrown.
  int32_t ReadDataWord(uint32_t address);

  // Write byte at the specified address.
  // The byte is taken as a 32bits integer, and only the
  // least significant 8 are stored.
  // If the address is invalid, an error is thrown.
  void WriteDataByte(uint32_t address, int32_t byte);

  // Read a byte of data at the specified address.
  // If the address is invalid, an error is thrown.
  int32_t ReadDataByte(uint32_t address, bool signed_load);
 
  // Write half word at the specified address.
  // The half word is taken as a 32bits integer, and only the
  // least significant 16 are stored.
  // If the address is invalid, an error is thrown.
  void WriteHalfDataWord(uint32_t address, int32_t half_word);  

  // Read a half word of data at the specified address.
  // If the address is invalid, an error is thrown.
  int32_t ReadHalfDataWord(uint32_t address, bool signed_load);

  // Read left part of a word.
  // If the address is invalid, an error is thrown.
  int32_t ReadWordLeft(uint32_t address);

  // Read right part of a word.
  // If the address is invalid, an error is thrown.
  int32_t ReadWordRight(uint32_t address);

 private:
  // Types of memory Load.
  enum MemOp {WORD, HALF_WORD, BYTE, LWL, LWR};

  // Simulates the block of memory containing the instructions.
  // Starts at simulated address: ADDR_INSTR.
  // Max lenght: ADDR_INSTR_LEN, variable length instantiated on request.
  // Read and execute memory.
  std::vector<uint32_t> instructions_memory_;

  // Simulates the block of memory containing data.
  // Starts at simulated address: ADDR_DATA.
  // Max lenght: ADDR_DATA_LEN, always the max lenght.
  // Read and write memory.
  std::vector<int32_t> data_memory_;

  // Reset the memory to the initial state.
  void ClearMemory();

  // Checks if an address is valid for data read/write. Either bytes or words.
  void DieIfInvalidDataAddress(uint32_t address, MemOp mem_op, bool write_operation) const;

  // Checks if the address is within the data memory.
  bool PointsToDataMemory(uint32_t address) const;

  // Checks if the address is within the instruction memory.
  bool PointsToInstructionMemory(uint32_t address) const;

  // Write to stdout.
  void WriteOutput(int32_t word);

  // Read from stdin.
  int32_t ReadInput();
};

#endif
