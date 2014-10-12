//===-- LEGSelectionDAGInfo.h - LEG SelectionDAG Info -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the LEG subclass for TargetSelectionDAGInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LEGSELECTIONDAGINFO_H
#define LEGSELECTIONDAGINFO_H

#include "llvm/Target/TargetSelectionDAGInfo.h"

namespace llvm {

class LEGSelectionDAGInfo : public TargetSelectionDAGInfo {
public:
  explicit LEGSelectionDAGInfo(const DataLayout &DL);
  ~LEGSelectionDAGInfo();
};
}

#endif
