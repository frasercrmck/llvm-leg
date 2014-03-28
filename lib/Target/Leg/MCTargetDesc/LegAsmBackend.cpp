//===-- LegAsmBackend.cpp - Leg Assembler Backend -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/LegMCTargetDesc.h"
/*
#include "MCTargetDesc/LegAddressingModes.h"
#include "MCTargetDesc/LegBaseInfo.h"
*/
#include "MCTargetDesc/LegFixupKinds.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDirectives.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCMachObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/MC/MCSectionMachO.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ELF.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MachO.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

namespace {
class LegELFObjectWriter : public MCELFObjectTargetWriter {
public:
  LegELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit*/ false, OSABI, ELF::EM_LEG,
                              /*HasRelocationAddend*/ false) {}
};

class LegAsmBackend : public MCAsmBackend {
  bool isThumbMode;  // Currently emitting Thumb code.
public:
  LegAsmBackend(const Target &T, const StringRef TT) : MCAsmBackend() {}

  ~LegAsmBackend() { } 

  unsigned getNumFixupKinds() const override {
    return Leg::NumTargetFixupKinds;
  }

  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override {
    const static MCFixupKindInfo Infos[Leg::NumTargetFixupKinds] = {
      // This table *must* be in the order that the fixup_* kinds are defined in
      // LegFixupKinds.h.
      //
      // Name                      Offset (bits) Size (bits)     Flags
      { "fixup_leg_movt_hi16_pcrel",     0, 32, MCFixupKindInfo::FKF_IsPCRel },
      { "fixup_leg_movw_lo16_pcrel",     0, 32, MCFixupKindInfo::FKF_IsPCRel },
    };

    if (Kind < FirstTargetFixupKind)
      return MCAsmBackend::getFixupKindInfo(Kind);

    assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
           "Invalid kind!");
    return Infos[Kind - FirstTargetFixupKind];
  }

  /// processFixupValue - Target hook to process the literal value of a fixup
  /// if necessary.
  void processFixupValue(const MCAssembler &Asm, const MCAsmLayout &Layout,
                         const MCFixup &Fixup, const MCFragment *DF,
                         MCValue &Target, uint64_t &Value,
                         bool &IsResolved) override;


  void applyFixup(const MCFixup &Fixup, char *Data, unsigned DataSize,
                  uint64_t Value) const override;

  bool mayNeedRelaxation(const MCInst &Inst) const override;

  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override;

  void relaxInstruction(const MCInst &Inst, MCInst &Res) const override {}

  bool writeNopData(uint64_t Count, MCObjectWriter *OW) const override {
    if (Count == 0)
      return true;
    return false;
  }

  unsigned getPointerSize() const { return 4; }
};
} // end anonymous namespace

bool LegAsmBackend::mayNeedRelaxation(const MCInst &Inst) const {
  return false;
}

bool LegAsmBackend::fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                                         const MCRelaxableFragment *DF,
                                         const MCAsmLayout &Layout) const {
  return false;
}

static unsigned adjustFixupValue(const MCFixup &Fixup, uint64_t Value,
                                 MCContext *Ctx = NULL) {
  unsigned Kind = Fixup.getKind();
  switch (Kind) {
  default:
    llvm_unreachable("Unknown fixup kind!");
  case Leg::fixup_leg_movt_hi16_pcrel:
    Value >>= 16;
    // Fallthrough
  case Leg::fixup_leg_movw_lo16_pcrel:
    unsigned Hi4  = (Value & 0xF000) >> 12;
    unsigned Lo12 = Value & 0x0FFF;
    // inst{19-16} = Hi4;
    // inst{11-0} = Lo12;
    Value = (Hi4 << 16) | (Lo12);
    return Value;
  }
  return Value;
}

void LegAsmBackend::processFixupValue(const MCAssembler &Asm,
                                      const MCAsmLayout &Layout,
                                      const MCFixup &Fixup,
                                      const MCFragment *DF,
                                      MCValue &Target, uint64_t &Value,
                                      bool &IsResolved) {
  (void)adjustFixupValue(Fixup, Value, &Asm.getContext());
}

void LegAsmBackend::applyFixup(const MCFixup &Fixup, char *Data,
                               unsigned DataSize, uint64_t Value) const {
  unsigned NumBytes = 4;
  Value = adjustFixupValue(Fixup, Value);
  if (!Value) return;           // Doesn't change encoding.

  unsigned Offset = Fixup.getOffset();
  assert(Offset + NumBytes <= DataSize && "Invalid fixup offset!");

  // For each byte of the fragment that the fixup touches, mask in the bits from
  // the fixup value. The Value has been "split up" into the appropriate
  // bitfields above.
  for (unsigned i = 0; i != NumBytes; ++i)
    Data[Offset + i] |= uint8_t((Value >> (i * 8)) & 0xff);
}

namespace {

class ELFLegAsmBackend : public LegAsmBackend {
public:
  uint8_t OSABI;
  ELFLegAsmBackend(const Target &T, const StringRef TT, uint8_t _OSABI)
      : LegAsmBackend(T, TT), OSABI(_OSABI) {}

  MCObjectWriter *createObjectWriter(raw_ostream &OS) const override {
    return createLegELFObjectWriter(OS, OSABI);
  }
};

} // end anonymous namespace

MCAsmBackend *llvm::createLegAsmBackend(const Target &T,
                                        const MCRegisterInfo &MRI,
                                        StringRef TT, StringRef CPU) {
  Triple TheTriple(TT);
  uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(Triple(TT).getOS());
  return new ELFLegAsmBackend(T, TT, OSABI);
}
