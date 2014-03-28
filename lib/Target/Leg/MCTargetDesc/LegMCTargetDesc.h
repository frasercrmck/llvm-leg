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

#include "llvm/Support/DataTypes.h"

namespace llvm {
class Target;
class MCInstrInfo;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCContext;
class MCCodeEmitter;
class MCAsmInfo;
class MCCodeGenInfo;
class MCInstPrinter;
class MCObjectWriter;
class MCAsmBackend;

class StringRef;
class raw_ostream;

extern Target TheLegTarget;

MCCodeEmitter *createLegMCCodeEmitter(const MCInstrInfo &MCII,
                                      const MCRegisterInfo &MRI,
                                      const MCSubtargetInfo &STI,
                                      MCContext &Ctx);

MCAsmBackend *createLegAsmBackend(const Target &T, const MCRegisterInfo &MRI,
                                  StringRef TT, StringRef CPU);

MCObjectWriter *createLegELFObjectWriter(raw_ostream &OS, uint8_t OSABI);

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
