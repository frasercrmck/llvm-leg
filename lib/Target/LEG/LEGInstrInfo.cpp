//===-- LEGInstrInfo.cpp - LEG Instruction Information ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LEG implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "LEGInstrInfo.h"
#include "LEG.h"
#include "LEGMachineFunctionInfo.h"
#include "MCTargetDesc/LEGBaseInfo.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/MC/MCContext.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "LEGGenInstrInfo.inc"

using namespace llvm;

// Pin the vtable to this file.
void LEGInstrInfo::anchor() {}

LEGInstrInfo::LEGInstrInfo(LEGSubtarget &STI)
    : LEGGenInstrInfo(LEG::ADJCALLSTACKDOWN, LEG::ADJCALLSTACKUP), RI() {}

/// isLoadFromStackSlot - If the specified machine instruction is a direct
/// load from a stack slot, return the virtual or physical register number of
/// the destination along with the FrameIndex of the loaded stack slot.  If
/// not, return 0.  This predicate must return 0 if the instruction has
/// any side effects other than loading from the stack slot.
unsigned LEGInstrInfo::isLoadFromStackSlot(const MachineInstr &MI,
                                           int &FrameIndex) const {
  assert(0 && "Unimplemented");
  return 0;
}

/// isStoreToStackSlot - If the specified machine instruction is a direct
/// store to a stack slot, return the virtual or physical register number of
/// the source reg along with the FrameIndex of the loaded stack slot.  If
/// not, return 0.  This predicate must return 0 if the instruction has
/// any side effects other than storing to the stack slot.
unsigned LEGInstrInfo::isStoreToStackSlot(const MachineInstr &MI,
                                          int &FrameIndex) const {
  assert(0 && "Unimplemented");
  return 0;
}

//===----------------------------------------------------------------------===//
// Branch Analysis
//===----------------------------------------------------------------------===//
//
/// analyzeBranch - Analyze the branching code at the end of MBB, returning
// true if it cannot be understood (e.g. it's a switch dispatch or isn't
/// implemented for a target).  Upon success, this returns false and returns
/// with the following information in various cases:
///
/// 1. If this block ends with no branches (it just falls through to its
/// succ)
///    just return false, leaving TBB/FBB null.
/// 2. If this block ends with only an unconditional branch, it sets TBB to
/// be
///    the destination block.
/// 3. If this block ends with an conditional branch and it falls through to
///    an successor block, it sets TBB to be the branch destination block
///    and a
///    list of operands that evaluate the condition. These
///    operands can be passed to other TargetInstrInfo methods to create new
///    branches.
/// 4. If this block ends with an conditional branch and an unconditional
///    block, it returns the 'true' destination in TBB, the 'false'
///    destination
///    in FBB, and a list of operands that evaluate the condition. These
///    operands can be passed to other TargetInstrInfo methods to create new
///    branches.
///
/// Note that removeBranch and insertBranch must be implemented to support
/// cases where this method returns success.
///
bool LEGInstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                 MachineBasicBlock *&TBB,
                                 MachineBasicBlock *&FBB,
                                 SmallVectorImpl<MachineOperand> &Cond,
                                 bool AllowModify) const {
  bool HasCondBranch = false;
  TBB = nullptr;
  FBB = nullptr;
  for (MachineInstr &MI : MBB) {
    if (MI.getOpcode() == LEG::B) {
      MachineBasicBlock *TargetBB = MI.getOperand(0).getMBB();
      if (HasCondBranch) {
        FBB = TargetBB;
      } else {
        TBB = TargetBB;
      }
    } else if (MI.getOpcode() == LEG::Bcc) {
      MachineBasicBlock *TargetBB = MI.getOperand(1).getMBB();
      TBB = TargetBB;
      Cond.push_back(MI.getOperand(0));
      HasCondBranch = true;
    }
  }
  return false;
}

/// removeBranch - Remove the branching code at the end of the specific MBB.
/// This is only invoked in cases where analyzeBranch returns success. It
/// returns the number of instructions that were removed.
unsigned LEGInstrInfo::removeBranch(MachineBasicBlock &MBB,
                                    int *BytesRemoved) const {
  assert(!BytesRemoved && "code size not handled");

  if (MBB.empty())
    return 0;
  unsigned NumRemoved = 0;
  auto I = MBB.end();
  do {
    --I;
    unsigned Opc = I->getOpcode();
    if ((Opc == LEG::B) || (Opc == LEG::Bcc)) {
      auto ToDelete = I;
      ++I;
      MBB.erase(ToDelete);
      NumRemoved++;
    }
  } while (I != MBB.begin());
  return NumRemoved;
}

