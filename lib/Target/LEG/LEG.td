//===-- LEG.td - Describe the LEG Target Machine -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This is the top level entry point for the LEG target.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Descriptions
//===----------------------------------------------------------------------===//

include "LEGRegisterInfo.td"
include "LEGInstrInfo.td"
include "LEGCallingConv.td"

def LEGInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// LEG processors supported.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features>
    : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def LEG : Target {
  let InstructionSet = LEGInstrInfo;
}
