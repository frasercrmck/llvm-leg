//===-- LEGMCTargetDesc.cpp - LEG Target Descriptions -----------------===//
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

#include "LEGMCTargetDesc.h"
#include "InstPrinter/LEGInstPrinter.h"
#include "LEGMCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "LEGGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "LEGGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "LEGGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createLEGMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitLEGMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createLEGMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitLEGMCRegisterInfo(X, LEG::LR);
  return X;
}

static MCSubtargetInfo *createLEGMCSubtargetInfo(const Triple &TT,
                                                 StringRef CPU, StringRef FS) {
  return createLEGMCSubtargetInfoImpl(TT, CPU, FS);
}

static MCAsmInfo *createLEGMCAsmInfo(const MCRegisterInfo &MRI,
                                     const Triple &TT) {
  return new LEGMCAsmInfo(TT);
}

static MCInstPrinter *createLEGMCInstPrinter(const Triple &TT,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  return new LEGInstPrinter(MAI, MII, MRI);
}

// Force static initialization.
extern "C" void LLVMInitializeLEGTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfoFn X(getTheLEGTarget(), createLEGMCAsmInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(getTheLEGTarget(), createLEGMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(getTheLEGTarget(), createLEGMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(getTheLEGTarget(),
                                          createLEGMCSubtargetInfo);
  // Register the MCCodeEmitter
  TargetRegistry::RegisterMCCodeEmitter(getTheLEGTarget(),
                                        createLEGMCCodeEmitter);

  // Register the ASM Backend.
  TargetRegistry::RegisterMCAsmBackend(getTheLEGTarget(), createLEGAsmBackend);

  // Register the MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(getTheLEGTarget(),
                                        createLEGMCInstPrinter);
}
