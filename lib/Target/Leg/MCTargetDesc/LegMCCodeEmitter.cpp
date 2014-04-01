//===-- Leg/LegMCCodeEmitter.cpp - Convert Leg code to machine code -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the LegMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "mccodeemitter"
#include "MCTargetDesc/LegMCTargetDesc.h"
#include "MCTargetDesc/LegFixupKinds.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

STATISTIC(MCNumEmitted, "Number of MC instructions emitted.");

namespace {
class LegMCCodeEmitter : public MCCodeEmitter {
  LegMCCodeEmitter(const LegMCCodeEmitter &) LLVM_DELETED_FUNCTION;
  void operator=(const LegMCCodeEmitter &) LLVM_DELETED_FUNCTION;
  const MCInstrInfo &MCII;
  const MCContext &CTX;

public:
  LegMCCodeEmitter(const MCInstrInfo &mcii, MCContext &ctx)
      : MCII(mcii), CTX(ctx) {}

  ~LegMCCodeEmitter() {}

  // getBinaryCodeForInstr - TableGen'erated function for getting the
  // binary encoding for an instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  /// getMachineOpValue - Return binary encoding of operand. If the machine
  /// operand requires relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  void EmitByte(unsigned char C, raw_ostream &OS) const { OS << (char)C; }

  void EmitConstant(uint64_t Val, unsigned Size, raw_ostream &OS) const {
    // Output the constant in little endian byte order.
    for (unsigned i = 0; i != Size; ++i) {
      EmitByte(Val & 255, OS);
      Val >>= 8;
    }
  }

  void EncodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;
};

} // end anonymous namespace

MCCodeEmitter *llvm::createLegMCCodeEmitter(const MCInstrInfo &MCII,
                                            const MCRegisterInfo &MRI,
                                            const MCSubtargetInfo &STI,
                                            MCContext &Ctx) {
  return new LegMCCodeEmitter(MCII, Ctx);
}

/// getMachineOpValue - Return binary encoding of operand. If the machine
/// operand requires relocation, record the relocation and return zero.
unsigned LegMCCodeEmitter::getMachineOpValue(const MCInst &MI,
                                             const MCOperand &MO,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  if (MO.isReg()) {
    unsigned Reg = MO.getReg();
    unsigned RegNo = CTX.getRegisterInfo()->getEncodingValue(Reg);
    return RegNo;
  } else if (MO.isImm()) {
    return static_cast<unsigned>(MO.getImm());
  }

  assert(MO.isExpr() && "unknown operand kind in printOperand");

  const MCExpr *Expr = MO.getExpr();
  MCExpr::ExprKind Kind = Expr->getKind();

  if (Kind == MCExpr::Binary) {
    Expr = static_cast<const MCBinaryExpr*>(Expr)->getLHS();
    Kind = Expr->getKind();
  }

  assert (Kind == MCExpr::SymbolRef);

  unsigned FixupKind;
  switch (cast<MCSymbolRefExpr>(Expr)->getKind()) {
  default:
    llvm_unreachable("Unknown fixup kind!");
  case MCSymbolRefExpr::VK_None: {
    // TODO: Hacky & not very clever - move this elsewhere
    const unsigned Opc = MI.getOpcode();
    assert((Opc == Leg::MOVTi16 || Opc == Leg::MOVWi16) &&
           "Unhandled instruction");
    FixupKind = Opc == Leg::MOVTi16 ? Leg::fixup_leg_movt_hi16_pcrel
                                    : Leg::fixup_leg_movw_lo16_pcrel;
    break;
  }
  }

  Fixups.push_back(MCFixup::Create(0, MO.getExpr(), MCFixupKind(FixupKind)));
  return 0;
}

void LegMCCodeEmitter::EncodeInstruction(const MCInst &MI, raw_ostream &OS,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  if (Desc.getSize() != 32)
    llvm_unreachable("Unexpected instruction size!");

  uint32_t Binary = getBinaryCodeForInstr(MI, Fixups, STI);

  EmitConstant(Binary, Desc.getSize(), OS);
  ++MCNumEmitted;
}

#include "LegGenMCCodeEmitter.inc"
