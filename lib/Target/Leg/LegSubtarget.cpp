//===-- LegSubtarget.cpp - Leg Subtarget Information ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the Leg specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "LegSubtarget.h"
#include "Leg.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "LegGenSubtargetInfo.inc"

using namespace llvm;

void LegSubtarget::anchor() {}

LegSubtarget::LegSubtarget(const std::string &TT, const std::string &CPU,
                           const std::string &FS)
    : LegGenSubtargetInfo(TT, CPU, FS) {}
