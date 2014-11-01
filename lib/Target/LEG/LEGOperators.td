//===-- LEGOperators.td - LEG-specific operators ------*- tblgen-*-===//
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

def MoveImm32Ty : SDTypeProfile<1, 1, [
  SDTCisSameAs<0, 1>, SDTCisInt<0>
]>;

def SDT_LEGCallSeqStart : SDCallSeqStart<[ SDTCisVT<0, i32> ]>;

def SDT_LEGCallSeqEnd   : SDCallSeqEnd<[
  SDTCisVT<0, i32>, SDTCisVT<1, i32>
]>;

def LEGRetFlag    : SDNode<"LEGISD::RET_FLAG", SDTNone,
                           [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_LEGCallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end   : SDNode<"ISD::CALLSEQ_END",   SDT_LEGCallSeqEnd,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

def SDT_LEGCall    : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;

//===----------------------------------------------------------------------===//
// Custom SDNodes.
//===----------------------------------------------------------------------===//

def load_sym : SDNode<"LEGISD::LOAD_SYM", SDTIntUnaryOp>;

def movei32 : SDNode<"LEGISD::MOVEi32", MoveImm32Ty>;

def leg_call
    : SDNode<"LEGISD::CALL", SDT_LEGCall,
             [ SDNPHasChain, SDNPOptInGlue, SDNPOutGlue, SDNPVariadic ]>;

//===----------------------------------------------------------------------===//
// Operand Definitions.
//===----------------------------------------------------------------------===//

def bl_target : Operand<i32>;

def memsrc : Operand<i32> {
  let MIOperandInfo = (ops GRRegs, i32imm);
  let PrintMethod = "printAddrModeMemSrc";
  let EncoderMethod = "getMemSrcValue";
}

def i32imm_lo : Operand<i32>, ImmLeaf<i32, [{
  return Imm >= 0 && Imm < 65536;
}]>;

def LEGimm8 : Operand<i32>, ImmLeaf<i32, [{
  return Imm >= 0 && Imm < 256;
}]>;

//===----------------------------------------------------------------------===//
// Complex Pattern Definitions.
//===----------------------------------------------------------------------===//

def addr : ComplexPattern<iPTR, 2, "SelectAddr", [], []>;

