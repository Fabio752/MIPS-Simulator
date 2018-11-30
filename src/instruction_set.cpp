#include "../include/instruction_set.hpp"
#include <iostream>

/// Public methods. ///

void InstructionSet::ExecuteInstructionAndUpdatePC(const ParsedInstruction& instruction,
                                                   Context& context,
                                                   Memory& memory) {
  switch(instruction.instruction_type) {
    case 0:
      Add(instruction, context);
      break;
    case 1: 
      Addi(instruction, context);
      break;
    case 2:
      Addiu(instruction, context);
      break;
    case 3:
      Addu(instruction, context);
      break;
    case 4:
      And(instruction, context);
      break;
    case 5:
      Andi(instruction, context);
      break;
    case 6:
      Beq(instruction, context);
      break;
    case 7:
      Bgez(instruction, context);
      break;
    case 8:
      Bgezal(instruction, context);
      break;
    case 9:
      Bgtz(instruction, context);
      break;
    case 10:
      Blez(instruction, context);
      break;
    case 11:
      Bltz(instruction, context);
      break;
    case 12:
      Bltzal(instruction, context);
      break;
    case 13:
      Bne(instruction, context);
      break;
    case 14:
      Div(instruction, context);
      break;
    case 15:
      Divu(instruction, context);
      break;
    case 16:
      J(instruction, context);
      break;
    case 17:
      Jalr(instruction, context);
      break;
    case 18:
      Jal(instruction, context);
      break;
    case 19:
      Jr(instruction, context);
      break;
    case 20:
      Lb(instruction, context, memory);
      break;
    case 21:
      Lbu(instruction, context, memory);
      break;
    case 22:
      Lh(instruction, context, memory);
      break;
    case 23:
      Lhu(instruction, context, memory);
      break;
    case 24:
      Lui(instruction, context);
      break;
    case 25:
      Lw(instruction, context, memory);
      break;
    case 26:
      Lwl(instruction, context, memory);
      break;
    case 27:
      Lwr(instruction, context, memory);
      break;
    case 28:
      Mfhi(instruction, context);
      break;
    case 29:
      Mflo(instruction, context);
      break;
    case 30:
      Mthi(instruction, context);
      break;
    case 31:
      Mtlo(instruction, context);
      break;
    case 32:
      Mult(instruction, context);
      break;
    case 33:
      Multu(instruction, context);
      break;
    case 34:
      Or(instruction, context);
      break;
    case 35:
      Ori(instruction, context);
      break;
    case 36:
      Sb(instruction, context, memory);
      break;
    case 37:
      Sh(instruction, context, memory);
      break;
    case 38:
      Sll(instruction, context);
      break;
    case 39:
      Sllv(instruction, context);
      break;
    case 40:
      Slt(instruction, context);
      break;
    case 41:
      Slti(instruction, context);
      break;
    case 42:
      Sltiu(instruction, context);
      break;
    case 43:
      Sltu(instruction, context);
      break;
    case 44:
      Sra(instruction, context);
      break;
    case 45:
      Srav(instruction, context);
      break;
    case 46:
      Srl(instruction, context);
      break;
    case 47:
      Srlv(instruction, context);
      break;
    case 48:
      Sub(instruction, context);
      break;
    case 49:
      Subu(instruction, context);
      break;
    case 50:
      Sw(instruction, context, memory);
      break;
    case 51:
      Xor(instruction, context);
      break;
    case 52:
      Xori(instruction, context);
      break;

    default:
      if (Util::DEBUG) {
        std::cerr << "ERROR: instruction with instruction_type " << std::dec << instruction.instruction_type
                  << " does not exist or is not yet implemented." << std::endl;
      }
      exit(Util::INTERNAL_ERROR);
  }
}

/// Private methods. ///

