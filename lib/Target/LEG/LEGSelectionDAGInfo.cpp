//===-- LEGSelectionDAGInfo.cpp - LEG SelectionDAG Info ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the LEGSelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#include "LEGTargetMachine.h"
using namespace llvm;

LEGSelectionDAGInfo::LEGSelectionDAGInfo(const LEGTargetMachine &TM)
    : TargetSelectionDAGInfo(TM) {}

LEGSelectionDAGInfo::~LEGSelectionDAGInfo() {}
