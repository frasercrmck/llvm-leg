//===-- LEGAsmBackend.cpp - LEG Assembler Backend -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/LEGMCTargetDesc.h"
#include "MCTargetDesc/LEGFixupKinds.h"
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
class LEGELFObjectWriter : public MCELFObjectTargetWriter {
public:
  LEGELFObjectWriter(uint8_t OSABI)
      : MCELFObjectTargetWriter(/*Is64Bit*/ false, OSABI, /*ELF::EM_LEG*/ ELF::EM_ARM,
                                /*HasRelocationAddend*/ false) {}
};

class LEGAsmBackend : public MCAsmBackend {
public:
  LEGAsmBackend(const Target &T, const StringRef TT) : MCAsmBackend() {}

  ~LEGAsmBackend() {}

  unsigned getNumFixupKinds() const override {
    return LEG::NumTargetFixupKinds;
  }

  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override {
    const static MCFixupKindInfo Infos[LEG::NumTargetFixupKinds] = {
      // This table *must* be in the order that the fixup_* kinds are defined in
      // LEGFixupKinds.h.
      //
      // Name                      Offset (bits) Size (bits)     Flags
      { "fixup_leg_movt_hi16_pcrel", 0, 32, MCFixupKindInfo::FKF_IsPCRel },
      { "fixup_leg_movw_lo16_pcrel", 0, 32, MCFixupKindInfo::FKF_IsPCRel },
    };

    if (Kind < FirstTargetFixupKind) {
      return MCAsmBackend::getFixupKindInfo(Kind);
    }

    assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
           "Invalid kind!");
    return Infos[Kind - FirstTargetFixupKind];
  }

  /// processFixupValue - Target hook to process the literal value of a fixup
  /// if necessary.
  void processFixupValue(const MCAssembler &Asm, const MCAsmLayout &Layout,
                         const MCFixup &Fixup, const MCFragment *DF,
                         const MCValue &Target, uint64_t &Value,
                         bool &IsResolved) override;

  void applyFixup(const MCFixup &Fixup, char *Data, unsigned DataSize,
                  uint64_t Value, bool IsPCRel) const override;

  bool mayNeedRelaxation(const MCInst &Inst) const override { return false; }

  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override {
    return false;
  }

  void relaxInstruction(const MCInst &Inst, MCInst &Res) const override {}

  bool writeNopData(uint64_t Count, MCObjectWriter *OW) const override {
    if (Count == 0) {
      return true;
    }
    return false;
  }

  unsigned getPointerSize() const { return 4; }
};
} // end anonymous namespace

static unsigned adjustFixupValue(const MCFixup &Fixup, uint64_t Value,
                                 MCContext *Ctx = NULL) {
  unsigned Kind = Fixup.getKind();
  switch (Kind) {
  default:
    llvm_unreachable("Unknown fixup kind!");
  case LEG::fixup_leg_movt_hi16_pcrel:
    Value >>= 16;
  // Intentional fall-through
  case LEG::fixup_leg_movw_lo16_pcrel:
    unsigned Hi4  = (Value & 0xF000) >> 12;
    unsigned Lo12 = Value & 0x0FFF;
    // inst{19-16} = Hi4;
    // inst{11-0} = Lo12;
    Value = (Hi4 << 16) | (Lo12);
    return Value;
  }
  return Value;
}

void LEGAsmBackend::processFixupValue(const MCAssembler &Asm,
                                      const MCAsmLayout &Layout,
                                      const MCFixup &Fixup,
                                      const MCFragment *DF,
                                      const MCValue &Target, uint64_t &Value,
                                      bool &IsResolved) {
  // We always have resolved fixups for now.
  IsResolved = true;
  // At this point we'll ignore the value returned by adjustFixupValue as
  // we are only checking if the fixup can be applied correctly.
  (void)adjustFixupValue(Fixup, Value, &Asm.getContext());
}

void LEGAsmBackend::applyFixup(const MCFixup &Fixup, char *Data,
                               unsigned DataSize, uint64_t Value,
                               bool isPCRel) const {
  unsigned NumBytes = 4;
  Value = adjustFixupValue(Fixup, Value);
  if (!Value) {
    return; // Doesn't change encoding.
  }

  unsigned Offset = Fixup.getOffset();
  assert(Offset + NumBytes <= DataSize && "Invalid fixup offset!");

  // For each byte of the fragment that the fixup touches, mask in the bits from
  // the fixup value. The Value has been "split up" into the appropriate
  // bitfields above.
  for (unsigned i = 0; i != NumBytes; ++i) {
    Data[Offset + i] |= uint8_t((Value >> (i * 8)) & 0xff);
  }
}

namespace {

class ELFLEGAsmBackend : public LEGAsmBackend {
public:
  uint8_t OSABI;
  ELFLEGAsmBackend(const Target &T, const StringRef TT, uint8_t _OSABI)
      : LEGAsmBackend(T, TT), OSABI(_OSABI) {}

  MCObjectWriter *createObjectWriter(raw_ostream &OS) const override {
    return createLEGELFObjectWriter(OS, OSABI);
  }
};

} // end anonymous namespace

MCAsmBackend *llvm::createLEGAsmBackend(const Target &T,
                                        const MCRegisterInfo &MRI,
                                        StringRef TT, StringRef CPU) {
  Triple TheTriple(TT);
  const uint8_t ABI = MCELFObjectTargetWriter::getOSABI(Triple(TT).getOS());
  return new ELFLEGAsmBackend(T, TT, ABI);
}
