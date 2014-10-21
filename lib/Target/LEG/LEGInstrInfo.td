//===-- LEGInstrInfo.td - Target Description for LEG ---*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the LEG instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "LEGInstrFormats.td"
include "LEGOperators.td"

//===----------------------------------------------------------------------===//
// ALU Instructions
//===----------------------------------------------------------------------===//

multiclass BinOp<bits<4> opcode, string opstr, SDNode opnode> {
  def rr : ALUInst<opcode, (outs GRRegs:$dst),
                   (ins GRRegs:$src1, GRRegs:$src2),
                   !strconcat(opstr, " $dst, $src1, $src2"),
                   [(set i32:$dst, (opnode i32:$src1, i32:$src2))]> {
    bits<4> dst;
    bits<4> src1;
    bits<4> src2;
    let Inst{25} = 0;
    let Inst{20} = 0;  // We never update flags
    let Inst{19-16} = src1;
    let Inst{15-12} = dst;
    let Inst{11-4}  = 0b00000000;
    let Inst{3-0}   = src2;
  }
  def ri : ALUInst<opcode, (outs GRRegs:$dst),
                   (ins GRRegs:$src1, i32imm:$imm),
                   !strconcat(opstr, " $dst, $src1, $imm"),
                   [/*(set i32:$dst, (opnode i32:$src1, LEGimm8:$imm))*/]> {
    bits<4> dst;
    bits<4> src1;
    bits<12> imm;
    let Inst{25} = 1;
    let Inst{20} = 0;  // We never update flags
    let Inst{19-16} = dst;
    let Inst{15-12} = src1;
    let Inst{11-0}  = imm;
  }
}

defm ADD  : BinOp<0b0100, "add", add>;
defm SUB  : BinOp<0b0010, "sub", sub>;

//===----------------------------------------------------------------------===//
// Multiply Instructions
//===----------------------------------------------------------------------===//

def MUL  : MulInst<0b0000000, (outs GRRegs:$dst),
                   (ins GRRegs:$src1, GRRegs:$src2),
                   "mul $dst, $src1, $src2",
                   [(set i32:$dst, (mul i32:$src1, i32:$src2))]> {
  bits<4> dst;
  bits<4> src1;
  bits<4> src2;
  let Inst{19-16} = dst;
  let Inst{15-12} = 0b0000;
  let Inst{11-8}  = src2;
  let Inst{3-0}   = src1;
}

def MLA  : MulInst<0b0000001, (outs GRRegs:$dst),
                   (ins GRRegs:$src1, GRRegs:$src2, GRRegs:$src3),
                   "mla $dst, $src1, $src2, $src3",
                   [(set i32:$dst, (add (mul i32:$src1, i32:$src2),
                                         i32:$src3))]> {
  bits<4> dst;
  bits<4> src1;
  bits<4> src2;
  bits<4> src3;
  let Inst{19-16} = dst;
  let Inst{15-12} = src3;
  let Inst{11-8}  = src2;
  let Inst{3-0}   = src1;
}

//===----------------------------------------------------------------------===//
// Mov Instructions
//===----------------------------------------------------------------------===//
def MOVrr  : ALUInst<0b1101, (outs GRRegs:$dst), (ins GRRegs:$src),
                     "mov $dst, $src",
                     [/* No pattern */]> {
  bits<4> dst;
  bits<4> src;
  let Inst{25-27} = 0b000;  
  let Inst{20} = 0;
  let Inst{19-16} = 0b0000;
  let Inst{15-12} = dst;
  let Inst{11-4}  = 0b00000000;
  let Inst{3-0} = src;
}

//===----------------------------------------------------------------------===//
// Mov Immediate Instructions
//===----------------------------------------------------------------------===//

class MOV<bits<4> opcode, string opstr, dag ins, list<dag>pattern>
    : ALUInst<opcode, (outs GRRegs:$dst), ins,
              !strconcat(opstr, " $dst, $imm"), pattern> {
  bits<4> dst;
  bits<16> imm;
  let Inst{15-12} = dst;
  let Inst{11-0}  = imm{11-0};
  let Inst{19-16} = imm{15-12};
  let Inst{20} = 0;
  let Inst{25} = 1;
}

def MOVWi16 : MOV<0b1000, "movw", (ins i32imm:$imm),
                  [(set i32:$dst, i32imm_lo:$imm)]>;
def MOVTi16 : MOV<0b1010, "movt", (ins GRRegs:$src1, i32imm:$imm),
                  [/* No Pattern */]>;

def MOVi32 : InstLEG<(outs GRRegs:$dst), (ins i32imm:$src), "",
                     [(set i32:$dst, (movei32 imm:$src))]> {
  let isPseudo = 1;
}


//===----------------------------------------------------------------------===//
// Load/Store Instructions
//===----------------------------------------------------------------------===//

def LDR  : InstLEG<(outs GRRegs:$val), (ins memsrc:$addr),
                   "ldr $val, $addr",
                   [(set i32:$val, (load addr:$addr))]> {
  bits<4> val;
  bits<16> addr;
  let Inst{27-25} = 0b010;
  let Inst{24} = 1; // not indexed
  let Inst{23} = 1; // add ofset
  let Inst{22} = 0;
  let Inst{21} = 0; // no writeback
  let Inst{20} = 1; // load
  let Inst{19-16} = addr{15-12};
  let Inst{15-12} = val;
  let Inst{11-0} = addr{11-0};
}

def STR : InstLEG<(outs), (ins GRRegs:$val, memsrc:$addr),
                  "str $val, $addr",
                  [(store i32:$val, addr:$addr)]> {
  bits<16> addr;
  bits<4> val;
  let Inst{27-25} = 0b010;
  let Inst{24} = 1; // not indexed
  let Inst{23} = 1; // add ofset
  let Inst{22} = 0;
  let Inst{21} = 0; // no writeback
  let Inst{20} = 0; // store
  let Inst{19-16} = addr{15-12};
  let Inst{15-12} = val;
  let Inst{11-0} = addr{11-0};
}

//===----------------------------------------------------------------------===//
// Return Instructions
//===----------------------------------------------------------------------===//

let isTerminator = 1, isReturn = 1, isBarrier = 1, Uses = [LR] in {
  def RET : InstLEG<(outs), (ins variable_ops),
                    "bx lr",  [(LEGRetFlag)]> {
    let Inst{27-0}  = 0b0001001011111111111100011110;
  }
}


//===----------------------------------------------------------------------===//
// Call Instructions
//===----------------------------------------------------------------------===//

let isCall = 1, Defs = [LR], Uses = [SP] in {
  def BL : BranchInst<0b1011, (outs), (ins GRRegs:$addr),
                      "bl $addr",
                      [(leg_call i32:$addr)]> {
    bits<4> addr;
    let Inst{31-28} = 0b1110;
    let Inst{3-0} = addr;
  }
}

def : Pattern<(i32 (load_sym tglobaladdr:$addr)),  [(MOVi32 $addr)]>;

//===----------------------------------------------------------------------===//
// Pseudo Instructions
//===----------------------------------------------------------------------===//

let Defs = [SP], Uses = [SP] in {
def ADJCALLSTACKDOWN : LEGPseudoInst<(outs), (ins i32imm:$amt),
                                    "# ADJCALLSTACKDOWN $amt",
                                    [(callseq_start timm:$amt)]>;
def ADJCALLSTACKUP   : LEGPseudoInst<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                    "# ADJCALLSTACKUP $amt1",
                                    [(callseq_end timm:$amt1, timm:$amt2)]>;
}
