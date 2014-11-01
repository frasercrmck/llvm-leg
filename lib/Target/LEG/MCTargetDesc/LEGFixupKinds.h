//===-- LEGFixupKinds.h - LEG-Specific Fixup Entries ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LEGFIXUPKINDS_H
#define LLVM_LEGFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace LEG {
enum Fixups {
  fixup_leg_mov_hi16_pcrel = FirstTargetFixupKind,
  fixup_leg_mov_lo16_pcrel,

  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
}
}

#endif

