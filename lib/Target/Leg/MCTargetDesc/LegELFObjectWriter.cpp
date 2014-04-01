//===-- LegELFObjectWriter.cpp - Leg ELF Writer ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/LegMCTargetDesc.h"
#include "MCTargetDesc/LegFixupKinds.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  class LegELFObjectWriter : public MCELFObjectTargetWriter {
  public:
    LegELFObjectWriter(uint8_t OSABI);

    virtual ~LegELFObjectWriter();

    unsigned GetRelocType(const MCValue &Target, const MCFixup &Fixup,
                          bool IsPCRel, bool IsRelocWithSymbol,
                          int64_t Addend) const override;
  };
}

unsigned LegELFObjectWriter::GetRelocType(const MCValue &Target,
                                          const MCFixup &Fixup, bool IsPCRel,
                                          bool IsRelocWithSymbol,
                                          int64_t Addend) const {
  if (!IsPCRel)
    llvm_unreachable("Only dealying with PC-relative fixups for now");

  unsigned Type = 0;
  switch ((unsigned)Fixup.getKind()) {
  default:
    llvm_unreachable("Unimplemented");
  case Leg::fixup_leg_movt_hi16_pcrel:
    Type = ELF::R_ARM_MOVT_PREL;
    break;
  case Leg::fixup_leg_movw_lo16_pcrel:
    Type = ELF::R_ARM_MOVW_PREL_NC;
    break;
  }
  return Type;
}

LegELFObjectWriter::LegELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit*/ false, OSABI, ELF::EM_LEG,
                              /*HasRelocationAddend*/ false) {}

LegELFObjectWriter::~LegELFObjectWriter() {}

MCObjectWriter *llvm::createLegELFObjectWriter(raw_ostream &OS, uint8_t OSABI) {
  MCELFObjectTargetWriter *MOTW = new LegELFObjectWriter(OSABI);
  return createELFObjectWriter(MOTW, OS, /*IsLittleEndian=*/true);
}
