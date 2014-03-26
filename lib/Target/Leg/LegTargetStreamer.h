//===-- LegTargetStreamer.h - Leg Target Streamer ----------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LEGTARGETSTREAMER_H
#define LEGTARGETSTREAMER_H

#include "llvm/MC/MCStreamer.h"

namespace llvm {
class LegTargetStreamer : public MCTargetStreamer {
public:
  LegTargetStreamer(MCStreamer &S);
  virtual ~LegTargetStreamer();
};
}

#endif

