//===-- LegTargetMachine.h - Define TargetMachine for Leg ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Leg specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LEGTARGETMACHINE_H
#define LEGTARGETMACHINE_H

#include "LegFrameLowering.h"
#include "LegISelLowering.h"
#include "LegInstrInfo.h"
#include "LegSelectionDAGInfo.h"
#include "LegSubtarget.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class LegTargetMachine : public LLVMTargetMachine {
  LegSubtarget Subtarget;
  const DataLayout DL;
  LegInstrInfo InstrInfo;
  LegFrameLowering FrameLowering;
  LegTargetLowering TLInfo;
  LegSelectionDAGInfo TSInfo;

public:
  LegTargetMachine(const Target &T, StringRef TT, StringRef CPU, StringRef FS,
                   const TargetOptions &Options, Reloc::Model RM,
                   CodeModel::Model CM, CodeGenOpt::Level OL);

  virtual const LegInstrInfo *getInstrInfo() const { return &InstrInfo; }
  virtual const LegFrameLowering *getFrameLowering() const {
    return &FrameLowering;
  }
  virtual const LegSubtarget *getSubtargetImpl() const { return &Subtarget; }
  virtual const LegTargetLowering *getTargetLowering() const { return &TLInfo; }

  virtual const LegSelectionDAGInfo *getSelectionDAGInfo() const {
    return &TSInfo;
  }

  virtual const TargetRegisterInfo *getRegisterInfo() const {
    return &InstrInfo.getRegisterInfo();
  }
  virtual const DataLayout *getDataLayout() const { return &DL; }

  // Pass Pipeline Configuration
  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM);

  virtual void addAnalysisPasses(PassManagerBase &PM);
};

} // end namespace llvm

#endif
