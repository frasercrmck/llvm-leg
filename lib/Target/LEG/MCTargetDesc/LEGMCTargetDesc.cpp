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
#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCStreamer.h"
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

static MCRegisterInfo *createLEGMCRegisterInfo(StringRef TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitLEGMCRegisterInfo(X, LEG::LR);
  return X;
}

static MCSubtargetInfo *createLEGMCSubtargetInfo(StringRef TT, StringRef CPU,
                                                 StringRef FS) {
  MCSubtargetInfo *X = new MCSubtargetInfo();
  InitLEGMCSubtargetInfo(X, TT, CPU, FS);
  return X;
}

static MCAsmInfo *createLEGMCAsmInfo(const MCRegisterInfo &MRI, StringRef TT) {
  MCAsmInfo *MAI = new LEGMCAsmInfo(TT);
  return MAI;
}

static MCCodeGenInfo *createLEGMCCodeGenInfo(StringRef TT, Reloc::Model RM,
                                             CodeModel::Model CM,
                                             CodeGenOpt::Level OL) {
  MCCodeGenInfo *X = new MCCodeGenInfo();
  if (RM == Reloc::Default) {
    RM = Reloc::Static;
  }
  if (CM == CodeModel::Default) {
    CM = CodeModel::Small;
  }
  if (CM != CodeModel::Small && CM != CodeModel::Large) {
    report_fatal_error("Target only supports CodeModel Small or Large");
  }

  X->InitMCCodeGenInfo(RM, CM, OL);
  return X;
}

static MCInstPrinter *
createLEGMCInstPrinter(const Target &T, unsigned SyntaxVariant,
                       const MCAsmInfo &MAI, const MCInstrInfo &MII,
                       const MCRegisterInfo &MRI, const MCSubtargetInfo &STI) {
  return new LEGInstPrinter(MAI, MII, MRI);
}

static MCStreamer *
createMCAsmStreamer(MCContext &Ctx, formatted_raw_ostream &OS,
                    bool isVerboseAsm, bool useDwarfDirectory,
                    MCInstPrinter *InstPrint, MCCodeEmitter *CE,
                    MCAsmBackend *TAB, bool ShowInst) {
  return createAsmStreamer(Ctx, OS, isVerboseAsm, useDwarfDirectory, InstPrint,
                           CE, TAB, ShowInst);
}

static MCStreamer *createMCStreamer(const Target &T, StringRef TT,
                                    MCContext &Ctx, MCAsmBackend &MAB,
                                    raw_ostream &OS,
                                    MCCodeEmitter *Emitter,
                                    const MCSubtargetInfo &STI,
                                    bool RelaxAll,
                                    bool NoExecStack) {
  return createELFStreamer(Ctx, MAB, OS, Emitter, false, NoExecStack);
}


// Force static initialization.
extern "C" void LLVMInitializeLEGTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfoFn X(TheLEGTarget, createLEGMCAsmInfo);

  // Register the MC codegen info.
  TargetRegistry::RegisterMCCodeGenInfo(TheLEGTarget, createLEGMCCodeGenInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(TheLEGTarget, createLEGMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheLEGTarget, createLEGMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(TheLEGTarget,
                                          createLEGMCSubtargetInfo);

  // Register the MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(TheLEGTarget, createLEGMCInstPrinter);

  // Register the ASM Backend.
  TargetRegistry::RegisterMCAsmBackend(TheLEGTarget, createLEGAsmBackend);

  // Register the assembly streamer.
  TargetRegistry::RegisterAsmStreamer(TheLEGTarget, createMCAsmStreamer);

  // Register the object streamer.
  TargetRegistry::RegisterMCObjectStreamer(TheLEGTarget, createMCStreamer);

  // Register the MCCodeEmitter
  TargetRegistry::RegisterMCCodeEmitter(TheLEGTarget, createLEGMCCodeEmitter);
}
