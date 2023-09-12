#include "DivZeroAnalysis.h"
#include "DataflowAnalysis.h"
#include <cassert>
#include <cstdlib>
#include <llvm-12/llvm/IR/InstrTypes.h>
#include <llvm-12/llvm/IR/Instruction.h>
#include <llvm-12/llvm/IR/Instructions.h>
#include <llvm-12/llvm/IR/ValueMap.h>
#include <llvm-12/llvm/Support/Debug.h>
#include <set>
#include <string>

namespace dataflow {

//===----------------------------------------------------------------------===//
// DivZero Analysis Implementation
//===----------------------------------------------------------------------===//

/**
 * Implement your data-flow analysis.
 * 1. Define "flowIn" that joins the memory set of all incoming flows
 * 2. Define "transfer" that computes the semantics of each instruction.
 * 3. Define "flowOut" that flows the memory set to all outgoing flows
 * 4. Define "doAnalysis" that stores your results in "InMap" and "OutMap".
 * 5. Define "check" that checks if a given instruction is erroneous or not.
 */

// shank helpers start ---------------------------------------------------

void printMemory(const Memory *M) {
  errs() << "Memory[ ";
  for (auto it : *M) {
    std::string var = it.first;
    Domain *d = it.second;
    errs() << "(" << var << " => " << d->to_string() << "), ";
  }
  errs() << " ]\n";
}

// shank helpers end ---------------------------------------------------

// define the following functions if needed (not compulsory to do so)
Memory *join(Memory *M1, Memory *M2) {
  errs() << "join >>\n";
  errs() << "M1: ";
  printMemory(M1);
  errs() << "M2: ";
  printMemory(M2);

  Memory *Result = new Memory();

  /* Add your code here */
  // get the variables in the memories
  std::set<std::string> vars;
  for (auto it : *M1) {
    vars.insert(it.first);
  }
  for (auto it : *M2) {
    vars.insert(it.first);
  }

  // merge the variables into the result
  for (auto v : vars) {
    errs() << "v: " << v << "\n";
    if (M1->find(v) != M1->end()) {
      if (Result->find(v) == Result->end()) {
        // new var
        Domain *d = M1->at(v);
        Result->insert(std::pair<std::string, Domain *>(v, d));
      } else {
        // existing var
        Domain *d1 = Result->at(v);
        Domain *d2 = M1->at(v);
        Result->at(v) = Domain::join(d1, d2);
      }
    }
    if (M2->find(v) != M2->end()) {
      if (Result->find(v) == Result->end()) {
        // new var
        Domain *d = M2->at(v);
        Result->insert(std::pair<std::string, Domain *>(v, d));
      } else {
        // existing var
        Domain *d1 = Result->at(v);
        Domain *d2 = M2->at(v);
        Result->at(v) = Domain::join(d1, d2);
      }
    }
  }

  /* Result will be the union of memories M1 and M2 */
  errs() << "Result: ";
  printMemory(Result);
  return Result;
}

bool equal(Memory *M1, Memory *M2) {
  /* Add your code here */
  /* Return true if the two memories M1 and M2 are equal */

  // same variables?
  std::set<std::string> vars_m1;
  for (auto it : *M1) {
    vars_m1.insert(it.first);
  }
  std::set<std::string> vars_m2;
  for (auto it : *M2) {
    vars_m2.insert(it.first);
  }
  if (!(vars_m1 == vars_m2)) {
    return false;
  }

  // same domain for each variable?
  for (auto it : *M1) {
    std::string v = it.first;
    if (M1->at(v) != M2->at(v)) {
      return false;
    }
  }

  return true;
}

void DivZeroAnalysis::flowIn(Instruction *I, Memory *In) {
  errs() << "flowIn >>\n";
  /* Add your code here */
  // preds of I
  std::vector<Instruction *> preds = getPredecessors(I);

  errs() << "previous mem: ";
  printMemory(In);

  // clear out the previous InMem
  InMap[I]->clear();

  // which variables are defined in the OutMem of all the preds?
  std::set<std::string> vars = collect_out_vars(preds);

  // for each var, join the in-vars
  for (const std::string &v : vars) {
    for (auto &pred : preds) {
      dbgs() << "pred: " << *pred << "\n";
      InMap[I] = join(InMap[I], OutMap[pred]);
    }
  }

  dbgs() << "after mem: ";
  // printMemory(In);
  printMemory(InMap[I]);

  errs() << "flowIn <<\n";
}

Domain *getDomain(Value *v, const Memory &In) {
  if (Constant *C = dyn_cast<Constant>(v)) {
    // get the domain from constant value
    if (C->isZeroValue()) {
      return new Domain(Domain::Zero);
    } else {
      return new Domain(Domain::NonZero);
    }
  } else {
    // get the domain from in memory
    const std::string var = variable(v);
    return In.at(var);
  }
}

void DivZeroAnalysis::handleBinaryOperator(BinaryOperator *BO, const Memory &In,
                                           Memory &Out) {
  // add | sub | mul | div
  auto op = BO->getOpcode();
  const std::string var = variable(BO);

  Value *op1 = BO->getOperand(0);
  Domain *d1 = getDomain(op1, In);

  Value *op2 = BO->getOperand(1);
  Domain *d2 = getDomain(op2, In);

  switch (op) {
  // case llvm::Instruction::BinaryOpsBegin:
  // case llvm::Instruction::FAdd:
  case llvm::Instruction::Sub: {
    Out[var] = Domain::sub(d1, d2);
    break;
  }
  // case llvm::Instruction::FSub:
  case llvm::Instruction::Mul:
    Out[var] = Domain::mul(d1, d2);
    break;
  // case llvm::Instruction::FMul:
  case llvm::Instruction::UDiv:
  case llvm::Instruction::SDiv:
    Out[var] = Domain::div(d1, d2);
    break;
  // case llvm::Instruction::FDiv:
  // case llvm::Instruction::URem:
  // case llvm::Instruction::SRem:
  // case llvm::Instruction::FRem:
  // case llvm::Instruction::Shl:
  // case llvm::Instruction::LShr:
  // case llvm::Instruction::AShr:
  case llvm::Instruction::And:
    Out[var] = Domain::add(d1, d2);
    break;
  // case llvm::Instruction::Or:
  // case llvm::Instruction::Xor:
  // case llvm::Instruction::BinaryOpsEnd:
  default:
    errs() << "[!] opcode not supported yet: " << op << "\n";
    exit(EXIT_FAILURE);
    break;
  }
}

void DivZeroAnalysis::handleCastInst(CastInst *CI, const Memory &In,
                                     Memory &Out) {
  errs() << "handleCastInst >>\n";

  CI->dump();

  errs() << "In: ";
  printMemory(&In);
  errs() << "Out: ";
  printMemory(&Out);

  // we are not here to check invalid casts, so the out domain would just be
  // the domain to which we are casting

  // based on the operand of cast, change the domain of the output

  if (CI->isIntegerCast()) {
    errs() << "num operands: " << CI->getNumOperands() << "\n";
    auto operand = CI->getOperand(0);
    auto var = variable(operand);
    errs() << "var: " << var << "\n";
    errs() << "In.at(var): " << In.at(var)->to_string() << "\n";
    auto instVar = variable(CI);
    Out[instVar] = In.at(var);
  }

  errs() << "OutMap[CI]: ";
  printMemory(OutMap[CI]);

  errs() << "handleCastInst <<\n";
}

void DivZeroAnalysis::handleCmpInst(CmpInst *CmpI, const Memory &In,
                                    Memory &Out) {
  errs() << "handleCmpInst: \n";

  const std::string varInst = variable(CmpI);

  // Type *typ = CmpI->getType();
  Value *op1 = CmpI->getOperand(0);
  Value *op2 = CmpI->getOperand(1);

  // if both are constants
  //    if both have the same value then NonZero
  //    else Zero
  // else MaybeZero

  Constant *c1 = dyn_cast<Constant>(op1);
  Constant *c2 = dyn_cast<Constant>(op2);
  if (c1 && c2) {
    errs() << "c1->getValueID(): " << c1->getValueID() << "\n";
    errs() << "c2->getValueID(): " << c2->getValueID() << "\n";
    if (c1->getValueID() == c2->getValueID()) {
      Out[varInst] = new Domain(Domain::NonZero);
    } else {
      Out[varInst] = new Domain(Domain::Zero);
    }
  } else {
    Out[varInst] = new Domain(Domain::MaybeZero);
  }

  errs() << "out memory: ";
  printMemory(&Out);
}

void DivZeroAnalysis::handleBranchInst(BranchInst *BI, const Memory &In,
                                       Memory &Out) {
  // TODO: shank
  // but it would be easier to do this once we have a print chain established
  errs() << "handleBranchInst: not implemented yet\n";
  exit(EXIT_FAILURE);
}

void DivZeroAnalysis::handleUserInput(Instruction *I, const Memory &In,
                                      Memory &Out) {
  // TODO: shank
  // but it would be easier to do this once we have a print chain established
  errs() << "handleUserInput: not implemented yet\n";
  exit(EXIT_FAILURE);
}

void DivZeroAnalysis::transfer(Instruction *I, const Memory *In, Memory *Out) {
  errs() << "transfer >>\n";

  errs() << "In: ";
  printMemory(In);
  errs() << "Out: ";
  printMemory(Out);

  /* Add your code here */

  // first copy over the in-memory to out-memory
  Out->clear();
  cloneMemory(*In, *Out);

  // you should handle:
  // - BinaryOperators (add, mul, sub, etc.)
  // - CastInst
  // - CmpInst (icmp eq, ne, slt, sgt, sge, etc.)
  // - BranchInst
  // - user input via getchar() - recall from above that this is handled using
  //    isInput() from DivZero/include/DataflowAnalysis.h

  // TODO: shank: wip
  // - find the instruction type
  // - do the transfer
  //    - clear out the OutMem
  //    - copy InMem to OutMem
  //    - update OutMem as per the insturtion's transfer function
  if (BinaryOperator *BO = dyn_cast<BinaryOperator>(I)) {
    dbgs() << ">> binary operator:\n";
    // BO->dump();
    handleBinaryOperator(BO, *In, *Out);

  } else if (CastInst *CI = dyn_cast<CastInst>(I)) {
    dbgs() << ">> cast inst:\n";
    // CI->dump();
    handleCastInst(CI, *In, *Out);

  } else if (CmpInst *CmpI = dyn_cast<CmpInst>(I)) {
    // dbgs() << ">> cmp inst:\n";
    // CmpI->dump();
    handleCmpInst(CmpI, *In, *Out);

  } else if (BranchInst *BI = dyn_cast<BranchInst>(I)) {
    dbgs() << ">> branch inst:\n";
    // BI->dump();
    handleBranchInst(BI, *In, *Out);

  } else if (isInput(I)) {
    dbgs() << ">> user input:\n";
    // I->dump();
    handleUserInput(I, *In, *Out);

  } else {
    // do nothing
    // OUT <- IN
    for (auto it : *In) {
      Out->at(it.first) = it.second;
    }
  }
}

void DivZeroAnalysis::flowOut(Instruction *I, Memory *Pre, Memory *Post,
                              SetVector<Instruction *> &WorkSet) {
  errs() << "flowOut >>\n";

  errs() << "Pre: ";
  printMemory(Pre);
  errs() << "Post: ";
  printMemory(Post);

  /* Add your code here */
  // - pre and post should have the same keys
  // - pre and post should have the same values
  std::set<std::string> pre_vars;
  for (auto it : *Pre) {
    pre_vars.insert(it.first);
  }
  std::set<std::string> post_vars;
  for (auto it : *Post) {
    post_vars.insert(it.first);
  }
  if (pre_vars != post_vars) {
    WorkSet.insert(I);
  }
}

void DivZeroAnalysis::doAnalysis(Function &F) {
  SetVector<Instruction *> WorkSet;
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    WorkSet.insert(&(*I));
  }

