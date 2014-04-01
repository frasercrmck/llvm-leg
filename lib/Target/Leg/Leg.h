//===-- Leg.h - Top-level interface for Leg representation --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// Leg back-end.
//
//===----------------------------------------------------------------------===//

#ifndef TARGET_LEG_H
#define TARGET_LEG_H

#include "MCTargetDesc/LegMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class TargetMachine;
class LegTargetMachine;

FunctionPass *createLegISelDag(LegTargetMachine &TM,
                               CodeGenOpt::Level OptLevel);
} // end namespace llvm;

#endif
