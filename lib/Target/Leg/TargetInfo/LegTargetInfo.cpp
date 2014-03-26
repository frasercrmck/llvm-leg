//===-- LegTargetInfo.cpp - Leg Target Implementation -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Leg.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheLegTarget;

extern "C" void LLVMInitializeLegTargetInfo() {
  RegisterTarget<Triple::leg> X(TheLegTarget, "leg", "Leg");
}
