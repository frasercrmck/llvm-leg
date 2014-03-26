//===-- LegOperators.td - Leg-specific operators ------*- tblgen-*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Type profiles
//===----------------------------------------------------------------------===//

def SDT_LegCallSeqStart : SDCallSeqStart<[ SDTCisVT<0, i32> ]>;
def SDT_LegCallSeqEnd   : SDCallSeqEnd<[ SDTCisVT<0, i32>,
                                         SDTCisVT<1, i32> ]>;

def LegRetFlag    : SDNode<"LegISD::RET_FLAG", SDTNone,
                           [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_LegCallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end   : SDNode<"ISD::CALLSEQ_END",   SDT_LegCallSeqEnd,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

//===----------------------------------------------------------------------===//
// Operand Definitions.
//===----------------------------------------------------------------------===//

def memsrc : Operand<i32> {
  let MIOperandInfo = (ops GRRegs, i32imm);
  let PrintMethod = "printAddrModeMemSrc";
}

def i32imm_lo : Operand<i32>, ImmLeaf<i32, [{
  return Imm >= 0 && Imm < 65536;
}]>;

//===----------------------------------------------------------------------===//
// Complex Pattern Definitions.
//===----------------------------------------------------------------------===//

def addr : ComplexPattern<iPTR, 2, "SelectAddr", [], []>;
