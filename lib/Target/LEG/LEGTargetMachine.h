//===-- LEGTargetMachine.h - Define TargetMachine for LEG -------*- C++ -*-===//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the LEG specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LIB_TARGET_LEG_LEGTARGETMACHINE_H
#define LIB_TARGET_LEG_LEGTARGETMACHINE_H

#include "LEG.h"
#include "LEGFrameLowering.h"
#include "LEGISelLowering.h"
#include "LEGInstrInfo.h"
#include "LEGSelectionDAGInfo.h"
#include "LEGSubtarget.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class LEGTargetMachine : public LLVMTargetMachine {
  LEGSubtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  LEGTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                   CodeGenOpt::Level OL, bool JIT);

  const LEGSubtarget *getSubtargetImpl() const { return &Subtarget; }

  virtual const TargetSubtargetInfo *
  getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }

  // Pass Pipeline Configuration
  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
};

} // end namespace llvm

#endif // LIB_TARGET_LEG_LEGTARGETMACHINE_H
