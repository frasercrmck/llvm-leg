//===-- LegTargetMachine.cpp - Define TargetMachine for Leg -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "LegTargetMachine.h"
#include "Leg.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

LegTargetMachine::LegTargetMachine(const Target &T, StringRef TT, StringRef CPU,
                                   StringRef FS, const TargetOptions &Options,
                                   Reloc::Model RM, CodeModel::Model CM,
                                   CodeGenOpt::Level OL)
    : LLVMTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL),
      Subtarget(TT, CPU, FS),
      DL("e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32-a:0:32-n32"),
      InstrInfo(), FrameLowering(Subtarget), TLInfo(*this), TSInfo(*this) {
  initAsmInfo();
}

namespace {
/// Leg Code Generator Pass Configuration Options.
class LegPassConfig : public TargetPassConfig {
public:
  LegPassConfig(LegTargetMachine *TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  LegTargetMachine &getLegTargetMachine() const {
    return getTM<LegTargetMachine>();
  }

  virtual bool addPreISel();
  virtual bool addInstSelector();
  virtual bool addPreEmitPass();
};
} // namespace

TargetPassConfig *LegTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new LegPassConfig(this, PM);
}

bool LegPassConfig::addPreISel() { return false; }

bool LegPassConfig::addInstSelector() {
  addPass(createLegISelDag(getLegTargetMachine(), getOptLevel()));
  return false;
}

bool LegPassConfig::addPreEmitPass() { return false; }

// Force static initialization.
extern "C" void LLVMInitializeLegTarget() {
  RegisterTargetMachine<LegTargetMachine> X(TheLegTarget);
}

void LegTargetMachine::addAnalysisPasses(PassManagerBase &PM) {}
