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
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options) {
  // XXX Build the triple from the arguments.
  // This is hard-coded for now for this example target.
  return "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32-a:0:32-n32";
}

LEGTargetMachine::LEGTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Reloc::Model RM, CodeModel::Model CM,
                                   CodeGenOpt::Level OL)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options), TT, CPU, FS,
                        Options, RM, CM, OL),
      Subtarget(TT, CPU, FS, *this),
      TLOF(make_unique<TargetLoweringObjectFileELF>()) {
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

  virtual bool addPreISel() override;
  virtual bool addInstSelector() override;
  virtual void addPreEmitPass() override;
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

void LEGPassConfig::addPreEmitPass() {}

// Force static initialization.
extern "C" void LLVMInitializeLEGTarget() {
  RegisterTargetMachine<LEGTargetMachine> X(TheLEGTarget);
}
