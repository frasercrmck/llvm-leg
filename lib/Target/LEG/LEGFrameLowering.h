//===-- LEGFrameLowering.h - Frame info for LEG Target ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains LEG frame information that doesn't fit anywhere else
// cleanly...
//
//===----------------------------------------------------------------------===//

#ifndef LEGFRAMEINFO_H
#define LEGFRAMEINFO_H

#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class LEGSubtarget;

class LEGFrameLowering : public TargetFrameLowering {
public:
  LEGFrameLowering();

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF) const;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const;

  void eliminateCallFramePseudoInstr(MachineFunction &MF,
                                     MachineBasicBlock &MBB,
                                     MachineBasicBlock::iterator I) const;

  bool hasFP(const MachineFunction &MF) const;

  //! Stack slot size (4 bytes)
  static int stackSlotSize() { return 4; }

private:
  uint64_t computeStackSize(MachineFunction &MF) const;
};
}

#endif // LEGFRAMEINFO_H

