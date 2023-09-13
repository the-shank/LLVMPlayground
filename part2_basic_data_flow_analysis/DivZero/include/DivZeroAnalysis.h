#ifndef DIV_ZERO_ANALYSIS_H
#define DIV_ZERO_ANALYSIS_H

#include "DataflowAnalysis.h"
#include <llvm-12/llvm/IR/InstrTypes.h>
#include <llvm-12/llvm/IR/Instructions.h>

namespace dataflow {
struct DivZeroAnalysis : public DataflowAnalysis {
  static char ID;
  DivZeroAnalysis() : DataflowAnalysis(ID) {}

protected:
  void transfer(Instruction *I, const Memory *In, Memory *NOut) override;

  void doAnalysis(Function &F) override;

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
  // Domain *getDomain(Value *v, const Memory &In);
};
} // namespace dataflow

#endif // DIV_ZERO_ANALYSIS_H
