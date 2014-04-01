//===-- LegMCTargetDesc.cpp - Leg Target Descriptions -----------------===//
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

#include "LegMCTargetDesc.h"
#include "InstPrinter/LegInstPrinter.h"
#include "LegMCAsmInfo.h"
#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "LegGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "LegGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "LegGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createLegMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitLegMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createLegMCRegisterInfo(StringRef TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitLegMCRegisterInfo(X, Leg::LR);
  return X;
}

static MCSubtargetInfo *createLegMCSubtargetInfo(StringRef TT, StringRef CPU,
                                                 StringRef FS) {
  MCSubtargetInfo *X = new MCSubtargetInfo();
  InitLegMCSubtargetInfo(X, TT, CPU, FS);
  return X;
}

static MCAsmInfo *createLegMCAsmInfo(const MCRegisterInfo &MRI, StringRef TT) {
  MCAsmInfo *MAI = new LegMCAsmInfo(TT);
  return MAI;
}

static MCCodeGenInfo *createLegMCCodeGenInfo(StringRef TT, Reloc::Model RM,
                                             CodeModel::Model CM,
                                             CodeGenOpt::Level OL) {
  MCCodeGenInfo *X = new MCCodeGenInfo();
  if (RM == Reloc::Default) {
    RM = Reloc::Static;
  }
  if (CM == CodeModel::Default) {
    CM = CodeModel::Small;
  }
  if (CM != CodeModel::Small && CM != CodeModel::Large)
    report_fatal_error("Target only supports CodeModel Small or Large");

  X->InitMCCodeGenInfo(RM, CM, OL);
  return X;
}

static MCInstPrinter *
createLegMCInstPrinter(const Target &T, unsigned SyntaxVariant,
                       const MCAsmInfo &MAI, const MCInstrInfo &MII,
                       const MCRegisterInfo &MRI, const MCSubtargetInfo &STI) {
  return new LegInstPrinter(MAI, MII, MRI);
}

static MCStreamer *
createMCAsmStreamer(MCContext &Ctx, formatted_raw_ostream &OS,
                       bool isVerboseAsm, bool useCFI, bool useDwarfDirectory,
                       MCInstPrinter *InstPrint, MCCodeEmitter *CE,
                       MCAsmBackend *TAB, bool ShowInst) {
  return createAsmStreamer(Ctx, OS, isVerboseAsm, useCFI, useDwarfDirectory,
                           InstPrint, CE, TAB, ShowInst);
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
extern "C" void LLVMInitializeLegTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfoFn X(TheLegTarget, createLegMCAsmInfo);

  // Register the MC codegen info.
  TargetRegistry::RegisterMCCodeGenInfo(TheLegTarget, createLegMCCodeGenInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(TheLegTarget, createLegMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheLegTarget, createLegMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(TheLegTarget,
                                          createLegMCSubtargetInfo);

  // Register the MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(TheLegTarget, createLegMCInstPrinter);

  // Register the ASM Backend.
  TargetRegistry::RegisterMCAsmBackend(TheLegTarget, createLegAsmBackend);

  // Register the assembly streamer.
  TargetRegistry::RegisterAsmStreamer(TheLegTarget, createMCAsmStreamer);

  // Register the object streamer.
  TargetRegistry::RegisterMCObjectStreamer(TheLegTarget, createMCStreamer);

  // Register the MCCodeEmitter
  TargetRegistry::RegisterMCCodeEmitter(TheLegTarget, createLegMCCodeEmitter);
}
