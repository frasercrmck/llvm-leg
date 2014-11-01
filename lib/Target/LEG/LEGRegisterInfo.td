//===-- LEGRegisterInfo.td - LEG Register defs ---------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the LEG register file 
//===----------------------------------------------------------------------===//

class LEGReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "LEG";
}

// CPU registers
def R0  : LEGReg< 0, "r0">;
def R1  : LEGReg< 1, "r1">;
def R2  : LEGReg< 2, "r2">;
def R3  : LEGReg< 3, "r3">;
def R4  : LEGReg< 4, "r4">;
def R5  : LEGReg< 5, "r5">;
def R6  : LEGReg< 6, "r6">;
def R7  : LEGReg< 7, "r7">;
def R8  : LEGReg< 8, "r8">;
def R9  : LEGReg< 9, "r9">;

def SP  : LEGReg<13, "sp">;
def LR  : LEGReg<14, "lr">;

// Register classes.
//
def GRRegs : RegisterClass<"LEG", [i32], 32,
  // Return values and arguments
  (add R0, R1, R2, R3,
       R4, R5, R6, R7, R8, R9, SP)>;
