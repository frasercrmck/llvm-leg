//===-- LEGRegisterInfo.cpp - LEG Register Information ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LEG implementation of the MRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "LEGRegisterInfo.h"
#include "LEG.h"
#include "LEGInstrInfo.h"
#include "LEGMachineFunctionInfo.h"
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
#include "LEGGenRegisterInfo.inc"

using namespace llvm;

LEGRegisterInfo::LEGRegisterInfo() : LEGGenRegisterInfo(LEG::LR) {}

const uint16_t *
LEGRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  static const uint16_t CalleeSavedRegs[] = { LEG::R4, LEG::R5, LEG::R6,
                                              LEG::R7, LEG::R8, LEG::R9,
                                              0 };
  return CalleeSavedRegs;
}

BitVector LEGRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(LEG::SP);
  Reserved.set(LEG::LR);
  return Reserved;
}

const uint32_t *LEGRegisterInfo::getCallPreservedMask(CallingConv::ID) const {
  return CC_Save_RegMask;
}

bool
LEGRegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const {
  return true;
}

bool
LEGRegisterInfo::trackLivenessAfterRegAlloc(const MachineFunction &MF) const {
  return true;
}

bool LEGRegisterInfo::useFPForScavengingIndex(const MachineFunction &MF) const {
  return false;
}

void LEGRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
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
  case LEG::LDR:
  case LEG::STR:
    ImmOpIdx = FIOperandNum + 1;
    break;
  }

  // FIXME: check the size of offset.
  MachineOperand &ImmOp = MI.getOperand(ImmOpIdx);
  int Offset = MFI->getObjectOffset(FI) + MFI->getStackSize() + ImmOp.getImm();
  FIOp.ChangeToRegister(LEG::SP, false);
  ImmOp.setImm(Offset);
}

unsigned LEGRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return LEG::SP;
}