// Add with overflow. CODE 0.
void InstructionSet::Add(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Add" << std::endl;
  }
  int32_t result = context.GetRegister(instruction.src_reg_1) +
               context.GetRegister(instruction.src_reg_2);

  // Overflow check.
  // If value in register positive.
  if ((context.GetRegister(instruction.src_reg_1) > 0) && (context.GetRegister(instruction.src_reg_2) > 0)) {
    if (result <= 0) {
      if (Util::DEBUG) {
        std::cerr << "ERROR: Positive Overflow, src_reg_1 value: " << std::dec << context.GetRegister(instruction.src_reg_1)
                  << " src_reg_2 value: " << context.GetRegister(instruction.src_reg_2) << std::endl;
      }
      exit(Util::ARITHMETIC_EXCEPTION);
    }
  }
  // If value in register negative.
  if ((context.GetRegister(instruction.src_reg_1) < 0) && (context.GetRegister(instruction.src_reg_2) < 0)) {
    if (result >= 0) {
      if (Util::DEBUG) {
        std::cerr << "ERROR: Negative Overflow, src_reg_1 :  " << std::dec << context.GetRegister(instruction.src_reg_1)
                  << " src_reg_2 value: " << context.GetRegister(instruction.src_reg_1) << std::endl;
      }
      exit(Util::ARITHMETIC_EXCEPTION);
    }

  }
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Add immediate with overflow. CODE 1.
void InstructionSet::Addi(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Addi" << std::endl;
  }
  int32_t result = context.GetRegister(instruction.src_reg_1) +
               instruction.immediate;
  
  // Overflow check.
  // If value in register positive.
   if ((context.GetRegister(instruction.src_reg_1) > 0) && (instruction.immediate > 0)) {
    if (result <= 0) {
      if (Util::DEBUG) {
        std::cerr << "ERROR: Positive Overflow, src_reg_1 value: " << std::dec << context.GetRegister(instruction.src_reg_1)
                  << " immediate value: " << instruction.immediate << std::endl;
      }
      exit(Util::ARITHMETIC_EXCEPTION);
    }

  }
  // If value in register negative.
  else if ((context.GetRegister(instruction.src_reg_1) < 0) && (instruction.immediate < 0)) {
    if (result >= 0) {
      if (Util::DEBUG) {
        std::cerr << "ERROR: Negative Overflow, src_reg_1 value:  " << std::dec << context.GetRegister(instruction.src_reg_1)
                  << " immediate value: " << instruction.immediate << std::endl;
      }
      exit(Util::ARITHMETIC_EXCEPTION);
    }
  }
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Add immediate unsigned (no overflow). CODE 2.
void InstructionSet::Addiu(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Addiu" << std::endl;
  }
  ////////////TODO: ASK IF IMMEDIATE HAS TO BE SIGN EXTENDED/////////////////
  uint32_t result = (uint32_t)context.GetRegister(instruction.src_reg_1) +
                        /*(*/(uint32_t)instruction.immediate /*& 0x0000FFFF)*/;  
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Add unsigned (no overflow). CODE 3.
void InstructionSet::Addu(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Addu" << std::endl;
  }
  uint32_t result = (uint32_t)context.GetRegister(instruction.src_reg_1) +
                        (uint32_t)context.GetRegister(instruction.src_reg_2);
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Bitwise and. CODE 4.
void InstructionSet::And(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> And" << std::endl;
  }
  int32_t result = context.GetRegister(instruction.src_reg_1) &
               context.GetRegister(instruction.src_reg_2);
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Bitwise and immediate. CODE 5.
void InstructionSet::Andi(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Andi" << std::endl;
  }
  int32_t result = context.GetRegister(instruction.src_reg_1) &
               (instruction.immediate & 0xFFFF);
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Branch on equal. CODE 6.
void InstructionSet::Beq(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Beq" << std::endl;
  }
  context.IncrementPC();
  if (context.GetRegister(instruction.src_reg_1) == context.GetRegister(instruction.dest_reg)) {
    Util::SetBranchIsTaken(context.GetPC());
    int32_t immediate = instruction.immediate << 2;
    int32_t pc_value = context.GetPC();
    context.SetPC(pc_value + immediate);
  }
}