/// insertBranch - Insert branch code into the end of the specified
/// MachineBasicBlock.  The operands to this method are the same as those
/// returned by analyzeBranch.  This is only invoked in cases where
/// analyzeBranch returns success. It returns the number of instructions
/// inserted.
///
/// It is also invoked by tail merging to add unconditional branches in
/// cases where analyzeBranch doesn't apply because there was no original
/// branch to analyze.  At least this much must be implemented, else tail
/// merging needs to be disabled.
unsigned LEGInstrInfo::insertBranch(MachineBasicBlock &MBB,
                                    MachineBasicBlock *TBB,
                                    MachineBasicBlock *FBB,
                                    ArrayRef<MachineOperand> Cond,
                                    const DebugLoc &DL, int *BytesAdded) const {
  // Shouldn't be a fall through.
  assert(TBB && "insertBranch must not be told to insert a fallthrough");
  assert(!BytesAdded && "code size not handled");
  unsigned NumInserted = 0;

  // Insert any conditional branch.
  if (Cond.size() > 0) {
    const unsigned CC = Cond[0].getImm();
    BuildMI(&MBB, DL, get(LEG::Bcc)).addMBB(TBB).addImm(CC);
    NumInserted++;
  }

  // Insert any unconditional branch.
  if (Cond.empty() || FBB) {
    BuildMI(MBB, MBB.end(), DL, get(LEG::B)).addMBB(Cond.empty() ? TBB : FBB);
    NumInserted++;
  }
  return NumInserted;
}

void LEGInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                               MachineBasicBlock::iterator I,
                               const DebugLoc &DL, unsigned DestReg,
                               unsigned SrcReg, bool KillSrc) const {
  BuildMI(MBB, I, I->getDebugLoc(), get(LEG::MOVrr), DestReg)
      .addReg(SrcReg, getKillRegState(KillSrc));
}

void LEGInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                       MachineBasicBlock::iterator I,
                                       unsigned SrcReg, bool isKill,
                                       int FrameIndex,
                                       const TargetRegisterClass *RC,
                                       const TargetRegisterInfo *TRI) const {
  BuildMI(MBB, I, I->getDebugLoc(), get(LEG::STR))
      .addReg(SrcReg, getKillRegState(isKill))
      .addFrameIndex(FrameIndex)
      .addImm(0);
}

void LEGInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator I,
                                        unsigned DestReg, int FrameIndex,
                                        const TargetRegisterClass *RC,
                                        const TargetRegisterInfo *TRI) const {
  BuildMI(MBB, I, I->getDebugLoc(), get(LEG::LDR), DestReg)
      .addFrameIndex(FrameIndex)
      .addImm(0);
}

bool LEGInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  switch (MI.getOpcode()) {
  default:
    return false;
  case LEG::MOVi32: {
    DebugLoc DL = MI.getDebugLoc();
    MachineBasicBlock &MBB = *MI.getParent();

    const unsigned DstReg = MI.getOperand(0).getReg();
    const bool DstIsDead = MI.getOperand(0).isDead();

    const MachineOperand &MO = MI.getOperand(1);

    auto LO16 = BuildMI(MBB, MI, DL, get(LEG::MOVLOi16), DstReg);
    auto HI16 =
        BuildMI(MBB, MI, DL, get(LEG::MOVHIi16))
            .addReg(DstReg, RegState::Define | getDeadRegState(DstIsDead))
            .addReg(DstReg);

    if (MO.isImm()) {
      const unsigned Imm = MO.getImm();
      const unsigned Lo16 = Imm & 0xffff;
      const unsigned Hi16 = (Imm >> 16) & 0xffff;
      LO16 = LO16.addImm(Lo16);
      HI16 = HI16.addImm(Hi16);
    } else {
      const GlobalValue *GV = MO.getGlobal();
      const unsigned TF = MO.getTargetFlags();
      LO16 = LO16.addGlobalAddress(GV, MO.getOffset(), TF | LEGII::MO_LO16);
      HI16 = HI16.addGlobalAddress(GV, MO.getOffset(), TF | LEGII::MO_HI16);
    }

    MBB.erase(MI);
    return true;
  }
  }
}
