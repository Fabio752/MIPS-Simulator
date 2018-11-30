#include "../include/memory.hpp"

#include <iostream>
#include <cstdio>

/// Public methods. ///

Memory::Memory() {
  ClearMemory();
}

void Memory::PlaceInstructionInMemory(uint32_t instruction) {
  if (instructions_memory_.size() >= Util::ADDR_INSTR_BYTES_LEN / 4) {
    if (Util::DEBUG) {
      std::cerr << "WARNING: Trying to place an instruction after max size for instructions is reached. Doing nothing."
                << std::endl;
    }
    return;
  }
  instructions_memory_.push_back(instruction);
}

uint32_t Memory::ReadInstruction(uint32_t address) const {
  if (!PointsToInstructionMemory(address)) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: Trying read an instruction outside the instructions space in memory (hex): "
                << std::hex << address << std::dec << "." << std::endl;
    }
    exit(Util::MEMORY_EXCEPTION);
  }
  if (address % 4 != 0) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: Trying read an instruction but address is not multiple of 4 (hex): "
                << std::hex << address << std::dec << "." << std::endl;
    }
    exit(Util::MEMORY_EXCEPTION);
  }

  // Offset from the beginning of the instruction memory, in number of words. 
  uint32_t offset = (address - Util::ADDR_INSTR) / 4;
  if (offset >= instructions_memory_.size()) {
    // If within memory limit, but the instruction is not defined, return empty instruction.
    if (Util::DEBUG) {
      std::cerr << "WARNING: Trying to read an instruction outside the defined ones. Returning zero."
                << std::endl;
    }
    return 0;
  }

  uint32_t instruction = instructions_memory_.at(offset);
  return instruction;
}

void Memory::WriteDataWord(uint32_t address, int32_t word) {
  if (address == Util::ADDR_PUTC) {
    WriteOutput(word);
    return;
  }

  DieIfInvalidDataAddress(address, WORD, /*write_operation=*/true);

  // Offset from the beginning of the data memory, in number of words. 
  uint32_t offset = (address - Util::ADDR_DATA) / 4;
  data_memory_[offset] = word;
}

int32_t Memory::ReadDataWord(uint32_t address) {
  if (address == Util::ADDR_GETC) {
    return ReadInput();
  }

  DieIfInvalidDataAddress(address, WORD, /*write_operation=*/false);

  int32_t word;
  if (PointsToDataMemory(address)) {
    // Offset from the beginning of the data memory, in number of words. 
    uint32_t offset = (address - Util::ADDR_DATA) / 4;
    word = data_memory_.at(offset);
  } else if (PointsToInstructionMemory(address)) {
    // Offset from the beginning of the instruction memory, in number of words. 
    uint32_t offset = (address - Util::ADDR_INSTR) / 4;
    word = offset < instructions_memory_.size()
           ? instructions_memory_.at(offset)
           : 0;
  }
  return word;
}

void Memory::WriteDataByte(uint32_t address, int32_t byte) {
  // Extract 8 least significant bits to actually have a byte.
  byte &= 0x000000ff;

  if (address == Util::ADDR_PUTC + 3) {
    WriteOutput(byte);
    return;
  } else if (address >= Util::ADDR_PUTC && address < Util::ADDR_PUTC + 3) {
    WriteOutput(0);
    return;
  }

  DieIfInvalidDataAddress(address, BYTE, /*write_operation=*/true);

  // Offset from the beginning of the data memory, in number of words. 
  uint32_t offset = (address - Util::ADDR_DATA) / 4;
  uint32_t offset_in_word = address % 4;
  uint32_t shifted_byte = byte << (3 - offset_in_word) * 8;  // 8 bits in a byte.
  uint32_t clearing_mask = 0x000000ff << (3 - offset_in_word) * 8; // 8 bits in a byte.

  data_memory_[offset] &= (~clearing_mask);
  data_memory_[offset] |= shifted_byte;
}

