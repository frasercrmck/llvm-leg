//===-- LegSelectionDAGInfo.cpp - Leg SelectionDAG Info ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the LegSelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#include "LegTargetMachine.h"
using namespace llvm;

LegSelectionDAGInfo::LegSelectionDAGInfo(const LegTargetMachine &TM)
    : TargetSelectionDAGInfo(TM) {}

LegSelectionDAGInfo::~LegSelectionDAGInfo() {}
