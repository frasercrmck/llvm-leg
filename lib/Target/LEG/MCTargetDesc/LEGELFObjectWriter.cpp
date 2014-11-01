//===-- LEGELFObjectWriter.cpp - LEG ELF Writer ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/LEGMCTargetDesc.h"
#include "MCTargetDesc/LEGFixupKinds.h"
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
  class LEGELFObjectWriter : public MCELFObjectTargetWriter {
  public:
    LEGELFObjectWriter(uint8_t OSABI);

    virtual ~LEGELFObjectWriter();

    unsigned GetRelocType(const MCValue &Target, const MCFixup &Fixup,
                          bool IsPCRel, bool IsRelocWithSymbol,
                          int64_t Addend) const override;
  };
}

unsigned LEGELFObjectWriter::GetRelocType(const MCValue &Target,
                                          const MCFixup &Fixup,
                                          bool IsPCRel, bool IsRelocWithSymbol,
                                          int64_t Addend) const {
  if (!IsPCRel) {
    llvm_unreachable("Only dealying with PC-relative fixups for now");
  }

  unsigned Type = 0;
  switch ((unsigned)Fixup.getKind()) {
  default:
    llvm_unreachable("Unimplemented");
  case LEG::fixup_leg_mov_hi16_pcrel:
    Type = ELF::R_ARM_MOVT_PREL;
    break;
  case LEG::fixup_leg_mov_lo16_pcrel:
    Type = ELF::R_ARM_MOVW_PREL_NC;
    break;
  }
  return Type;
}

LEGELFObjectWriter::LEGELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit*/ false, OSABI, /*ELF::EM_LEG*/ ELF::EM_ARM,
                              /*HasRelocationAddend*/ false) {}

LEGELFObjectWriter::~LEGELFObjectWriter() {}

MCObjectWriter *llvm::createLEGELFObjectWriter(raw_ostream &OS, uint8_t OSABI) {
  MCELFObjectTargetWriter *MOTW = new LEGELFObjectWriter(OSABI);
  return createELFObjectWriter(MOTW, OS, /*IsLittleEndian=*/true);
}
