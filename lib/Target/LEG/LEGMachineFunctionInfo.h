//===-- LEGMachineFuctionInfo.h - LEG machine function info -*- C++ -*-===//
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

#ifndef LEGMACHINEFUNCTIONINFO_H
#define LEGMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

// Forward declarations
class Function;

/// LEGFunctionInfo - This class is derived from MachineFunction private
/// LEG target-specific information for each MachineFunction.
class LEGFunctionInfo : public MachineFunctionInfo {
public:
  LEGFunctionInfo() {}

  ~LEGFunctionInfo() {}
};
} // End llvm namespace

#endif // LEGMACHINEFUNCTIONINFO_H