int32_t Memory::ReadDataByte(uint32_t address, bool signed_load) {
  if (address == Util::ADDR_GETC + 3) {
    return ReadInput();
  } else if (address >= Util::ADDR_GETC && address < Util::ADDR_GETC + 3) {
    if (std::getchar() == EOF) {
      return signed_load ? EOF : 0x000000FF;
    } else {
      // Waste a char.
      return 0;
    }
  }

  DieIfInvalidDataAddress(address, BYTE, /*write_operation=*/false);

  uint32_t offset_in_word = address % 4;
  uint32_t clearing_mask = 0x000000ff << (3 - offset_in_word) * 8; // 8 bits in a byte.

  uint32_t word;
  if (PointsToDataMemory(address)) {
    // Offset from the beginning of the data memory, in number of words. 
    uint32_t offset = (address - Util::ADDR_DATA) / 4;
    word = data_memory_[offset];
  } else if (PointsToInstructionMemory(address)) {
    // Offset from the beginning of the instruction memory, in number of words. 
    uint32_t offset = (address - Util::ADDR_INSTR) / 4;
    word = offset < instructions_memory_.size()
           ? instructions_memory_.at(offset)
           : 0;
  }

  word &= clearing_mask;
  word = word >> (3 - offset_in_word) * 8;

  if (signed_load) {
    Util::SignExtend8To32(word);
  }
  return word;
}

void Memory::WriteHalfDataWord(uint32_t address, int32_t half_word) {
  // Extract the 16 least significant bits to actually have half word.
  half_word &= 0x0000ffff;

  if (address == Util::ADDR_PUTC + 2) {
    WriteOutput(half_word);
    return;
  } else if (address == Util::ADDR_PUTC) {
    WriteOutput(0);
    return;
  }

  DieIfInvalidDataAddress(address, HALF_WORD, /*write_operation=*/true);

  // Offset from the beginning of the data memory, in number of words. 
  uint32_t offset = (address - Util::ADDR_DATA) / 4;
  uint32_t offset_in_word = address % 4;
  uint32_t shifted_half_word = half_word << (2 - offset_in_word) * 8; // 8 bits in a byte.
  uint32_t clearing_mask = 0x0000ffff << (2 - offset_in_word) * 8;    // 8 bits in a byte.

  data_memory_[offset] &= (~clearing_mask);
  data_memory_[offset] |= shifted_half_word;
}

int32_t Memory::ReadHalfDataWord(uint32_t address, bool signed_load) {
  if (address == Util::ADDR_GETC + 2) {
    return ReadInput();
  } else if (address == Util::ADDR_GETC) {
    if (std::getchar() == EOF) {
      return signed_load ? EOF : 0x0000ffff;
    } else {
      // Waste a char.
      return 0;
    }
  }

  DieIfInvalidDataAddress(address, HALF_WORD, /*write_operation=*/false);

  uint32_t offset_in_word = address % 4;
  uint32_t clearing_mask = 0x0000ffff << (2 - offset_in_word) * 8; // 8 bits in a byte.

  uint32_t word;
  if (PointsToDataMemory(address)) {
    // Offset from the beginning of the data memory, in number of words. 
    uint32_t offset = (address - Util::ADDR_DATA) / 4;
    word = data_memory_[offset];
  } else if (PointsToInstructionMemory(address)) {
    // Offset from the beginning of the instruction memory, in number of words. 
    uint32_t offset = (address - Util::ADDR_INSTR) / 4;
    word = offset < instructions_memory_.size()
           ? instructions_memory_.at(offset)
           : 0;
  }

  word &= clearing_mask;
  word = word >> (2 - offset_in_word) * 8;

  if (signed_load) {
    Util::SignExtend16To32(word);
  }
  return word;
}

int32_t Memory::ReadWordLeft(uint32_t address) {
  int32_t complete_word;
  if (address >= Util::ADDR_GETC && address < Util::ADDR_GETC + 4) {
    complete_word = ReadInput();
  } else {
    DieIfInvalidDataAddress(address, LWL, /*write_operation=*/false);
  }

  uint32_t byte_offset = address % 4;

  if (PointsToDataMemory(address)) {
    // Offset from the beginning of the data memory, in number of words. 
    uint32_t offset = (address - Util::ADDR_DATA) / 4;
    complete_word = data_memory_[offset];
  } else if (PointsToInstructionMemory(address)) {
    // Offset from the beginning of the instruction memory, in number of words. 
    uint32_t offset = (address - Util::ADDR_INSTR) / 4;
    complete_word = offset < instructions_memory_.size()
           ? instructions_memory_.at(offset)
           : 0;
  }

  // Get only the (4 - byte_offset) least significant bytes from the complete word.
  int32_t result = 0;
  for (int i = 0; i < 4 - byte_offset; i++) {
    uint32_t byte = complete_word & (0x000000ff << (i * 8));
    result = result | byte;
  }

  // Shift the inserted bytes to the left: most significant bytes.
  result = result << (byte_offset * 8);
  return result;
}

