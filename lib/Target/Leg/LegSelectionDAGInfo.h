//===-- LegSelectionDAGInfo.h - Leg SelectionDAG Info -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the Leg subclass for TargetSelectionDAGInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LEGSELECTIONDAGINFO_H
#define LEGSELECTIONDAGINFO_H

#include "llvm/Target/TargetSelectionDAGInfo.h"

namespace llvm {

class LegTargetMachine;

class LegSelectionDAGInfo : public TargetSelectionDAGInfo {
public:
  explicit LegSelectionDAGInfo(const LegTargetMachine &TM);
  ~LegSelectionDAGInfo();
};
}

#endif
