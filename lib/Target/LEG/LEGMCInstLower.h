//===-- LEGMCInstLower.h - Lower MachineInstr to MCInst --------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LIB_TARGET_LEG_LEGMCINSTLOWER_H
#define LIB_TARGET_LEG_LEGMCINSTLOWER_H
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class MCContext;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineFunction;
class Mangler;
class AsmPrinter;

/// \brief This class is used to lower an MachineInstr into an MCInst.
class LLVM_LIBRARY_VISIBILITY LEGMCInstLower {
  typedef MachineOperand::MachineOperandType MachineOperandType;
  MCContext *Ctx;
  AsmPrinter &Printer;

public:
  LEGMCInstLower(MCContext *Ctx, AsmPrinter &Printer)
      : Ctx(Ctx), Printer(Printer) {}
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;

  void Initialize(MCContext *C);

  MCOperand LowerOperand(const MachineOperand &MO, unsigned offset = 0) const;

private:
  MCOperand LowerSymbolOperand(const MachineOperand &MO,
                               MachineOperandType MOTy, unsigned Offset) const;
};
}

#endif // LIB_TARGET_LEG_LEGMCINSTLOWER_H