  /* Add your code here */
  /* Basic Workflow-
       Visit instruction in WorkSet
       For each visited instruction I, construct its In memory by joining all
     memory sets of incoming flows (predecessors of I) Based on the type of
     instruction I and the In memory, populate the NOut memory Based on the
     previous Out memory and the current Out memory, check if there is a
     difference between the two and flow the memory set appropriately to all
     successors of I and update WorkSet accordingly
  */
  while (!WorkSet.empty()) {
    // get an item from the workset
    Instruction *I = WorkSet.front();
    WorkSet.remove(I);
    errs() << "\ninst >>>>>>>\n";
    dbgs() << ">> inst: " << *I << "\n";

    // flowin
    flowIn(I, InMap[I]);

    errs() << "after flowIn --\n";
    errs() << "InMap[I]: ";
    printMemory(InMap[I]);

    // transfer
    auto Pre = cloneMemory(*OutMap[I]);
    transfer(I, InMap[I], OutMap[I]);
    auto Post = OutMap[I];

    // flowout
    flowOut(I, Pre, Post, WorkSet);

    errs() << "finally for this instruction: \n";
    errs() << "InMap[I]: ";
    printMemory(InMap[I]);
    errs() << "OutMap[I]: ";
    printMemory(OutMap[I]);

    errs() << "inst <<<<<<<\n";
  }
}

