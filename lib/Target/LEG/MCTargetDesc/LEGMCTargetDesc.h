//===-- LEGMCTargetDesc.h - LEG Target Descriptions ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides LEG specific target descriptions.
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

extern Target TheLEGTarget;

MCCodeEmitter *createLEGMCCodeEmitter(const MCInstrInfo &MCII,
                                      const MCRegisterInfo &MRI,
                                      const MCSubtargetInfo &STI,
                                      MCContext &Ctx);

MCAsmBackend *createLEGAsmBackend(const Target &T, const MCRegisterInfo &MRI,
                                  StringRef TT, StringRef CPU);

MCObjectWriter *createLEGELFObjectWriter(raw_ostream &OS, uint8_t OSABI);

} // End llvm namespace

// Defines symbolic names for LEG registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "LEGGenRegisterInfo.inc"

// Defines symbolic names for the LEG instructions.
//
#define GET_INSTRINFO_ENUM
#include "LEGGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "LEGGenSubtargetInfo.inc"

#endif
