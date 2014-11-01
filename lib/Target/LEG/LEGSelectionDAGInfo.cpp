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

#include "LEGSelectionDAGInfo.h"
using namespace llvm;

#define DEBUG_TYPE "leg-selectiondag-info"

LEGSelectionDAGInfo::LEGSelectionDAGInfo(const TargetMachine &TM)
    : TargetSelectionDAGInfo(TM) {}

LEGSelectionDAGInfo::~LEGSelectionDAGInfo() {}
