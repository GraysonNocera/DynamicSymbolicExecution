#include "Instrument.h"

using namespace llvm;

namespace instrument {

/*
 * Implement your instrumentation for dynamic symbolic execution engine
 */
bool Instrument::runOnFunction(Function &F) {
  outs() << "Running " << getAnalysisName() << " on " << F.getName() << "\n";


  // insert "call void __DSE_Init__();"
  // before the first command
  inst_iterator I = inst_begin(F);
  Module *M = F.getParent();
  std::vector<Type *> Params(0);
  std::vector<Value *> Args(0);
  FunctionType *FT = FunctionType::get(Type::getVoidTy(M->getContext()), Params, false);
  FunctionCallee f = M->getOrInsertFunction(DSEInitFunctionName, FT);
  CallInst *Call = CallInst::Create(f, Args, "", &I);
  Call->setCallingConv(CallingConv::C);
  Call->setTailCall(true);

  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    // do something for each instruction
  }

  return true;
}

char Instrument::ID = 1;
static RegisterPass<Instrument>
    X("Instrument", "Instrumentations for Dynamic Symbolic Execution", false,
      false);

} // namespace instrument
