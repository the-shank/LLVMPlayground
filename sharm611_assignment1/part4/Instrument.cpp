#include "Instrument.h"
#include <llvm-12/llvm/IR/DebugLoc.h>
#include <llvm-12/llvm/Support/Debug.h>
#include <llvm-12/llvm/Support/raw_ostream.h>

using namespace llvm;

namespace instrument {

static const char *SanitizerFunctionName = "__sanitize__";
static const char *CoverageFunctionName = "__coverage__";

/*
 * Implement divide-by-zero sanitizer.
 */
void instrumentSanitize(Module *M, Function &F, Instruction &I) {
  /* Add you code here */
  // only required for div instructions
  if (BinaryOperator *BO = dyn_cast<BinaryOperator>(&I)) {
    if (BO->getOpcode() == llvm::Instruction::UDiv ||
        BO->getOpcode() == llvm::Instruction::SDiv) {

      // get sanitizer function
      Type *void_type = Type::getVoidTy(M->getContext());
      Type *int_type = Type::getInt32Ty(M->getContext());
      Value *NewValue = M->getOrInsertFunction(SanitizerFunctionName, void_type,
                                               int_type, int_type, int_type)
                            .getCallee();
      Function *SanitizerFunc = cast<Function>(NewValue);

      // prepare args
      auto divisor = BO->getOperand(1);
      // errs() << ">> divisor: " << *divisor << "\n";
      // errs().flush();
      const DebugLoc &Debug = I.getDebugLoc();
      int linenum = Debug.getLine();
      int colnum = Debug.getCol();
      Value *line = ConstantInt::get(M->getContext(), APInt(32, linenum, true));
      Value *col = ConstantInt::get(M->getContext(), APInt(32, colnum, true));
      std::vector<Value *> Args;
      Args.push_back(divisor);
      Args.push_back(line);
      Args.push_back(col);

      // create the call to sanitizer
      CallInst *Call = CallInst::Create(SanitizerFunc, Args, "", &I);
      Call->setCallingConv(CallingConv::C);
      Call->setTailCall(true);
    }
  }
}

/*
 * Implement code coverage instrumentation.
 */
void instrumentCoverage(Module *M, Function &F, Instruction &I) {
  /* Add you code here */
  // only do this for instructions that have a debug information
  const DebugLoc &Debug = I.getDebugLoc();
  if (Debug) {
    // get coverage function
    Type *void_type = Type::getVoidTy(M->getContext());
    Type *int_type = Type::getInt32Ty(M->getContext());
    Value *NewValue = M->getOrInsertFunction(CoverageFunctionName, void_type,
                                             int_type, int_type)
                          .getCallee();
    Function *CoverageFunc = cast<Function>(NewValue);

    // prepare args
    int linenum = Debug.getLine();
    int colnum = Debug.getCol();
    Value *line = ConstantInt::get(M->getContext(), APInt(32, linenum, true));
    Value *col = ConstantInt::get(M->getContext(), APInt(32, colnum, true));
    std::vector<Value *> Args;
    Args.push_back(line);
    Args.push_back(col);

    // create the call to sanitizer
    CallInst *Call = CallInst::Create(CoverageFunc, Args, "", &I);
    Call->setCallingConv(CallingConv::C);
    Call->setTailCall(true);
  }
}

bool Instrument::runOnFunction(Function &F) {
  /* Add you code here */
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    // errs() << *I << "\n";
    instrumentCoverage(F.getParent(), F, *I);
    instrumentSanitize(F.getParent(), F, *I);
  }
  return true;
}

char Instrument::ID = 1;
static RegisterPass<Instrument>
    X("Instrument", "Instrumentations for Dynamic Analysis", false, false);

} // namespace instrument
