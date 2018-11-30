/*
 * Class with some utility functions and constants used by other classes.
 */

#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>
#include <cstdint>

// Global variables that simulates the lock for branches.
extern uint32_t __PC_before_branch;
extern bool __branch_taken;

struct ParsedInstruction {
  // Enumerate all the instructions types.
  int32_t instruction_type;

  uint32_t opcode;       // 6 bits.

  // R and I types specific.
  uint32_t src_reg_1;    // 5 bits.
  uint32_t dest_reg;     // 5 bits.

  // R type specific.
  uint32_t src_reg_2;    // 5 bits.
  uint32_t shift_amount; // 5 bits.
  uint32_t function;     // 6 bits.

  // I type specific.
  int32_t immediate;             // 16 bits.

  // J type specific.
  int32_t target;                // 26 bits.
};

class Util{
 public:
  const static bool DEBUG = 0;

  /// Memory location constants. ///

  // Jumping to this address means the Binary has finished execution.
  static const uint32_t ADDR_NULL = 0x00000000;

  // Executable memory. The Binary should be loaded here.
  static const uint32_t ADDR_INSTR = 0x10000000;
  static const uint32_t ADDR_INSTR_BYTES_LEN = 0x1000000;

  // Read-write data area. Should be zero-initialised.
  static const uint32_t ADDR_DATA = 0x20000000;
  static const uint32_t ADDR_DATA_BYTES_LEN = 0x4000000;

  // Location of memory mapped input. Read-only.
  static const uint32_t ADDR_GETC = 0x30000000;
  // Location of memory mapped output. Write-only.
  static const uint32_t ADDR_PUTC = 0x30000004;

  /// Error and exception constants. ///
  static const int32_t ARITHMETIC_EXCEPTION = -10;
  static const int32_t MEMORY_EXCEPTION = -11;
  static const int32_t INVALID_INSTRUCTION_EXCEPTION = -12;
  static const int32_t INTERNAL_ERROR = -20;
  static const int32_t IO_ERROR = -21;

  // Decode an 32 bits instruction.
  static ParsedInstruction ParseInstruction(uint32_t instruction);

  // Sign extensions.
  static void SignExtend8To32(uint32_t& value);
  static void SignExtend16To32(uint32_t& value);
  static void SignExtend16To32(int& value);
  static void SignExtend26To32(int& value);

  // Branch locks.
  static void SetBranchIsTaken(uint32_t PC_address);
  static bool CheckBranchLock();
  static uint32_t GetPCBeforeBranch();
  static void ReleaseBranchLock();
};

#endif
