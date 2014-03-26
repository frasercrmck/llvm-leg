//===-- LegMCTargetDesc.h - Leg Target Descriptions ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Leg specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LEGMCTARGETDESC_H
#define LEGMCTARGETDESC_H

namespace llvm {
class Target;

extern Target TheLegTarget;

} // End llvm namespace

// Defines symbolic names for Leg registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "LegGenRegisterInfo.inc"

// Defines symbolic names for the Leg instructions.
//
#define GET_INSTRINFO_ENUM
#include "LegGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "LegGenSubtargetInfo.inc"

#endif
