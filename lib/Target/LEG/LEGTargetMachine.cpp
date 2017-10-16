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
#include "LEGISelLowering.h"
#include "LEGInstrInfo.h"
#include "LEGSelectionDAGInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

// Force static initialization.
extern "C" void LLVMInitializeLEGTarget() {
  RegisterTargetMachine<LEGTargetMachine> X(getTheLEGTarget());
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options) {
  // XXX Build the triple from the arguments.
  // This is hard-coded for now for this example target.
  return "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32-a:0:32-n32";
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue()) {
    return Reloc::Static;
  }
  return *RM;
}

static CodeModel::Model getEffectiveCodeModel(Optional<CodeModel::Model> CM) {
  if (CM) {
    return *CM;
  }
  return CodeModel::Small;
}

LEGTargetMachine::LEGTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Optional<Reloc::Model> RM,
                                   Optional<CodeModel::Model> CM,
                                   CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options), TT, CPU, FS,
                        Options, getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM), OL),
      Subtarget(TT, CPU, FS, *this),
      TLOF(make_unique<TargetLoweringObjectFileELF>()) {
  initAsmInfo();
}

namespace {
/// LEG Code Generator Pass Configuration Options.
class LEGPassConfig : public TargetPassConfig {
public:
  LEGPassConfig(LEGTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  LEGTargetMachine &getLEGTargetMachine() const {
    return getTM<LEGTargetMachine>();
  }

  bool addPreISel() override;
  bool addInstSelector() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *LEGTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new LEGPassConfig(*this, PM);
}

bool LEGPassConfig::addPreISel() { return false; }

bool LEGPassConfig::addInstSelector() {
  addPass(createLEGISelDag(getLEGTargetMachine()));
  return false;
}

void LEGPassConfig::addPreEmitPass() {}
