//===-- LegRegisterInfo.td - Leg Register defs ---------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the Leg register file 
//===----------------------------------------------------------------------===//

class LegReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "Leg";
}

// CPU registers
def R0  : LegReg< 0, "r0">;
def R1  : LegReg< 1, "r1">;
def R2  : LegReg< 2, "r2">;
def R3  : LegReg< 3, "r3">;
def R4  : LegReg< 4, "r4">;
def R5  : LegReg< 5, "r5">;
def R6  : LegReg< 6, "r6">;
def R7  : LegReg< 7, "r7">;
def R8  : LegReg< 8, "r8">;
def R9  : LegReg< 9, "r9">;

def SP  : LegReg<13, "sp">;
def LR  : LegReg<14, "lr">;

// Register classes.
//
def GRRegs : RegisterClass<"Leg", [i32], 32,
  // Return values and arguments
  (add R0, R1, R2, R3,
       R4, R5, R6, R7, R8, R9)>;
