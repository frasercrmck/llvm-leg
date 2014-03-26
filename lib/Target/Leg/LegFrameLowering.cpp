//===-- LegFrameLowering.cpp - Frame info for Leg Target --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains Leg frame information that doesn't fit anywhere else
// cleanly...
//
//===----------------------------------------------------------------------===//

#include "LegFrameLowering.h"
#include "Leg.h"
#include "LegInstrInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetLowering.h"
#include "llvm/Target/TargetOptions.h"
#include <algorithm> // std::sort

using namespace llvm;

//===----------------------------------------------------------------------===//
// LegFrameLowering:
//===----------------------------------------------------------------------===//
LegFrameLowering::LegFrameLowering(const LegSubtarget &ST)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, 4, 0) {
  // Do nothing
}

bool LegFrameLowering::hasFP(const MachineFunction &MF) const {
  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
         MF.getFrameInfo()->hasVarSizedObjects();
}

uint64_t LegFrameLowering::computeStackSize(MachineFunction &MF) const {
  MachineFrameInfo *MFI = MF.getFrameInfo();
  uint64_t StackSize = MFI->getStackSize();
  unsigned StackAlign = getStackAlignment();
  if (StackAlign > 0)
    StackSize = RoundUpToAlignment(StackSize, StackAlign);
  return StackSize;
}

void LegFrameLowering::emitPrologue(MachineFunction &MF) const {
  const TargetInstrInfo &TII = *MF.getTarget().getInstrInfo();
  MachineBasicBlock &MBB = MF.front();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  uint64_t StackSize = computeStackSize(MF);
  unsigned StackReg = Leg::SP;
  const uint64_t MaxSubImm = 0xfff;
  if (StackSize <= MaxSubImm) {
    // The stack offset fits in the SUB instruction.
    BuildMI(MBB, MBBI, dl, TII.get(Leg::SUB_ri), StackReg).addReg(StackReg)
      .addImm(StackSize).setMIFlag(MachineInstr::FrameSetup);
  } else {
    // The stack offset does not fit in the SUB instruction.
    // Materialize the offset using MOVW/MOVT.
    unsigned OffsetReg = Leg::R4;
    unsigned StackSizeLo = (unsigned)(StackSize & 0xffff);
    unsigned StackSizeHi = (unsigned)((StackSize & 0xffff0000) >> 16);
    BuildMI(MBB, MBBI, dl, TII.get(Leg::MOVWi16), OffsetReg).addImm(StackSizeLo)
      .setMIFlag(MachineInstr::FrameSetup);
    if (StackSizeHi) {
      BuildMI(MBB, MBBI, dl, TII.get(Leg::MOVTi16), OffsetReg).addReg(OffsetReg)
        .addImm(StackSizeLo).setMIFlag(MachineInstr::FrameSetup);
    }
    BuildMI(MBB, MBBI, dl, TII.get(Leg::SUB), StackReg).addReg(StackReg)
      .addReg(OffsetReg).setMIFlag(MachineInstr::FrameSetup);
  }
}

void LegFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  assert(0 && "Unimplemented");
}

// This function eliminates ADJCALLSTACKDOWN, ADJCALLSTACKUP pseudo
// instructions
void LegFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  assert(0 && "Unimplemented");
}
