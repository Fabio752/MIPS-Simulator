#include "../include/context.hpp"

#include <algorithm>
#include <iostream>

/// Public methods. ///

Context::Context() {
  // The instuctions of the binary starts at ADDR_INSTR.
  pc_ = Util::ADDR_INSTR;
  // 32 registers initially filled with zero.
  register_file_.resize(32);
  ResetRegisters();
  hi_ = 0;
  lo_ = 0;
}

int32_t Context::GetRegister(int32_t register_id) const {
  if (!IsValidRegisterId(register_id)) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: trying to get an invalid register: " << register_id << "." << std::endl;
    }
    exit(Util::INTERNAL_ERROR);
  }
  return register_file_[register_id];
}

void Context::SetRegister(int32_t register_id, int32_t value) {
  if (!IsValidRegisterId(register_id)) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: trying to set an invalid register: " << register_id << "." << std::endl;
    }
    exit(Util::INTERNAL_ERROR);
  }
  if (register_id == 0) {
    if (Util::DEBUG) {
      std::cerr << "WARNING: trying to set $0. Ignoring." << std::endl;
    }
    return;
  }
  register_file_[register_id] = value;
}

void Context::SetHi(int32_t value) {
  hi_ = value;
}

void Context::SetLo(int32_t value) {
  lo_ = value;
}
int32_t Context::GetHi() const {
  return hi_;
}

int32_t Context::GetLo() const {
  return lo_;
}

uint32_t Context::GetPC() const {
  return pc_;
}

void Context::SetPC(uint32_t pc_value) {
  //MAYBE IT CAN BE FOR JR////////////
  if (pc_value % 4 != 0) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: Register value not multiple of 4:  " << std::dec
                << pc_value << std::endl;
    }
    exit(Util::MEMORY_EXCEPTION);
  }
  if ((pc_value < Util::ADDR_INSTR || pc_value >= Util::ADDR_INSTR + Util::ADDR_INSTR_BYTES_LEN) &&
       pc_value != 0) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: program counter jumped outside instruction memory limits: "
                << pc_value << "." << std::endl;
    }
    exit(Util::MEMORY_EXCEPTION);
  }
  pc_ = pc_value;
}

void Context::IncrementPC() {
  if (Util::CheckBranchLock()) {
    return;
  }
  pc_ += 4;
  if (pc_ >= Util::ADDR_INSTR + Util::ADDR_INSTR_BYTES_LEN) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: program counter incremented above instruction memory limit: "
                << pc_ << "." << std::endl;
    }
    exit(Util::MEMORY_EXCEPTION);
  }
}

void Context::PrintRegisterValue(int32_t register_id) const {
  if (Util::DEBUG) {
    std::cerr << "> Reg " << register_id << " has value: "
              << GetRegister(register_id) << std::endl;
  }
}

/// Private methods. ///

void Context::ResetRegisters() {
  std::fill(register_file_.begin(), register_file_.end(), 0);
}

bool Context::IsValidRegisterId(int32_t register_id) const {
  return (0 <= register_id) && (register_id < 32);
}