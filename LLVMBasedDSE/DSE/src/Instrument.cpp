#include "Instrument.h"

using namespace llvm;

namespace instrument {

void Instrument::createDseFunction(const char *name, std::vector<Type *> Params, std::vector<Value *> Args, Function &F, Instruction &I, bool before) {
  Module *M = F.getParent();
  FunctionType *FT = FunctionType::get(Type::getVoidTy(M->getContext()), Params, false);
  FunctionCallee f = M->getOrInsertFunction(name, FT);
  CallInst *Call;
  if (before) {
    Call = CallInst::Create(f, Args, "", &I);
  } else {
    Instruction *Next = I.getNextNode();
    Next->dump();
    Call = CallInst::Create(f, Args, "", Next);
  }
  Call->setCallingConv(CallingConv::C);
  Call->dump();
  // Call->setTailCall(true);
}

/*
 * Implement your instrumentation for dynamic symbolic execution engine
 */
bool Instrument::runOnFunction(Function &F) {
  Module *M = F.getParent();

  F.dump();

  // insert "call void __DSE_Init__();"
  // before the first command
  inst_iterator Iter = inst_begin(F);
  Instruction *I = &(*Iter);
  std::vector<Type *> Params(0);
  std::vector<Value *> Args(0);
  Instrument::createDseFunction(DSEInitFunctionName, Params, Args, F, *I, true);

  for (inst_iterator Iter = inst_begin(F), E = inst_end(F); Iter != E; ++Iter) {
    Instruction *I = &(*Iter);
    outs() << "next iterated instruction: ";
    I->dump();
    Type *i32Type = IntegerType::getInt32Ty(M->getContext());
    // do something for each instruction
    if (BinaryOperator *BO = dyn_cast<BinaryOperator>(I))
    {
      // example 
      outs() << "Binary operator\n";
      Value *op1 = BO->getOperand(0);
      Value *op2 = BO->getOperand(1);
      
      std::vector<Type *> Params(1, Type::getInt32Ty(M->getContext()));
      std::vector<Value *> Args(1);
      if (ConstantInt *i = dyn_cast<ConstantInt>(op1)) {
        Args[0] = i;
        Instrument::createDseFunction(DSEConstFunctionName, Params, Args, F, *I);
      } else {
        Constant *idVal = ConstantInt::get(i32Type, APInt(32, getRegisterID(op1)));
        Args[0] = idVal;
        Instrument::createDseFunction(DSERegisterFunctionName, Params, Args, F, *I);
      }
      if (ConstantInt *i = dyn_cast<ConstantInt>(op2)) {
        Args[0] = i;
        Instrument::createDseFunction(DSEConstFunctionName, Params, Args, F, *I);
      } else {
        Constant *idVal = ConstantInt::get(i32Type, APInt(32, getRegisterID(op2)));
        Args[0] = idVal;
        Instrument::createDseFunction(DSERegisterFunctionName, Params, Args, F, *I);
      }

      int id = getRegisterID(I);
      int opcode = BO->getOpcode();
      Type *i32Type = IntegerType::getInt32Ty(M->getContext());
      Constant *idVal = ConstantInt::get(i32Type, APInt(32, id));
      Constant *opcodeVal = ConstantInt::get(i32Type, APInt(32, opcode));

      std::vector<Type *> ParamsBinOp(2, Type::getInt32Ty(M->getContext()));
      std::vector<Value *> ArgsBinOp(2);
      ArgsBinOp[0] = idVal;
      ArgsBinOp[1] = opcodeVal;
      Instrument::createDseFunction(DSEBinOpFunctionName, ParamsBinOp, ArgsBinOp, F, *I);
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

      std::vector<Type *> Params(1, Type::getInt32Ty(M->getContext()));
      std::vector<Value *> Args(1);
      if (ConstantInt *i = dyn_cast<ConstantInt>(op1)) {
        Args[0] = i;
        Instrument::createDseFunction(DSEConstFunctionName, Params, Args, F, *I);
      } else {
        Constant *idVal = ConstantInt::get(i32Type, APInt(32, getRegisterID(op1)));
        Args[0] = idVal;
        Instrument::createDseFunction(DSERegisterFunctionName, Params, Args, F, *I);
      }
      if (ConstantInt *i = dyn_cast<ConstantInt>(op2)) {
        Args[0] = i;
        Instrument::createDseFunction(DSEConstFunctionName, Params, Args, F, *I);
      } else {
        Constant *idVal = ConstantInt::get(i32Type, APInt(32, getRegisterID(op2)));
        Args[0] = idVal;
        Instrument::createDseFunction(DSERegisterFunctionName, Params, Args, F, *I);
      }

      int id = getRegisterID(I);
      int opcode = CmpI->getPredicate();
      Type *i32Type = IntegerType::getInt32Ty(M->getContext());
      Constant *idVal = ConstantInt::get(i32Type, APInt(32, id));
      Constant *opcodeVal = ConstantInt::get(i32Type, APInt(32, opcode));

      std::vector<Type *> ParamsICmp(2, Type::getInt32Ty(M->getContext()));
      std::vector<Value *> ArgsICmp(2);
      ArgsICmp[0] = idVal;
      ArgsICmp[1] = opcodeVal;
      Instrument::createDseFunction(DSEICmpFunctionName, ParamsICmp, ArgsICmp, F, *I);
    }
    else if (BranchInst *BI = dyn_cast<BranchInst>(I))
    {
      outs() << "branch inst\n";

      if (!BI->isConditional()) {
        continue;
      }

      Value *condition = BI->getCondition();
      int id = getRegisterID(condition);
      Constant *idVal = ConstantInt::get(i32Type, APInt(32, id));
      idVal->dump();

      int branchId = getBranchID(I);
      Constant *branchIdVal = ConstantInt::get(i32Type, APInt(32, branchId));
      branchIdVal->dump();

      std::vector<Type *> Params(3);
      std::vector<Value *> Args(3);
      Params[0] = Type::getInt32Ty(M->getContext());
      Params[1] = Type::getInt32Ty(M->getContext());
      Params[2] = Type::getInt1Ty(M->getContext());
      Args[0] = branchIdVal;
      Args[1] = idVal;
      Args[2] = condition;
      Instrument::createDseFunction(DSEBranchFunctionName, Params, Args, F, *I);
    }
    else if (CallInst *CI = dyn_cast<CallInst>(I))
    {
      outs() << "call inst\n";
    }
    else if (AllocaInst *AI = dyn_cast<AllocaInst>(I))
    {
      outs() << "alloca inst\n";
      int id = getRegisterID(I);
      Type *i32Type = IntegerType::getInt32Ty(M->getContext());
      Constant *idVal = ConstantInt::get(i32Type, APInt(32, id));

      std::vector<Type *> Params(2);
      std::vector<Value *> Args(2);
      Params[0] = IntegerType::getInt32Ty(M->getContext());
      Params[1] = PointerType::getUnqual(IntegerType::getInt32Ty(M->getContext()));
      Args[0] = idVal;
      Args[1] = AI;
      Instrument::createDseFunction(DSEAllocaFunctionName, Params, Args, F, *I, false);
    }
    else if (LoadInst *LI = dyn_cast<LoadInst>(I))
    {
      outs() << "load inst\n";
      Value *FromPtr = LI->getPointerOperand();
      int id = getRegisterID(LI);
      Type *i32Type = IntegerType::getInt32Ty(M->getContext());
      Constant *idVal = ConstantInt::get(i32Type, APInt(32, id));

      LI->getType()->dump();
      std::vector<Type *> Params(2);
      std::vector<Value *> Args(2);
      Params[0] = IntegerType::getInt32Ty(M->getContext());
      Params[1] = PointerType::getUnqual(IntegerType::getInt32Ty(M->getContext()));
      Args[0] = idVal;
      Args[1] = FromPtr;
      Instrument::createDseFunction(DSELoadFunctionName, Params, Args, F, *I);
    }
    else if (StoreInst *SI = dyn_cast<StoreInst>(I))
    {
      outs() << "store inst\n";
      // for a store, setup:
      Value *Ptr = SI->getPointerOperand(); // index 1
      Value *Val = SI->getValueOperand(); // index 0
      Ptr->dump();
      Val->dump();

      std::vector<Type *> ParamsConstReg(1);
      std::vector<Value *> ArgsConstReg(1);
      // setup register or const for value operand
      if (ConstantInt *i = dyn_cast<ConstantInt>(Val)) {
        ParamsConstReg[0] = IntegerType::getInt32Ty(M->getContext());
        ArgsConstReg[0] = i;
        Instrument::createDseFunction(DSEConstFunctionName, ParamsConstReg, ArgsConstReg, F, *I);
      } else {
        ParamsConstReg[0] = IntegerType::getInt32Ty(M->getContext());
        Constant *idVal = ConstantInt::get(i32Type, APInt(32, getRegisterID(Val)));
        ArgsConstReg[0] = idVal;
        Instrument::createDseFunction(DSERegisterFunctionName, ParamsConstReg, ArgsConstReg, F, *I);
      }

      std::vector<Type *> Params(1);
      std::vector<Value *> Args(1);
      Params[0] = PointerType::get(IntegerType::getInt32Ty(M->getContext()), SI->getPointerAddressSpace());
      Args[0] = Ptr;
      Instrument::createDseFunction(DSEStoreFunctionName, Params, Args, F, *I);
    }
  }

  outs() << "final function\n";
  F.dump();

  return true;
}

char Instrument::ID = 1;
static RegisterPass<Instrument>
    X("Instrument", "Instrumentations for Dynamic Symbolic Execution", false,
      false);

} // namespace instrument
