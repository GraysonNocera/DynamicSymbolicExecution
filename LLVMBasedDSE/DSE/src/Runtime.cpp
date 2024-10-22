#include <iostream>

#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"

#include "SymbolicInterpreter.h"

using namespace llvm;

extern SymbolicInterpreter SI;

/*
 * Implement your transfer functions.
 */
extern "C" void __DSE_Alloca__(int R, int *Ptr) {
  Address X(R);
  z3::expr Z = SI.getContext().int_val(*Ptr);
  SI.getMemory().insert(std::make_pair(X, Z));
}

extern "C" void __DSE_Store__(int *X) {
  Address A(X);
  z3::expr E = SI.getStack().top();
  SI.getStack().pop();
  SI.getMemory().insert(std::make_pair(A, E));
}

extern "C" void __DSE_Load__(int Y, int *X) {
  Address A(Y);
  z3::expr E = SI.getMemory().at(Address(X));
  SI.getMemory().insert(std::make_pair(A, E));
}

extern "C" void __DSE_ICmp__(int R, int Op) {
  // the two parameters should be on the stack
  // Op is the opcode (Add, Sub, Divide)
  // R is the Register that we are going to add as a result of this binary operation

  Address A(R);
  // TODO: may need to switch these around?
  z3::expr O2 = SI.getStack().top();
  SI.getStack().pop();
  z3::expr O1 = SI.getStack().top();
  SI.getStack().pop();
  
  z3::expr Result(SI.getContext()); 
  switch (Op) {
  case CmpInst::ICMP_EQ:
    Result = O1 == O2;
    break;
  case CmpInst::ICMP_NE:
    Result = O1 != O2;
    break;
  case CmpInst::ICMP_SGE:
  case CmpInst::ICMP_UGE:
    Result = O1 >= O2;
    break;
  case CmpInst::ICMP_SLE:
  case CmpInst::ICMP_ULE:
    Result = O1 <= O2;
    break;
  case CmpInst::ICMP_SGT:
  case CmpInst::ICMP_UGT:
    Result = O1 > O2;
    break;
  case CmpInst::ICMP_SLT:
  case CmpInst::ICMP_ULT:
    Result = O1 < O2;
    break;
  default:
    Result = O1 + O2;
    break;
  }

  SI.getMemory().insert(std::make_pair(A, Result));
}

extern "C" void __DSE_BinOp__(int R, int Op) {
  Address A(R);
  // TODO: may need to switch these around?
  z3::expr O2 = SI.getStack().top();
  SI.getStack().pop();
  z3::expr O1 = SI.getStack().top();
  SI.getStack().pop();
  
  z3::expr Result(SI.getContext());
  switch (Op) {
  case Instruction::Add:
    Result = O1 + O2;
    break;
  case Instruction::Sub:
    Result = O1 - O2;
    break;
  case Instruction::Mul:
    Result = O1 * O2;
    break;
  case Instruction::SDiv:
  case Instruction::UDiv:
    Result = O1 / O2;
    break;
  }

  SI.getMemory().insert(std::make_pair(A, Result));
}
