//===-- LEGMachineFuctionInfo.h - LEG machine function info ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares LEG-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LIB_TARGET_LEG_LEGMACHINEFUNCTIONINFO_H
#define LIB_TARGET_LEG_LEGMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

// Forward declarations
class Function;

/// LEGFunctionInfo - This class is derived from MachineFunction private
/// LEG target-specific information for each MachineFunction.
class LEGMachineFunctionInfo : public MachineFunctionInfo {
  virtual void anchor();

public:
  LEGMachineFunctionInfo() {}

  ~LEGMachineFunctionInfo() {}
};
} // End llvm namespace

#endif // LIB_TARGET_LEG_LEGMACHINEFUNCTIONINFO_H
