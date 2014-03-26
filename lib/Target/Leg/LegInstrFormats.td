//===-- LegInstrFormats.td - Leg Instruction Formats ---*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//
class InstLeg<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;

  let Namespace = "Leg";
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString   = asmstr;
  let Pattern = pattern;
  let Size = 32;
}

// Leg pseudo instructions format
class LegPseudoInst<dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstLeg<outs, ins, asmstr, pattern> {
  let isPseudo = 1;
}
