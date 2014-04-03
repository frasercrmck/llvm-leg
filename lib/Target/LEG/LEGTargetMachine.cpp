//===-- LEGTargetMachine.cpp - Define TargetMachine for LEG -----------===//
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

#include "LEGTargetMachine.h"
#include "LEG.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

LEGTargetMachine::LEGTargetMachine(const Target &T, StringRef TT, StringRef CPU,
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
/// LEG Code Generator Pass Configuration Options.
class LEGPassConfig : public TargetPassConfig {
public:
  LEGPassConfig(LEGTargetMachine *TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  LEGTargetMachine &getLEGTargetMachine() const {
    return getTM<LEGTargetMachine>();
  }

  virtual bool addPreISel();
  virtual bool addInstSelector();
  virtual bool addPreEmitPass();
};
} // namespace

TargetPassConfig *LEGTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new LEGPassConfig(this, PM);
}

bool LEGPassConfig::addPreISel() { return false; }

bool LEGPassConfig::addInstSelector() {
  addPass(createLEGISelDag(getLEGTargetMachine(), getOptLevel()));
  return false;
}

bool LEGPassConfig::addPreEmitPass() { return false; }

// Force static initialization.
extern "C" void LLVMInitializeLEGTarget() {
  RegisterTargetMachine<LEGTargetMachine> X(TheLEGTarget);
}

void LEGTargetMachine::addAnalysisPasses(PassManagerBase &PM) {}
