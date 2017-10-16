set(LLVM_TARGET_DEFINITIONS LEG.td)

tablegen(LLVM LEGGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM LEGGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM LEGGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM LEGGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM LEGGenCallingConv.inc -gen-callingconv)
tablegen(LLVM LEGGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM LEGGenMCCodeEmitter.inc -gen-emitter)
add_public_tablegen_target(LEGCommonTableGen)

add_llvm_target(LEGCodeGen
  LEGISelDAGToDAG.cpp
  LEGISelLowering.cpp
  LEGInstrInfo.cpp
  LEGFrameLowering.cpp
  LEGMachineFunctionInfo.cpp
  LEGRegisterInfo.cpp
  LEGSubtarget.cpp
  LEGTargetMachine.cpp
  LEGAsmPrinter.cpp
  LEGMCInstLower.cpp
  LEGSelectionDAGInfo.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
