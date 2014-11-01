//===-- LEGMCAsmInfo.h - LEG asm properties --------------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the LEGMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LEGTARGETASMINFO_H
#define LEGTARGETASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class StringRef;
class Target;

class LEGMCAsmInfo : public MCAsmInfoELF {
  virtual void anchor();

public:
  explicit LEGMCAsmInfo(StringRef TT);
};

} // namespace llvm

#endif
