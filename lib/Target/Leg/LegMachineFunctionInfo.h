//===-- LegMachineFuctionInfo.h - Leg machine function info -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares Leg-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LEGMACHINEFUNCTIONINFO_H
#define LEGMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

// Forward declarations
class Function;

/// LegFunctionInfo - This class is derived from MachineFunction private
/// Leg target-specific information for each MachineFunction.
class LegFunctionInfo : public MachineFunctionInfo {
public:
  LegFunctionInfo() {}

  ~LegFunctionInfo() {}
};
} // End llvm namespace

#endif // LEGMACHINEFUNCTIONINFO_H

