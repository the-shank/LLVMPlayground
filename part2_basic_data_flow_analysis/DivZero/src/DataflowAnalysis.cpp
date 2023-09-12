#include "DataflowAnalysis.h"
#include <llvm-12/llvm/Support/Debug.h>
#include <string>

using namespace llvm;

namespace dataflow {

//===----------------------------------------------------------------------===//
// Memory Access
//===----------------------------------------------------------------------===//

const char *WhiteSpaces = " \t\n\r";

std::string variable(Value *V) {
  std::string Code;
  raw_string_ostream SS(Code);
  V->print(SS);
  Code.erase(0, Code.find_first_not_of(WhiteSpaces));
  return Code;
}

//===----------------------------------------------------------------------===//
// Dataflow Analysis Implementation
//===----------------------------------------------------------------------===//

DataflowAnalysis::DataflowAnalysis(char ID) : FunctionPass(ID) {}

void DataflowAnalysis::collectErrorInsts(Function &F) {
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    if (check(&*I))
      ErrorInsts.insert(&*I);
  }
}

bool DataflowAnalysis::runOnFunction(Function &F) {
  outs() << "Running " << getAnalysisName() << " on " << F.getName() << "\n";
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    InMap[&(*I)] = new Memory;
    OutMap[&(*I)] = new Memory;
  }

  doAnalysis(F);

  collectErrorInsts(F);
  outs() << "Potential Instructions by " << getAnalysisName() << ": \n";
  for (auto I : ErrorInsts) {
    outs() << *I << "\n";
  }

  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    delete InMap[&(*I)];
    delete OutMap[&(*I)];
  }
  return false;
}

std::set<std::string>
DataflowAnalysis::collect_out_vars(const std::vector<Instruction *> &preds) {
  std::set<std::string> vars;

  // dbgs() << "   preds:\n";
  for (auto I : preds) {
    // dbgs() << *I << "\n";
    for (auto it = OutMap[I]->begin(); it != OutMap[I]->end(); it++) {
      vars.insert(it->first);
    }
  }

  return vars;
}

void DataflowAnalysis::cloneMemory(const Memory &A, Memory &B) {
  for (auto it : A) {
    std::string var = it.first;
    Domain *d = it.second;
    B[var] = new Domain(d->Value);
  }
}

Memory *DataflowAnalysis::cloneMemory(const Memory &M) {
  Memory *Result = new Memory;
  for (auto it : M) {
    std::string var = it.first;
    Domain *d = it.second;
    (*Result)[var] = new Domain(d->Value);
  }
  return Result;
}
} // namespace dataflow
