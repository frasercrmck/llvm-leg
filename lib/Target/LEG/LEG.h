//===-- LEG.h - Top-level interface for LEG representation --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// LEG back-end.
//
//===----------------------------------------------------------------------===//

#ifndef TARGET_LEG_H
#define TARGET_LEG_H

#include "MCTargetDesc/LEGMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class TargetMachine;
class LEGTargetMachine;

FunctionPass *createLEGISelDag(LEGTargetMachine &TM,
                               CodeGenOpt::Level OptLevel);
} // end namespace llvm;

#endif
