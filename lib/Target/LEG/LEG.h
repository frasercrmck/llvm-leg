//==-- LEG.h - Top-level interface for LEG representation --------*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM LEG backend.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LEG_LEG_H
#define LLVM_LIB_TARGET_LEG_LEG_H

#include "MCTargetDesc/LEGMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class LEGTargetMachine;
class FunctionPass;

FunctionPass *createLEGISelDag(LEGTargetMachine &TM);
} // end namespace llvm;

#endif