bool DivZeroAnalysis::check(Instruction *I) {
  /* Add your code here */

  // - is a div instruction
  // - divisor has either a Zero or MaybeZero domain
  if (BinaryOperator *BO = dyn_cast<BinaryOperator>(I)) {
    bool isDivInst = false;
    auto op = BO->getOpcode();
    if (op == llvm::Instruction::SDiv || op == llvm::Instruction::UDiv) {
      isDivInst = true;
    }

    if (isDivInst) {
      Value *divisor = BO->getOperand(1);
      if (Constant *C = dyn_cast<Constant>(divisor)) {
        // constant 0 as the divisor?
        if (C->isZeroValue()) {
          return true;
        }

      } else {
        // divisor is a variable which is either Zero or MaybeZero?
        auto divisor_var = variable(divisor);
        if (OutMap[I]->find(divisor_var) != OutMap[I]->end()) {
          Domain *d = OutMap[I]->at(divisor_var);
          if (d->Value == Domain::Zero || d->Value == Domain::MaybeZero) {
            return true;
          }
        }
      }
    }
  }

  return false;
}

char DivZeroAnalysis::ID = 1;
static RegisterPass<DivZeroAnalysis> X("DivZero", "Divide-by-zero Analysis",
                                       false, false);
} // namespace dataflow