int32_t Memory::ReadWordRight(uint32_t address) {
  if (address == Util::ADDR_GETC + 3) {
    return ReadInput();
  } else if (address >= Util::ADDR_GETC && address < Util::ADDR_GETC + 3) {
    if (std::getchar() == EOF) {
      return EOF;
    } else {
      // Waste a char.
      return 0;
    }
  }

  DieIfInvalidDataAddress(address, LWR, /*write_operation=*/false);

  int32_t byte_offset = address % 4;

  int32_t complete_word;
  if (PointsToDataMemory(address)) {
    // Offset from the beginning of the data memory, in number of words. 
    uint32_t offset = (address - Util::ADDR_DATA) / 4;
    complete_word = data_memory_[offset];
  } else if (PointsToInstructionMemory(address)) {
    // Offset from the beginning of the instruction memory, in number of words. 
    uint32_t offset = (address - Util::ADDR_INSTR) / 4;
    complete_word = offset < instructions_memory_.size()
           ? instructions_memory_.at(offset)
           : 0;
  }

  // Get only the (byte_offset + 1) most significant bytes from the complete word.
  uint32_t result = 0;

  for (int32_t i = 3; i >= 3 - byte_offset; i--) {
    uint32_t byte = complete_word & ((uint32_t)0xff000000 >> ((3 - i) * 8));
    result = result | byte;
  }

  // Shift the inserted bytes to the right: least significant bytes.
  result = result >> ((3 - byte_offset) * 8);

  return result;
}

/// Private methods. ///

void Memory::ClearMemory() {
  instructions_memory_.clear();
  data_memory_ = std::vector<int32_t>(Util::ADDR_DATA_BYTES_LEN / 4, 0);
}

void Memory::DieIfInvalidDataAddress(uint32_t address, MemOp mem_op, bool write_operation) const {
  // Write operation, can only access Data memory.
  if (write_operation && !PointsToDataMemory(address)) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: Trying to write data outside the data memory (hex): "
                << std::hex << address << std::dec << "." << std::endl;
    }
    exit(Util::MEMORY_EXCEPTION);
  }
  // Read operation, can access Data and Instruction memory.
  if (!write_operation &&
      (!PointsToDataMemory(address) && !PointsToInstructionMemory(address))) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: Trying to read data outside both data and instruction memory (hex): "
                << std::hex << address << std::dec << "." << std::endl;
    }
    exit(Util::MEMORY_EXCEPTION);
  }

  // Check if the memory address is correctly aligned.
  if (mem_op == WORD && address % 4 != 0) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: Trying to read/write data word but address is not multiple of 4 (hex): "
                << std::hex << address << std::dec << "." << std::endl;
    }
    exit(Util::MEMORY_EXCEPTION);
  } else if (mem_op == HALF_WORD && address % 2 != 0) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: Trying to read/write data half word but address is not multiple of 2 (hex): "
                << std::hex << address << std::dec << "." << std::endl;
    }
    exit(Util::MEMORY_EXCEPTION);
  }
}

bool Memory::PointsToDataMemory(uint32_t address) const {
  return address >= Util::ADDR_DATA && address < Util::ADDR_DATA + Util::ADDR_DATA_BYTES_LEN;
}

bool Memory::PointsToInstructionMemory(uint32_t address) const {
  return address >= Util::ADDR_INSTR && address < Util::ADDR_INSTR + Util::ADDR_INSTR_BYTES_LEN;
}

void Memory::WriteOutput(int32_t value) {
  // Putchar automatically cast value to unsigned char, so only takes the 8 LSBs.
  int32_t ret_code = std::putchar(value);
  if (Util::DEBUG) {
    std::cerr << std::endl << "> Writing to ADDR_PUTC (dec): " << (uint32_t)(value & 0x000000ff) << std::endl;
  }
  if (ferror(stdout)) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: fail I/O operation with putchar.";
    }
    exit(Util::IO_ERROR);
  }
}

int32_t Memory::ReadInput() {
  int32_t input = std::getchar();
  if (Util::DEBUG) {
    std::cerr << "> Reading from ADDR_GETC (dec): " << input << std::endl;
  }
  if (ferror(stdout)) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: fail I/O operation with getchar.";
    }
    exit(Util::IO_ERROR);
  }
  return input;
}