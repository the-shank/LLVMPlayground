#ifndef DIV_ZERO_ANALYSIS_H
#define DIV_ZERO_ANALYSIS_H

#include "DataflowAnalysis.h"
#include "PointerAnalysis.h"
#include <llvm-12/llvm/IR/InstrTypes.h>
#include <llvm-12/llvm/IR/Instructions.h>

namespace dataflow {
struct DivZeroAnalysis : public DataflowAnalysis {
  static char ID;
  DivZeroAnalysis() : DataflowAnalysis(ID) {}

protected:
  void transfer(Instruction *I, const Memory *In, Memory *NOut,
                PointerAnalysis *PA, SetVector<Value *> PointerSet) override;

  void doAnalysis(Function &F, PointerAnalysis *PA) override;

  void flowIn(Instruction *I, Memory *In);

  void flowOut(Instruction *I, Memory *In, Memory *NOut,
               SetVector<Instruction *> &);

  bool check(Instruction *I) override;

  std::string getAnalysisName() override { return "DivZero"; }

  // shank: helpers
  void handleBinaryOperator(BinaryOperator *BO, const Memory &In, Memory &Out);
  void handleCastInst(CastInst *CI, const Memory &In, Memory &Out);
  void handleCmpInst(CmpInst *CmpI, const Memory &In, Memory &Out);
  void handleBranchInst(BranchInst *BI, const Memory &In, Memory &Out);
  void handlePHIInst(Instruction *I, const Memory &In, Memory &Out);
  void handleUserInput(Instruction *I, const Memory &In, Memory &Out);
  void handleExternalFn(Instruction *I, const Memory &In, Memory &Out);
  void setupArgs(Function &F, PointerAnalysis *PA,
                 SetVector<Value *> &PointerSet, Memory *InMem);
  void handleLoadInst(LoadInst *LI, const Memory &In, Memory &Out,
                      PointerAnalysis *PA, SetVector<Value *> &PointerSet);
  void handleStoreInst(StoreInst *SI, const Memory &In, Memory &Out,
                       PointerAnalysis *PA, SetVector<Value *> &PointerSet);
  void handleStoreInstInt(StoreInst *SI, const Memory &In, Memory &Out,
                          PointerAnalysis *PA, SetVector<Value *> &PointerSet);
  void handleStoreInstPtr(StoreInst *SI, const Memory &In, Memory &Out,
                          PointerAnalysis *PA, SetVector<Value *> &PointerSet);
  // Domain *getDomain(Value *v, const Memory &In);
};
} // namespace dataflow

#endif // REF_DIV_ZERO_ANALYSIS_H
