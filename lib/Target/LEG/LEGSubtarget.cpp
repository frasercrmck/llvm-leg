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

#define DEBUG_TYPE "leg-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "LEGGenSubtargetInfo.inc"

using namespace llvm;

void LEGSubtarget::anchor() {}

LEGSubtarget &LEGSubtarget::initializeSubtargetDependencies(StringRef CPU,
                                                            StringRef FS) {
  std::string CPUName = CPU;
  if (CPUName.empty())
    CPUName = "leg";

  ParseSubtargetFeatures(CPUName, FS);

  return *this;
}

LEGSubtarget::LEGSubtarget(const Triple &TT, const std::string &CPU,
                           const std::string &FS, const TargetMachine &TM)
    : LEGGenSubtargetInfo(TT, CPU, FS),
      DL("e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32-a:0:32-n32"),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)), TLInfo(TM, *this),
      FrameLowering() {}
