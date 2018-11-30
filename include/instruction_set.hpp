/*
 * Class that implements the instruction set for the MIPS architecture.
 */


#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include "context.hpp"
#include "memory.hpp"

class InstructionSet{
 public:
  static void ExecuteInstructionAndUpdatePC(const ParsedInstruction& instruction,
                                            Context& context,
                                            Memory& memory);

 private:                                                                                   // TESTS IMPLEMENTED
  static void Add(const ParsedInstruction& instruction, Context& context);                  // add1 - add5
  static void Addi(const ParsedInstruction& instruction, Context& context);                 // addi1 - addi5
  static void Addiu(const ParsedInstruction& instruction, Context& context);                // addiu1 - addiu2
  static void Addu(const ParsedInstruction& instruction, Context& context);                 // addu1
  static void And(const ParsedInstruction& instruction, Context& context);                  // and1
  static void Andi(const ParsedInstruction& instruction, Context& context);                 // andi1 - andi2
  static void Beq(const ParsedInstruction& instruction, Context& context);                  // beq1 - beq3
  static void Bgez(const ParsedInstruction& instruction, Context& context);                 // bgez1 - bgez3
  static void Bgezal(const ParsedInstruction& instruction, Context& context);               // bgezal1 - bgezal6
  static void Bgtz(const ParsedInstruction& instruction, Context& context);                 // bgtz1 - bgtz3
  static void Blez(const ParsedInstruction& instruction, Context& context);                 // blez1 - blez3
  static void Bltz(const ParsedInstruction& instruction, Context& context);                 // bltz1 - bltz3
  static void Bltzal(const ParsedInstruction& instruction, Context& context);               // bltzal1 - bltzal6
  static void Bne(const ParsedInstruction& instruction, Context& context);                  // bne1 - bne3
  static void Div(const ParsedInstruction& instruction, Context& context);                  // div1 - div8
  static void Divu(const ParsedInstruction& instruction, Context& context);                 // divu1 - divu5
  static void J(const ParsedInstruction& instruction, Context& context);                    // j1 - j2
  static void Jalr(const ParsedInstruction& instruction, Context& context);                 // jalr1 - jalr3
  static void Jal(const ParsedInstruction& instruction, Context& context);                  // jal1 - jal4
  static void Jr(const ParsedInstruction& instruction, Context& context);                   // jr1 - jr6
  static void Lb(const ParsedInstruction& instruction, Context& context, Memory& memory);   // lb1- lb13
  static void Lbu(const ParsedInstruction& instruction, Context& context, Memory& memory);  // lbu1- lbu13
  static void Lh(const ParsedInstruction& instruction, Context& context, Memory& memory);   // lh1 - lh13
  static void Lhu(const ParsedInstruction& instruction, Context& context, Memory& memory);  // lhu1 - lhu13
  static void Lui(const ParsedInstruction& instruction, Context& context);                  // lui1 - lui2
  static void Lw(const ParsedInstruction& instruction, Context& context, Memory& memory);   // lw1 - lw11
  static void Lwl(const ParsedInstruction& instruction, Context& context, Memory& memory);  // lwl1 -lwl11
  static void Lwr(const ParsedInstruction& instruction, Context& context, Memory& memory);  // lwr1 -lwr12
  static void Mfhi(const ParsedInstruction& instruction, Context& context);                 // mfhi1
  static void Mflo(const ParsedInstruction& instruction, Context& context);                 // mflo1
  static void Mthi(const ParsedInstruction& instruction, Context& context);                 // mthi1
  static void Mtlo(const ParsedInstruction& instruction, Context& context);                 // mtlo1
  static void Mult(const ParsedInstruction& instruction, Context& context);                 // mult1 - mult6
  static void Multu(const ParsedInstruction& instruction, Context& context);                // multu1 - multu4
  static void Or(const ParsedInstruction& instruction, Context& context);                   // or1 - or2
  static void Ori(const ParsedInstruction& instruction, Context& context);                  // ori1 - ori2
  static void Sb(const ParsedInstruction& instruction, Context& context, Memory& memory);   // sb1 - sb8
  static void Sh(const ParsedInstruction& instruction, Context& context, Memory& memory);   // sh1 - sh8
  static void Sll(const ParsedInstruction& instruction, Context& context);                  // sll1 - sll2
  static void Sllv(const ParsedInstruction& instruction, Context& context);                 // sllv1 - sllv2
  static void Slt(const ParsedInstruction& instruction, Context& context);                  // slt1 - slt2
  static void Slti(const ParsedInstruction& instruction, Context& context);                 // slti1 - slti2
  static void Sltiu(const ParsedInstruction& instruction, Context& context);                // sltiu1 - slti2
  static void Sltu(const ParsedInstruction& instruction, Context& context);                 // sltu1 - slti2
  static void Sra(const ParsedInstruction& instruction, Context& context);                  // sra1 - sra2
  static void Srav(const ParsedInstruction& instruction, Context& context);                 //srav1 -srav2
  static void Srl(const ParsedInstruction& instruction, Context& context);                  // srl1
  static void Srlv(const ParsedInstruction& instruction, Context& context);                 // srlv1 
  static void Sub(const ParsedInstruction& instruction, Context& context);                  // sub1 - sub3          
  static void Subu(const ParsedInstruction& instruction, Context& context);                 // subu1
  static void Sw(const ParsedInstruction& instruction, Context& context, Memory& memory);   // sw1 - sw9
  static void Xor(const ParsedInstruction& instruction, Context& context);                  // xor1
  static void Xori(const ParsedInstruction& instruction, Context& context);                 // xori1
};

#endif