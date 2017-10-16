//===-- LEGSelectionDAGInfo.h - LEG SelectionDAG Info -----------*- C++ -*-===//
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

#ifndef LIB_TARGET_LEG_LEGSELECTIONDAGINFO_H
#define LIB_TARGET_LEG_LEGSELECTIONDAGINFO_H

#include "llvm/CodeGen/SelectionDAGTargetInfo.h"

namespace llvm {

class LEGSelectionDAGInfo : public SelectionDAGTargetInfo {
public:
  explicit LEGSelectionDAGInfo() = default;

  ~LEGSelectionDAGInfo();
};
}

#endif // LIB_TARGET_LEG_LEGSELECTIONDAGINFO_H
