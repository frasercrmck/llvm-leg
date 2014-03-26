//===-- LegMCAsmInfo.h - Leg asm properties --------------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the LegMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LEGTARGETASMINFO_H
#define LEGTARGETASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class StringRef;
class Target;

class LegMCAsmInfo : public MCAsmInfoELF {
  virtual void anchor();

public:
  explicit LegMCAsmInfo(StringRef TT);
};

} // namespace llvm

#endif
