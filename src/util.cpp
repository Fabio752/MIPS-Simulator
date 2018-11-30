#include <iostream>

#include "../include/util.hpp"

ParsedInstruction Util::ParseInstruction(uint32_t instruction) {
  ParsedInstruction parsed_instruction;

  parsed_instruction.opcode = instruction >> 26;

  /////////////////// R-TYPE ///////////////////
  // If RType instruction opcode must be 000000.
  if (parsed_instruction.opcode == 0b000000){
    parsed_instruction.src_reg_1 = (instruction >> 21) & 0x1F;
    parsed_instruction.src_reg_2 = (instruction >> 16) & 0x1F;
    parsed_instruction.dest_reg = (instruction >> 11) & 0x1F;
    parsed_instruction.shift_amount = (instruction >> 6) & 0x1F;
    parsed_instruction.function = instruction & 0x3F;

    // Decode instruction.
    if (parsed_instruction.function == 0b100000)
      parsed_instruction.instruction_type = 0; //ADD

    else if (parsed_instruction.function == 0b100001)
      parsed_instruction.instruction_type = 3; //ADDU

    else if (parsed_instruction.function == 0b100100)
      parsed_instruction.instruction_type = 4; //AND

    else if (parsed_instruction.function == 0b011010)
      parsed_instruction.instruction_type = 14; //DIV

    else if (parsed_instruction.function == 0b011011)
      parsed_instruction.instruction_type = 15; //DIVU
    
    else if (parsed_instruction.function == 0b001001)
      parsed_instruction.instruction_type = 17; //JALR

    else if (parsed_instruction.function == 0b001000)
      parsed_instruction.instruction_type = 19; //JR

    else if (parsed_instruction.function == 0b010000)
      parsed_instruction.instruction_type = 28; //MFHI

    else if (parsed_instruction.function == 0b010010)
      parsed_instruction.instruction_type = 29; //MFLO
      
    else if (parsed_instruction.function == 0b010001)
      parsed_instruction.instruction_type = 30; //MTHI
      
    else if (parsed_instruction.function == 0b010011)
      parsed_instruction.instruction_type = 31; //MTLO

    else if (parsed_instruction.function == 0b011000)
      parsed_instruction.instruction_type = 32; //MULT
      
    else if (parsed_instruction.function == 0b011001)
      parsed_instruction.instruction_type = 33; //MULTU
      
    else if (parsed_instruction.function == 0b100101)
      parsed_instruction.instruction_type = 34; //OR

    else if (parsed_instruction.function == 0b000000)
      parsed_instruction.instruction_type = 38; //SLL
              
    else if (parsed_instruction.function == 0b000100)
      parsed_instruction.instruction_type = 39; //SLLV
              
    else if (parsed_instruction.function == 0b101010)
      parsed_instruction.instruction_type = 40; //SLT
              
    else if (parsed_instruction.function == 0b101011)
      parsed_instruction.instruction_type = 43; //SLTU

    else if (parsed_instruction.function == 0b000011)
      parsed_instruction.instruction_type = 44; //SRA

    else if (parsed_instruction.function == 0b000111)
      parsed_instruction.instruction_type = 45; //SRAV

    else if (parsed_instruction.function == 0b000010)
      parsed_instruction.instruction_type = 46; //SRL

    else if (parsed_instruction.function == 0b000110)
      parsed_instruction.instruction_type = 47; //SRLV

    else if (parsed_instruction.function == 0b100010)
      parsed_instruction.instruction_type = 48; //SUB

    else if (parsed_instruction.function == 0b100011)
      parsed_instruction.instruction_type = 49; //SUBU

    else if (parsed_instruction.function == 0b100110)
      parsed_instruction.instruction_type = 51; //XOR

    else{
      if (Util::DEBUG) {
        std::cerr << "ERROR: Invalid instruction with R-Type opcode:" << std::endl
                  << "- invalid function code (hex): " << std::hex << parsed_instruction.function << std::dec << std::endl;
      }
      exit(Util::INVALID_INSTRUCTION_EXCEPTION);
    }
  }  

  /////////////////// J-TYPE ///////////////////
  // If JType instruction opcode must be 00001?.
  else if (parsed_instruction.opcode == 0b000010 || parsed_instruction.opcode == 0b000011){
    parsed_instruction.target = instruction & 0x3FFFFFF;
    SignExtend26To32(parsed_instruction.target);

    if(parsed_instruction.opcode == 0b000010)
      parsed_instruction.instruction_type = 16; //J

    else if(parsed_instruction.opcode == 0b000011)
      parsed_instruction.instruction_type = 18; //JAL
  }

  /////////////////// I-TYPE ///////////////////
  // If IType instruction opcode must be ??????.
  else {
    parsed_instruction.src_reg_1 = (instruction >> 21) & 0b11111;
    parsed_instruction.dest_reg = (instruction >> 16) & 0b11111;
    parsed_instruction.immediate = instruction & 0xFFFF;
    SignExtend16To32(parsed_instruction.immediate);

    // Decode instruction.
    if (parsed_instruction.opcode == 0b001000)
      parsed_instruction.instruction_type = 1; //ADDI

    else if (parsed_instruction.opcode == 0b001001)
      parsed_instruction.instruction_type = 2; //ADDIU
    
    else if (parsed_instruction.opcode == 0b001100)
      parsed_instruction.instruction_type = 5; //ANDI

    else if (parsed_instruction.opcode == 0b000100)
      parsed_instruction.instruction_type = 6; //BEQ

    else if (parsed_instruction.opcode == 0b000001){
      if(parsed_instruction.dest_reg == 0b00001)
        parsed_instruction.instruction_type = 7; //BGEZ
      
      else if(parsed_instruction.dest_reg == 0b10001)
        parsed_instruction.instruction_type = 8; //BGEZAL
      
      else if(parsed_instruction.dest_reg == 0b00000)
        parsed_instruction.instruction_type = 11; //BLTZ
      
      else if(parsed_instruction.dest_reg == 0b10000)
        parsed_instruction.instruction_type = 12; //BLTZAL
      
      else {
        if (Util::DEBUG) {
          std::cerr << "ERROR: Invalid instruction with I-Type opcode:" << std::endl
                    << "- opcode (hex): " << std::hex << parsed_instruction.opcode << std::dec << std::endl
                    << "- invalid dest_reg (dec): " << parsed_instruction.dest_reg << std::endl;
        }
        exit(Util::INVALID_INSTRUCTION_EXCEPTION);
      }
    }

    else if (parsed_instruction.opcode == 0b000111)
      parsed_instruction.instruction_type = 9; //BGTZ

    else if (parsed_instruction.opcode == 0b000110)
      parsed_instruction.instruction_type = 10; //BLEZ

    else if (parsed_instruction.opcode == 0b000101)
      parsed_instruction.instruction_type = 13; //BNE

    else if (parsed_instruction.opcode == 0b100000)
      parsed_instruction.instruction_type = 20; //LB
    
    else if (parsed_instruction.opcode == 0b100100)
      parsed_instruction.instruction_type = 21; //LBU
      
    else if (parsed_instruction.opcode == 0b100001)
      parsed_instruction.instruction_type = 22; //LH
      
    else if (parsed_instruction.opcode == 0b100101)
      parsed_instruction.instruction_type = 23; //LHU
      
    else if (parsed_instruction.opcode == 0b001111)
      parsed_instruction.instruction_type = 24; //LUI
      
    else if (parsed_instruction.opcode == 0b100011)
      parsed_instruction.instruction_type = 25; //LW
      
    else if (parsed_instruction.opcode == 0b100010)
      parsed_instruction.instruction_type = 26; //LWL
      
    else if (parsed_instruction.opcode == 0b100110)
      parsed_instruction.instruction_type = 27; //LWR
      
    else if (parsed_instruction.opcode == 0b001101)
      parsed_instruction.instruction_type = 35; //ORI
      
    else if (parsed_instruction.opcode == 0b101000)
      parsed_instruction.instruction_type = 36; //SB
      
    else if (parsed_instruction.opcode == 0b101001)
      parsed_instruction.instruction_type = 37; //SH
    
    else if (parsed_instruction.opcode == 0b001010)
      parsed_instruction.instruction_type = 41; //SLTI
      
    else if (parsed_instruction.opcode == 0b001011)
      parsed_instruction.instruction_type = 42; //SLTIU
      
    else if (parsed_instruction.opcode == 0b101011)
      parsed_instruction.instruction_type = 50; //SW
      
    else if (parsed_instruction.opcode == 0b001110)
      parsed_instruction.instruction_type = 52; //XORI
    
    else{
      if (Util::DEBUG) {
        std::cerr << "ERROR: Invalid instruction with I-Type opcode:" << std::endl
                  << "- invalid opcode (hex): " << std::hex << parsed_instruction.opcode << std::dec << std::endl;
      }
      exit(Util::INVALID_INSTRUCTION_EXCEPTION);
    }
  }

  return parsed_instruction;
}

void Util::SignExtend8To32(unsigned int& value) {
  value = (value & 0x80) ? (value | 0xFFFFFF00) : value;
}

void Util::SignExtend16To32(unsigned int& value) {
  value = (value & 0x8000) ? (value | 0xFFFF0000) : value;
}

void Util::SignExtend16To32(int& value) {
  value = (value & 0x8000) ? (value | 0xFFFF0000) : value;
}

void Util::SignExtend26To32(int& value) {
  value = (value & 0x2000000) ? (value | 0xFC000000) : value;
}

uint32_t __PC_before_branch;
bool __branch_taken;

void Util::SetBranchIsTaken(uint32_t PC_address) {
  __PC_before_branch = PC_address;
  __branch_taken = true;
}

bool Util::CheckBranchLock() {
  return __branch_taken;
}

uint32_t Util::GetPCBeforeBranch() {
  return __PC_before_branch;
}

void Util::ReleaseBranchLock() {
  __branch_taken = false;
}
