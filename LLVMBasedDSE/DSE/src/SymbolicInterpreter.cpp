#include "SymbolicInterpreter.h"

#include <ctime>
#include <fstream>

// Function to print out the std::map<Address, z3::expr>
void printMapSI(const std::map<Address, z3::expr>& m) {
    for (const auto& pair : m) {
        std::cout << pair.first << " => " << pair.second.to_string() << std::endl;
    }
}
void print_map_si(const std::map<int, int>& myMap) {
    for (const auto& pair : myMap) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}

std::ostream &operator<<(std::ostream &OS, const Address &A) {
  if (A.Type == A.Memory) {
    OS << A.Addr;
  } else {
    OS << "R" << A.Addr;
  }
  return OS;
}

int SymbolicInterpreter::NewInput(int *Ptr, int ID) {
  std::cout << "NEW INPUT\n";
  int Ret = 0;
  if (Inputs.find(ID) != Inputs.end()) {
    Ret = Inputs[ID];
    std::cout << "found Ret = " << Ret << std::endl;
  } else {
    std::cout << "assigning random value\n";
    Ret = std::rand();
    Inputs[ID] = Ret;
  }
  Address X(Ptr);
  std::string InputName = "X" + std::to_string(NumOfInputs);
  z3::expr SE = Ctx.int_const(InputName.c_str());
  Mem.insert(std::make_pair(X, SE));
  printMapSI(Mem);
  NumOfInputs++;
  return Ret;
}

SymbolicInterpreter SI;

void print(std::ostream &OS) {
  OS << "=== Inputs ===" << std::endl;
  int Idx = 0;
  for (auto &E : SI.getInputs()) {
    OS << "X" << E.first << " : " << E.second << std::endl;
  }
  OS << std::endl;
  OS << "=== Symbolic Memory ===" << std::endl;
  for (auto &E : SI.getMemory()) {
    OS << E.first << " : " << E.second << std::endl;
  }
  OS << std::endl;

  OS << "=== Path Condition ===" << std::endl;
  for (auto &E : SI.getPathCondition()) {
    std::string BID = "B" + std::to_string(E.first);
    OS << BID << " : " << E.second << std::endl;
  }
}

extern "C" void __DSE_Exit__() {
  printf("at exit function\n");
  z3::solver Solver(SI.getContext());
  std::ofstream Branch(BranchFile);
  for (auto &E : SI.getPathCondition()) {
    std::string BID = "B" + std::to_string(E.first);
    Solver.add(E.second);
    Branch << BID << "\n";
  }
  std::ofstream Smt2(FormulaFile);
  Smt2 << Solver.to_smt2();
  std::ofstream Log(LogFile);
  print(Log);
}

extern "C" void __DSE_Init__() {
  std::cout << "INIT\n";
  std::srand(std::time(nullptr));
  std::string Line;
  std::ifstream Input(InputFile);
  if (Input.is_open()) {
    while (getline(Input, Line)) {
      int ID = std::stoi(Line.substr(1, Line.find(",")));
      int Val = std::stoi(Line.substr(Line.find(",") + 1));
      SI.getInputs()[ID] = Val;
    }
  }
  std::atexit(__DSE_Exit__);
  std::cout << "registered exit\n";
}

extern "C" void __DSE_Input__(int *X, int ID) { *X = (int)SI.NewInput(X, ID); }

extern "C" void __DSE_Branch__(int BID, int RID, int B) {
  std::cout << "BID, RID, B: " << BID << " " << RID << " " << B << " " << std::endl;
  MemoryTy &Mem = SI.getMemory();
  Address Addr(RID);
  z3::expr SE = Mem.at(Addr);
  // B is the condition itself, it will evaluate to 1 or 0
  z3::expr Cond =
      B ? SI.getContext().bool_val(true) : SI.getContext().bool_val(false);
  SI.getPathCondition().push_back(std::make_pair(BID, SE == Cond));
  std::cout << "after branch condition" << std::endl;
  printMapSI(Mem);
}

extern "C" void __DSE_Const__(int X) {
  z3::expr SE = SI.getContext().int_val(X);
  SI.getStack().push(SE);
}

extern "C" void __DSE_Register__(int X) {
  std::string Name = "R" + std::to_string(X);
  z3::expr SE = SI.getContext().int_const(Name.c_str());
  SI.getStack().push(SE);
}
