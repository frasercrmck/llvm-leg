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

class LegReg<string n> : Register<n> {
  field bits<4> Num;
  let Namespace = "Leg";
}

class R<bits<4> num, string n> : LegReg<n> {
  let Num = num;
}

// CPU registers
def R0  : R< 0, "r0">;
def R1  : R< 1, "r1">;
def R2  : R< 2, "r2">; 
def R3  : R< 3, "r3">;
def R4  : R< 4, "r4">;
def R5  : R< 5, "r5">; 
def R6  : R< 6, "r6">;
def R7  : R< 7, "r7">;
def R8  : R< 8, "r8">;
def R9  : R< 9, "r9">; 

def SP  : R<10, "sp">;
def LR  : R<11, "lr">;

// Register classes.
//
def GRRegs : RegisterClass<"Leg", [i32], 32,
  // Return values and arguments
  (add R0, R1, R2, R3,
       R4, R5, R6, R7, R8, R9)>;