// Branch on greater than or equal to zero. CODE 7.
void InstructionSet::Bgez(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Bgez" << std::endl;
  }
  context.IncrementPC();
  if (context.GetRegister(instruction.src_reg_1) >= 0) {
    Util::SetBranchIsTaken(context.GetPC());
    context.SetPC(context.GetPC() + (instruction.immediate << 2));
  }
}

// Branch on non-negative and link. CODE 8.
void InstructionSet::Bgezal(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Bgezal" << std::endl;
  }
  context.IncrementPC();
  context.SetRegister(31, context.GetPC() + 4);
  
  if (context.GetRegister(instruction.src_reg_1) >= 0) { 
    Util::SetBranchIsTaken(context.GetPC());
    context.SetPC(context.GetPC() + (instruction.immediate << 2));
  }
}

// Branch on greater than zero. CODE 9.
void InstructionSet::Bgtz(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Bgtz" << std::endl;
  }
  context.IncrementPC();
  
  if (context.GetRegister(instruction.src_reg_1) > 0) {
    Util::SetBranchIsTaken(context.GetPC());
    context.SetPC(context.GetPC() + (instruction.immediate << 2));
  }
}

// Branch on less than or equal to zero. CODE 10.
void InstructionSet::Blez(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Blez" << std::endl;
  }
  context.IncrementPC();
  
  if (context.GetRegister(instruction.src_reg_1) <= 0) {
    Util::SetBranchIsTaken(context.GetPC());
    context.SetPC(context.GetPC() + (instruction.immediate << 2));
  }
}

// Branch on less than zero. CODE 11.
void InstructionSet::Bltz(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Bltz" << std::endl;
  }
  context.IncrementPC();
  
  if (context.GetRegister(instruction.src_reg_1) < 0) {
    Util::SetBranchIsTaken(context.GetPC());
    context.SetPC(context.GetPC() + (instruction.immediate << 2));
  }
}
// Branch on less than zero and link. CODE 12.
void InstructionSet::Bltzal(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Bltzal" << std::endl;
  }
  context.IncrementPC();
  context.SetRegister(31, context.GetPC() + 4);
  
  if (context.GetRegister(instruction.src_reg_1) < 0) {  
    Util::SetBranchIsTaken(context.GetPC());
    context.SetPC(context.GetPC() + (instruction.immediate << 2));
  }
}

// Branch on not equal. CODE 13.
void InstructionSet::Bne(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Bne" << std::endl;
  }
  context.IncrementPC();
  
  if (context.GetRegister(instruction.src_reg_1) != context.GetRegister(instruction.dest_reg)) {
    Util::SetBranchIsTaken(context.GetPC());
    context.SetPC(context.GetPC() + (instruction.immediate << 2));
  }
}

// Divide. CODE 14.
void InstructionSet::Div(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Div" << std::endl;
  }
  if(context.GetRegister(instruction.src_reg_2) != 0){
    int32_t quotient = context.GetRegister(instruction.src_reg_1) / context.GetRegister(instruction.src_reg_2);
    int32_t div_remainder = context.GetRegister(instruction.src_reg_1) - quotient * context.GetRegister(instruction.src_reg_2);

    context.SetLo(quotient);
    context.SetHi(div_remainder);
  }
  context.IncrementPC();
}

// Divide unsigned. CODE 15.
void InstructionSet::Divu(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Divu" << std::endl;
  }
  if(context.GetRegister(instruction.src_reg_2) != 0){
    uint32_t quotient = (unsigned)context.GetRegister(instruction.src_reg_1) / (unsigned)context.GetRegister(instruction.src_reg_2);
    int32_t div_remainder = context.GetRegister(instruction.src_reg_1) - quotient * context.GetRegister(instruction.src_reg_2);
    context.SetLo(quotient);
    context.SetHi(div_remainder);
  }

  context.IncrementPC();
}

