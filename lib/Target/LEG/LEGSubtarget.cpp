//===-- LEGSubtarget.cpp - LEG Subtarget Information ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the LEG specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "LEGSubtarget.h"
#include "LEG.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "LEGGenSubtargetInfo.inc"

using namespace llvm;

void LEGSubtarget::anchor() {}

LEGSubtarget::LEGSubtarget(const std::string &TT, const std::string &CPU,
                           const std::string &FS)
    : LEGGenSubtargetInfo(TT, CPU, FS) {}
