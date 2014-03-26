//===-- LegRegisterInfo.cpp - Leg Register Information ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Leg implementation of the MRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "LegRegisterInfo.h"
#include "Leg.h"
#include "LegInstrInfo.h"
#include "LegMachineFunctionInfo.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

#define GET_REGINFO_TARGET_DESC
#include "LegGenRegisterInfo.inc"

using namespace llvm;

LegRegisterInfo::LegRegisterInfo() : LegGenRegisterInfo(Leg::LR) {}

const uint16_t *
LegRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  static const uint16_t CalleeSavedRegs[] = { Leg::R4, Leg::R5, Leg::R6,
                                              Leg::R7, Leg::R8, Leg::R9,
                                              0 };
  return CalleeSavedRegs;
}

BitVector LegRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(Leg::SP);
  Reserved.set(Leg::LR);
  return Reserved;
}

bool
LegRegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const {
  return true;
}

bool
LegRegisterInfo::trackLivenessAfterRegAlloc(const MachineFunction &MF) const {
  return true;
}

bool LegRegisterInfo::useFPForScavengingIndex(const MachineFunction &MF) const {
  return false;
}

void LegRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  MachineInstr &MI = *II;
  const MachineFunction &MF = *MI.getParent()->getParent();
  const MachineFrameInfo *MFI = MF.getFrameInfo();
  MachineOperand &FIOp = MI.getOperand(FIOperandNum);
  unsigned FI = FIOp.getIndex();

  // Determine if we can eliminate the index from this kind of instruction.
  unsigned ImmOpIdx = 0;
  switch (MI.getOpcode()) {
  default:
      // Not supported yet.
      return;
  case Leg::LOAD:
  case Leg::STORE:
      ImmOpIdx = FIOperandNum + 1;
      break;
  }

  // FIXME: check the size of offset.
  MachineOperand &ImmOp = MI.getOperand(ImmOpIdx);
  int Offset = MFI->getObjectOffset(FI) + MFI->getStackSize() + ImmOp.getImm();
  FIOp.ChangeToRegister(Leg::SP, false);
  ImmOp.setImm(Offset);
}

unsigned LegRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return Leg::SP;
}