// Jump. CODE 16.
void InstructionSet::J(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> J" << std::endl;
  }
  context.IncrementPC();
  Util::SetBranchIsTaken(context.GetPC());

  // Target = target * 4.
  uint32_t target = instruction.target << 2;
  uint32_t pc_value = context.GetPC();
  // Clean PC bits (0 : 27).
  pc_value = pc_value & 0xF0000000;
  // Adding 28 bits target.
  pc_value += target;

  context.SetPC(pc_value);
}

// Jump and link register. CODE 17.
void InstructionSet::Jalr(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Jalr" << std::endl;
  }
  context.IncrementPC();
  uint32_t pc_value = context.GetPC();
  uint32_t link_reg = instruction.dest_reg;  
  uint32_t target = context.GetRegister(instruction.src_reg_1);
  context.SetRegister(link_reg, pc_value + 4);

  Util::SetBranchIsTaken(context.GetPC());

  context.SetPC(target);
}

// Jump and link. CODE 18.
void InstructionSet::Jal(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Jal" << std::endl;
  }
  context.IncrementPC();
  uint32_t pc_value = context.GetPC();
  context.SetRegister(31, pc_value + 4);
  
  Util::SetBranchIsTaken(context.GetPC());


  // Target = target * 4.
  uint32_t target = instruction.target << 2;

  // Clean PC bits (0 : 27).
  pc_value = pc_value & 0xF0000000;

  // Adding 28 bits target.
  pc_value += target;

  context.SetPC(pc_value);
}

// Jump register. CODE 19.
void InstructionSet::Jr(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Jr" << std::endl;
  }
  context.IncrementPC();
  Util::SetBranchIsTaken(context.GetPC());
  
  uint32_t pc_value = context.GetRegister(instruction.src_reg_1);
  context.SetPC(pc_value);
}

// Load byte. CODE 20.
void InstructionSet::Lb(const ParsedInstruction& instruction, Context& context, Memory& memory) {
  if (Util::DEBUG) {
    std::cerr << "> Lb" << std::endl;
  }
  int32_t source_register = context.GetRegister(instruction.src_reg_1);
  uint32_t address = instruction.immediate + source_register;
  context.SetRegister(instruction.dest_reg, memory.ReadDataByte(address, /*signed_load=*/true));

  context.IncrementPC();
}

// Load byte unsigned. CODE 21.
void InstructionSet::Lbu(const ParsedInstruction& instruction, Context& context, Memory& memory) {
  if (Util::DEBUG) {
    std::cerr << "> Lbu" << std::endl;
  }
  int32_t source_register = context.GetRegister(instruction.src_reg_1);
  uint32_t address = instruction.immediate + source_register;
  context.SetRegister(instruction.dest_reg, memory.ReadDataByte(address, /*signed_load=*/false));

  context.IncrementPC();
}

// Load half-word. CODE 22.
void InstructionSet::Lh(const ParsedInstruction& instruction, Context& context, Memory& memory) {
  if (Util::DEBUG) {
    std::cerr << "> Lh" << std::endl;
  }
  int32_t source_register = context.GetRegister(instruction.src_reg_1);
  uint32_t address = instruction.immediate + source_register;
  context.SetRegister(instruction.dest_reg, memory.ReadHalfDataWord(address, /*signed_load=*/true));

  context.IncrementPC();
}

// Load half-word unsigned. CODE 23.
void InstructionSet::Lhu(const ParsedInstruction& instruction, Context& context, Memory& memory) {
  if (Util::DEBUG) {
    std::cerr << "> Lhu" << std::endl;
  }
  int32_t source_register = context.GetRegister(instruction.src_reg_1);
  uint32_t address = instruction.immediate + source_register;
  context.SetRegister(instruction.dest_reg, memory.ReadHalfDataWord(address, /*signed_load=*/false));

  context.IncrementPC();
}

// Load upper immediate. CODE 24.
void InstructionSet::Lui(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Lui" << std::endl;
  }
  int32_t target_reg = instruction.dest_reg;
  int32_t result = instruction.immediate << 16;
  context.SetRegister(target_reg, result);

  context.IncrementPC();
}

