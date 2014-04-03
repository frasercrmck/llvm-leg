//===-- LEGTargetInfo.cpp - LEG Target Implementation -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "LEG.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheLEGTarget;

extern "C" void LLVMInitializeLEGTargetInfo() {
  RegisterTarget<Triple::leg> X(TheLEGTarget, "leg", "LEG");
}
