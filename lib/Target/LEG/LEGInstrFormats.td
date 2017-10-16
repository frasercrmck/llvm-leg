//===-- LEGInstrFormats.td - LEG Instruction Formats -*------- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Describe LEG instructions format here
//

// Format specifies the encoding used by the instruction.  This is part of the
// ad-hoc solution used to emit machine instruction encodings by our machine
// code emitter.
class InstLEG<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;

  let Namespace = "LEG";
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString   = asmstr;
  let Pattern = pattern;
  let Size = 4;

  // Always execute - no condition flags for this example
  let Inst{31-28} = 0b1110;
}

// LEG pseudo instructions format
class LEGPseudoInst<dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstLEG<outs, ins, asmstr, pattern> {
  let isPseudo = 1;
}

class ALUInst<bits<4> opcode, dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstLEG<outs, ins, asmstr, pattern> {
  let Inst{27-26} = 0b00;
  let Inst{24-21} = opcode;
}

class MulInst<bits<7> opcode, dag outs, dag ins, string asmstr,
              list<dag> pattern> : InstLEG<outs, ins, asmstr, pattern> {
  let Inst{27-21} = opcode;
  let Inst{20}    = 0;  // We never update flags
  let Inst{7-4}   = 0b1001;
}

class BranchInst<bits<4> opcode, dag outs, dag ins,
                 string asmstr, list<dag> pattern>
    : InstLEG<outs, ins, asmstr, pattern> {
  let Inst{27-24} = opcode;
}
