//===-- LegInstrInfo.td - Target Description for Leg ---*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the Leg instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "LegInstrFormats.td"
include "LegOperators.td"

def ADD    : InstLeg<(outs GRRegs:$dst),
                     (ins GRRegs:$src1, GRRegs:$src2),
                     "add $dst, $src1, $src2",
                     [(set i32:$dst, (add i32:$src1, i32:$src2))]>;

def ADD_ri : InstLeg<(outs GRRegs:$dst),
                     (ins GRRegs:$src1, i32imm:$src2),
                     "add $dst, $src1, #$src2",
                     [(set i32:$dst, (add i32:$src1, imm:$src2))]>;

def RET  : InstLeg<(outs), (ins), "RET",  [(LegRetFlag)]>;

def LOAD : InstLeg<(outs GRRegs:$dst), (ins memsrc:$src),
                   "ldr $dst, [$src]",
                   [(set i32:$dst, (load addr:$src))]>;

def STORE : InstLeg<(outs), (ins GRRegs:$src, memsrc:$dst),
                   "str $src, [$dst]",
                   [(store i32:$src, addr:$dst)]>;

//===----------------------------------------------------------------------===//
// Pseudo Instructions
//===----------------------------------------------------------------------===//

let Defs = [SP], Uses = [SP] in {
def ADJCALLSTACKDOWN : LegPseudoInst<(outs), (ins i32imm:$amt),
                                    "# ADJCALLSTACKDOWN $amt",
                                    [(callseq_start timm:$amt)]>;
def ADJCALLSTACKUP   : LegPseudoInst<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                    "# ADJCALLSTACKUP $amt1",
                                    [(callseq_end timm:$amt1, timm:$amt2)]>;
}
