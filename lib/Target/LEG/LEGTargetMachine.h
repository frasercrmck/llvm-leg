//===-- LEGTargetMachine.h - Define TargetMachine for LEG ---*- C++ -*-===//
//
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

#ifndef LEGTARGETMACHINE_H
#define LEGTARGETMACHINE_H

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

public:
  LEGTargetMachine(const Target &T, StringRef TT, StringRef CPU, StringRef FS,
                   const TargetOptions &Options, Reloc::Model RM,
                   CodeModel::Model CM, CodeGenOpt::Level OL);

  virtual const LEGSubtarget *getSubtargetImpl() const { return &Subtarget; }

  // Pass Pipeline Configuration
  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM);

  virtual void addAnalysisPasses(PassManagerBase &PM);
};

} // end namespace llvm

#endif
