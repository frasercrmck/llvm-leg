//===-- LegAsmPrinter.cpp - Leg LLVM assembly writer ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the XAS-format Leg assembly language.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "asm-printer"
#include "Leg.h"
#include "InstPrinter/LegInstPrinter.h"
#include "LegInstrInfo.h"
#include "LegMCInstLower.h"
#include "LegSubtarget.h"
#include "LegTargetMachine.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineJumpTableInfo.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include <algorithm>
#include <cctype>
using namespace llvm;

namespace {
class LegAsmPrinter : public AsmPrinter {
  const LegSubtarget &Subtarget;
  LegMCInstLower MCInstLowering;

public:
  explicit LegAsmPrinter(TargetMachine &TM, MCStreamer &Streamer)
      : AsmPrinter(TM, Streamer), Subtarget(TM.getSubtarget<LegSubtarget>()),
        MCInstLowering(*this) {}

  virtual const char *getPassName() const { return "Leg Assembly Printer"; }

  void EmitFunctionEntryLabel();
  void EmitInstruction(const MachineInstr *MI);
  void EmitFunctionBodyStart();
};
} // end of anonymous namespace

void LegAsmPrinter::EmitFunctionBodyStart() {
  MCInstLowering.Initialize(Mang, &MF->getContext());
}

void LegAsmPrinter::EmitFunctionEntryLabel() {
  OutStreamer.EmitLabel(CurrentFnSym);
}

void LegAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);

  EmitToStreamer(OutStreamer, TmpInst);
}

// Force static initialization.
extern "C" void LLVMInitializeLegAsmPrinter() {
  RegisterAsmPrinter<LegAsmPrinter> X(TheLegTarget);
}