// Load word. CODE 25.`
void InstructionSet::Lw(const ParsedInstruction& instruction, Context& context, Memory& memory) {
  if (Util::DEBUG) {
    std::cerr << "> Lw" << std::endl;
  }
  int32_t source_register = context.GetRegister(instruction.src_reg_1);
  uint32_t address = instruction.immediate + source_register;
  int32_t memory_word = memory.ReadDataWord(address);
  context.SetRegister(instruction.dest_reg, memory_word);

  context.IncrementPC();
}

// Load word left. CODE 26.
void InstructionSet::Lwl(const ParsedInstruction& instruction, Context& context, Memory& memory) {
  if (Util::DEBUG) {
    std::cerr << "> Lwl" << std::endl;
  }
  int32_t source_register = context.GetRegister(instruction.src_reg_1);
  uint32_t address = instruction.immediate + source_register;
  int32_t load_result = memory.ReadWordLeft(address);
  int32_t byte_offset = address % 4;
  int32_t dest_reg = context.GetRegister(instruction.dest_reg);

  // Clear the part to overwrite. The most significant bytes.
  for (int32_t i = 3; i >= byte_offset; i--) {
    dest_reg = dest_reg & (0x00ffffff >> ((3 - i) * 8));
  }

  // Insert the memory load result.
  dest_reg = dest_reg | load_result;

  context.SetRegister(instruction.dest_reg, dest_reg);

  context.IncrementPC();
}

// Load word right. CODE 27.
void InstructionSet::Lwr(const ParsedInstruction& instruction, Context& context, Memory& memory) {
  if (Util::DEBUG) {
    std::cerr << "> Lwr" << std::endl;
  }
  int32_t source_register = context.GetRegister(instruction.src_reg_1);
  uint32_t address = instruction.immediate + source_register;
  int32_t load_result = memory.ReadWordRight(address);
  int32_t byte_offset = address % 4;
  int32_t dest_reg = context.GetRegister(instruction.dest_reg);

  // Clear the part to overwrite. The least significant bytes.
  for (int32_t i = 0; i <= byte_offset; i++) {
    dest_reg = dest_reg & (0xffffff00 << ((i) * 8));
  }

  // Insert the memory load result.
  dest_reg = dest_reg | load_result;

  context.SetRegister(instruction.dest_reg, dest_reg);

  context.IncrementPC();
}

// Move from HI. CODE 28.
void InstructionSet::Mfhi(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Mfhi" << std::endl;
  }
  int32_t Hi_value = context.GetHi();
  context.SetRegister(instruction.dest_reg, Hi_value); 

  context.IncrementPC();
}

// Move from LO. CODE 29.
void InstructionSet::Mflo(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Mflo" << std::endl;
  }
  int32_t Lo_value = context.GetLo();
  context.SetRegister(instruction.dest_reg, Lo_value); 

  context.IncrementPC();
}

// Move to HI. CODE 30.
void InstructionSet::Mthi(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Mthi" << std::endl;
  }
  int32_t Hi_value = context.GetRegister(instruction.src_reg_1);
  context.SetHi(Hi_value); 

  context.IncrementPC();
}

// Move to LO. CODE 31.
void InstructionSet::Mtlo(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Mtlo" << std::endl;
  }
  int32_t Lo_value = context.GetRegister(instruction.src_reg_1);
  context.SetLo(Lo_value); 

  context.IncrementPC();
}

// Multiply. CODE 32.
void InstructionSet::Mult(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Mult" << std::endl;
  }
  int64_t src_reg_1 = context.GetRegister(instruction.src_reg_1);
  int64_t src_reg_2 = context.GetRegister(instruction.src_reg_2);
  
  int64_t product = src_reg_1 * src_reg_2;
  int32_t Hi_product = product >> 32;
  int32_t Lo_product = product & 0xFFFFFFFF;
  
  context.SetHi(Hi_product); 
  context.SetLo(Lo_product); 

  context.IncrementPC();
}

