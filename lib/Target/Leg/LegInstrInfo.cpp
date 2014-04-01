//===-- LegInstrInfo.cpp - Leg Instruction Information ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Leg implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "LegInstrInfo.h"
#include "Leg.h"
#include "LegMachineFunctionInfo.h"
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
#include "LegGenInstrInfo.inc"

using namespace llvm;

// Pin the vtable to this file.
void LegInstrInfo::anchor() {}

LegInstrInfo::LegInstrInfo()
  : LegGenInstrInfo(Leg::ADJCALLSTACKDOWN, Leg::ADJCALLSTACKUP),
    RI() {
}

/// isLoadFromStackSlot - If the specified machine instruction is a direct
/// load from a stack slot, return the virtual or physical register number of
/// the destination along with the FrameIndex of the loaded stack slot.  If
/// not, return 0.  This predicate must return 0 if the instruction has
/// any side effects other than loading from the stack slot.
unsigned
LegInstrInfo::isLoadFromStackSlot(const MachineInstr *MI, int &FrameIndex) const{
  assert(0 && "Unimplemented");
  return 0;
}
  
  /// isStoreToStackSlot - If the specified machine instruction is a direct
  /// store to a stack slot, return the virtual or physical register number of
  /// the source reg along with the FrameIndex of the loaded stack slot.  If
  /// not, return 0.  This predicate must return 0 if the instruction has
  /// any side effects other than storing to the stack slot.
unsigned
LegInstrInfo::isStoreToStackSlot(const MachineInstr *MI,
                                   int &FrameIndex) const {
  assert(0 && "Unimplemented");
  return 0;
}

//===----------------------------------------------------------------------===//
// Branch Analysis
//===----------------------------------------------------------------------===//
//
/// AnalyzeBranch - Analyze the branching code at the end of MBB, returning
/// true if it cannot be understood (e.g. it's a switch dispatch or isn't
/// implemented for a target).  Upon success, this returns false and returns
/// with the following information in various cases:
///
/// 1. If this block ends with no branches (it just falls through to its succ)
///    just return false, leaving TBB/FBB null.
/// 2. If this block ends with only an unconditional branch, it sets TBB to be
///    the destination block.
/// 3. If this block ends with an conditional branch and it falls through to
///    an successor block, it sets TBB to be the branch destination block and a
///    list of operands that evaluate the condition. These
///    operands can be passed to other TargetInstrInfo methods to create new
///    branches.
/// 4. If this block ends with an conditional branch and an unconditional
///    block, it returns the 'true' destination in TBB, the 'false' destination
///    in FBB, and a list of operands that evaluate the condition. These
///    operands can be passed to other TargetInstrInfo methods to create new
///    branches.
///
/// Note that RemoveBranch and InsertBranch must be implemented to support
/// cases where this method returns success.
///
bool
LegInstrInfo::AnalyzeBranch(MachineBasicBlock &MBB, MachineBasicBlock *&TBB,
                              MachineBasicBlock *&FBB,
                              SmallVectorImpl<MachineOperand> &Cond,
                              bool AllowModify) const {
  // Can't handle this.
  return true;
}

unsigned
LegInstrInfo::RemoveBranch(MachineBasicBlock &MBB) const {
  assert(0 && "Unimplemented");
  return 0;
}

void LegInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator I, DebugLoc DL,
                                 unsigned DestReg, unsigned SrcReg,
                                 bool KillSrc) const {
  BuildMI(MBB, I, I->getDebugLoc(), get(Leg::MOVrr), DestReg)
      .addReg(SrcReg, getKillRegState(KillSrc));
}

void LegInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                         MachineBasicBlock::iterator I,
                                         unsigned SrcReg, bool isKill,
                                         int FrameIndex,
                                         const TargetRegisterClass *RC,
                                         const TargetRegisterInfo *TRI) const
{
  BuildMI(MBB, I, I->getDebugLoc(), get(Leg::STORE))
    .addReg(SrcReg, getKillRegState(isKill))
    .addFrameIndex(FrameIndex).addImm(0);
}

void LegInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator I,
                                          unsigned DestReg, int FrameIndex,
                                          const TargetRegisterClass *RC,
                                          const TargetRegisterInfo *TRI) const
{
  BuildMI(MBB, I, I->getDebugLoc(), get(Leg::LOAD), DestReg)
      .addFrameIndex(FrameIndex).addImm(0);
}

bool LegInstrInfo::expandPostRAPseudo(MachineBasicBlock::iterator MI) const {
  assert(0 && "Unimplemented");
  return false;
}
