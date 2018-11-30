/*
 * Class that simulates the Context of the CPU.
 * Handles:
 * - program counter: pc_
 * - registers: registers_
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "util.hpp"

#include <vector>

class Context{
 public:
  Context();

  // Set register to a value, if register_id is valid.
  void SetRegister(int32_t register_id, int32_t value);
  // Get the value of a register, if register_id is valid.
  int32_t GetRegister(int32_t register_id) const;
  
  // Getters and Setters for hi and lo.
  void SetHi(int32_t value);
  void SetLo(int32_t value);
  int32_t GetHi() const;
  int32_t GetLo() const;

  // Get the value of the program counter.
  uint32_t GetPC() const;
  // Set the value of the program counter, checking if it is in the instruction memory.
  void SetPC(uint32_t pc_value);
  
  // Increment PC by 4 bytes and checks if it goes out of range.
  void IncrementPC();

  // Debug functions.
  void PrintRegisterValue(int32_t register_id) const;

 private:
  uint32_t pc_;
  // General purpose registers.
  std::vector<int32_t> register_file_;
  // Special registers for arithmetic.
  int32_t lo_;
  int32_t hi_;

  // Set all registers to 0.
  void ResetRegisters();

  // Checks 0 <= register_id < 32.
  bool IsValidRegisterId(int32_t register_id) const;
};

#endif