// Multiply unsigned. CODE 33.
void InstructionSet::Multu(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Multu" << std::endl;
  }
  uint64_t src_reg_1 = (unsigned)context.GetRegister(instruction.src_reg_1);
  uint64_t src_reg_2 = (unsigned)context.GetRegister(instruction.src_reg_2);

  uint64_t product = src_reg_1 * src_reg_2;
  uint32_t Hi_product = product >> 32;
  uint32_t Lo_product = product & 0xFFFFFFFF;
  
  context.SetHi(Hi_product); 
  context.SetLo(Lo_product); 

  context.IncrementPC();
}

// Bitwise or. CODE 34.
void InstructionSet::Or(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Or" << std::endl;
  }
  int32_t result = context.GetRegister(instruction.src_reg_1) |
               context.GetRegister(instruction.src_reg_2);
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Bitwise or immediate. CODE 35.
void InstructionSet::Ori(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Ori" << std::endl;
  }
  int32_t result = context.GetRegister(instruction.src_reg_1) |
               (instruction.immediate & 0xFFFF);
  
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Store byte. CODE 36.
void InstructionSet::Sb(const ParsedInstruction& instruction, Context& context, Memory& memory) {
  if (Util::DEBUG) {
    std::cerr << "> Sb" << std::endl;
  }

  int32_t source_register = context.GetRegister(instruction.src_reg_1);
  int32_t out_word = context.GetRegister(instruction.dest_reg);
  uint32_t address = instruction.immediate + source_register;
  memory.WriteDataByte(address, out_word);

  context.IncrementPC();
}

// Store half word. CODE 37.
void InstructionSet::Sh(const ParsedInstruction& instruction, Context& context, Memory& memory) {
  if (Util::DEBUG) {
    std::cerr << "> Sh" << std::endl;
  }

  int32_t source_register = context.GetRegister(instruction.src_reg_1);
  int32_t out_word = context.GetRegister(instruction.dest_reg);
  uint32_t address = instruction.immediate + source_register;
  memory.WriteHalfDataWord(address, out_word);

  context.IncrementPC();
}

// Shift left logical. CODE 38.
void InstructionSet::Sll(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Sll" << std::endl;
  }
  int32_t source_register = context.GetRegister(instruction.src_reg_2);
  int32_t result =  source_register << instruction.shift_amount;
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Shift left logical variable. CODE 39.
void InstructionSet::Sllv(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Sllv" << std::endl;
  }
  uint32_t source_register = (uint32_t)context.GetRegister(instruction.src_reg_2);
  uint32_t shift_amount = (uint32_t)context.GetRegister(instruction.src_reg_1);
  int32_t result =  source_register << shift_amount;
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Set less than. CODE 40.
void InstructionSet::Slt(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Slt" << std::endl;
  }
  int32_t boolean_result = context.GetRegister(instruction.src_reg_1) < context.GetRegister(instruction.src_reg_2);
  int32_t dest_reg = instruction.dest_reg;
  context.SetRegister(dest_reg, boolean_result);

  context.IncrementPC();
}

// Set less than immediate. CODE 41.
void InstructionSet::Slti(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Slti" << std::endl;
  }
  int32_t boolean_result = context.GetRegister(instruction.src_reg_1) < instruction.immediate;
  int32_t dest_reg = instruction.dest_reg;
  context.SetRegister(dest_reg, boolean_result);

  context.IncrementPC();
}

// Set less than immediate unsigned. CODE 42.
void InstructionSet::Sltiu(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Sltiu" << std::endl;
  }
  int32_t boolean_result = context.GetRegister(instruction.src_reg_1) < unsigned(instruction.immediate);
  int32_t dest_reg = instruction.dest_reg;
  context.SetRegister(dest_reg, boolean_result);

  context.IncrementPC();
}

// Set less than unsigned. CODE 43.
void InstructionSet::Sltu(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Sltu" << std::endl;
  }
  int32_t boolean_result = unsigned(context.GetRegister(instruction.src_reg_1)) < unsigned(context.GetRegister(instruction.src_reg_2));
  int32_t dest_reg = instruction.dest_reg;
  context.SetRegister(dest_reg, boolean_result);

  context.IncrementPC();
}

