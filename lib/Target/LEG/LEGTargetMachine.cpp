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
#include "LEGFrameLowering.h"
#include "LEGInstrInfo.h"
#include "LEGISelLowering.h"
#include "LEGSelectionDAGInfo.h"
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
      Subtarget(TT, CPU, FS, *this) {
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
