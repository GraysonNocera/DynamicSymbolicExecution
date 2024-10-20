#include "Instrument.h"

using namespace llvm;

namespace instrument {

void Instrument::createDseFunction(static const char *name, std::vector<Type *> Params, std::vector<Value *> Args, Function &F, Instruction &I, bool before) {
  Module *M = F.getParent();
  FunctionType *FT = FunctionType::get(Type::getVoidTy(M->getContext()), Params, false);
  FunctionCallee f = M->getOrInsertFunction(name, FT);
  CallInst *Call = CallInst::Create(f, Args, "", &I);
  // TODO: handle before or after this instructions
  Call->setCallingConv(CallingConv::C);
  Call->setTailCall(true);
}

/*
 * Implement your instrumentation for dynamic symbolic execution engine
 */
bool Instrument::runOnFunction(Function &F) {
  outs() << "Running " << getAnalysisName() << " on " << F.getName() << "\n";


  // insert "call void __DSE_Init__();"
  // before the first command
  inst_iterator I = inst_begin(F);
  std::vector<Type *> Params(0);
  std::vector<Value *> Args(0);
  Instrument::createDseFunction(DSEInitFunctionName, Params, Args, F, I, true)

  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    // do something for each instruction
    if (BinaryOperator *BO = dyn_cast<BinaryOperator>(I))
    {
      outs() << "Binary operator\n";
      Value *op1 = BO->getOperand(0);
      Value *op2 = BO->getOperand(1);

      switch (BO->getOpcode())
      {
      case Instruction::Add:
        break;
      case Instruction::Sub:
        break;
      case Instruction::Mul:
        break;
      case Instruction::SDiv:
      case Instruction::UDiv:
        break;
      }
    }
    else if (CastInst *CastI = dyn_cast<CastInst>(I))
    {
      outs() << "Cast int\n";
      Value *op1 = CastI->getOperand(0);
    }
    else if (CmpInst *CmpI = dyn_cast<CmpInst>(I))
    {
      outs() << "compare inst\n";
      Value *op1 = CmpI->getOperand(0);
      Value *op2 = CmpI->getOperand(1);

        // at this point, we know that neither are MaybeZero or Uninit
        switch (CmpI->getPredicate())
        {
        case CmpInst::ICMP_EQ:
        case CmpInst::ICMP_NE:
        case CmpInst::ICMP_SGE:
        case CmpInst::ICMP_UGE:
        case CmpInst::ICMP_SLE:
        case CmpInst::ICMP_ULE:
        case CmpInst::ICMP_SGT:
        case CmpInst::ICMP_UGT:
        case CmpInst::ICMP_SLT:
        case CmpInst::ICMP_ULT:
        }
    }
    else if (BranchInst *BI = dyn_cast<BranchInst>(I))
    {
      outs() << "branch inst\n";
    }
    else if (CallInst *CI = dyn_cast<CallInst>(I))
    {
      if (!CI->getFunctionType()->getReturnType()->isIntegerTy())
      {
        return;
      }
    }
    else if (AllocaInst *AI = dyn_cast<AllocaInst>(I))
    {
      // TODO: do we need this?
    }
    else if (LoadInst *LI = dyn_cast<LoadInst>(I))
    {
      Value *FromPtr = LI->getPointerOperand();
      LI->getType()->dump();
      if (LI->getType()->isIntegerTy())
      {
        Value *LoadedInt = FromPtr;
      }
      else if (FromPtr->getType()->isPointerTy())
      {
        outs() << "a load with a pointer!\n";
      }
    }
    else if (StoreInst *SI = dyn_cast<StoreInst>(I))
    {
      // for a store, setup:
      Value *Ptr = SI->getPointerOperand();
      Value *Val = SI->getValueOperand();
      Ptr->dump();
      Val->dump();
      if (Val->getType()->isIntegerTy())
      {
        // if integery type:
        // get the abstract domain value of the value operand
        // update the Ptr operand's out to be domain value of value operand
        outs() << "inserting: " << variable(Ptr) << "\n";
      }
      else if (Val->getType()->isPointerTy())
      {
        outs() << "variable PTR = " << variable(Ptr) << "\n";
      }
    }
  }

  return true;
}

char Instrument::ID = 1;
static RegisterPass<Instrument>
    X("Instrument", "Instrumentations for Dynamic Symbolic Execution", false,
      false);

} // namespace instrument