// Shift right arithmetic. CODE 44.
void InstructionSet::Sra(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Sra" << std::endl;
  }
  int32_t src_reg = context.GetRegister(instruction.src_reg_2);
  // One filled since src_reg is signed int.
  int32_t result = src_reg >> instruction.shift_amount;
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Shift right arithmetic variable. CODE 45.
void InstructionSet::Srav(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Srav" << std::endl;
  }
  int32_t src_reg = context.GetRegister(instruction.src_reg_2);
  // One filled since src_reg is signed int.
  uint32_t shift_amount = (uint32_t)context.GetRegister(instruction.src_reg_1);
  int32_t result = src_reg >> shift_amount;
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Shift right logical. CODE 46.
void InstructionSet::Srl(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Srl" << std::endl;
  }
  uint32_t source_register = (uint32_t)context.GetRegister(instruction.src_reg_2);
  // Zero filled since source_register is unsigned int.
  int32_t result =  source_register >> instruction.shift_amount;
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Shift right logical variable. CODE 47.
void InstructionSet::Srlv(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Srlv" << std::endl;
  }
  uint32_t source_register = (uint32_t)context.GetRegister(instruction.src_reg_2);
  uint32_t shift_amount = (uint32_t)context.GetRegister(instruction.src_reg_1);
  // Zero filled since source_register is unsigned int.
  int32_t result =  source_register >> shift_amount;
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Subtract. CODE 48.
void InstructionSet::Sub(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Sub" << std::endl;
  }
  int32_t src_reg_1 = context.GetRegister(instruction.src_reg_1);
  int32_t src_reg_2 = context.GetRegister(instruction.src_reg_2);
  int32_t result = src_reg_1 - src_reg_2;
  if (Util::DEBUG) {
    std::cerr << std::dec << "reg1: " << src_reg_1 << " reg2: " << src_reg_2 << " res: " << result << std::endl;
  }

  if ((src_reg_1 >= 0 && src_reg_2 < 0) && result < 0 ||
      (src_reg_1 < 0 && src_reg_2 >= 0) && result >= 0) {
    if (Util::DEBUG) {
      std::cerr << "ERROR: Overflow, src_reg_1 value (dec): " << std::dec << src_reg_1
                << " src_reg_2 value (dec): " << src_reg_2 << std::endl;
    }
    exit(Util::ARITHMETIC_EXCEPTION);
  }
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Subtract unsigned. CODE 49.
void InstructionSet::Subu(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Subu" << std::endl;
  }
  int32_t src_reg_1 = context.GetRegister(instruction.src_reg_1);
  int32_t src_reg_2 = context.GetRegister(instruction.src_reg_2);
  int32_t result = src_reg_1 - src_reg_2;
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Store word. CODE 50.
void InstructionSet::Sw(const ParsedInstruction& instruction, Context& context, Memory& memory) {
  if (Util::DEBUG) {
    std::cerr << "> Sw" << std::endl;
  }
  int32_t source_register = context.GetRegister(instruction.src_reg_1);
  int32_t out_word = context.GetRegister(instruction.dest_reg);
  uint32_t address = instruction.immediate + source_register;
  memory.WriteDataWord(address, out_word);

  context.IncrementPC();
}

// Bitwise xor. CODE 51.
void InstructionSet::Xor(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Xor" << std::endl;
  }
  int32_t result = context.GetRegister(instruction.src_reg_1) ^
               context.GetRegister(instruction.src_reg_2);
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}

// Bitwise xor immediate. CODE 52.
void InstructionSet::Xori(const ParsedInstruction& instruction, Context& context) {
  if (Util::DEBUG) {
    std::cerr << "> Xori" << std::endl;
  }
  int32_t result = context.GetRegister(instruction.src_reg_1) ^
               (instruction.immediate & 0xFFFF);
  context.SetRegister(instruction.dest_reg, result);

  context.IncrementPC();
}
