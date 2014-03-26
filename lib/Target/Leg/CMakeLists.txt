set(LLVM_TARGET_DEFINITIONS Leg.td)

tablegen(LLVM LegGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM LegGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM LegGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM LegGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM LegGenCallingConv.inc -gen-callingconv)
tablegen(LLVM LegGenSubtargetInfo.inc -gen-subtarget)
add_public_tablegen_target(LegCommonTableGen)

add_llvm_target(LegCodeGen
  LegSubtarget.cpp
  LegTargetMachine.cpp
  LegRegisterInfo.cpp
  LegInstrInfo.cpp
  LegFrameLowering.cpp
  LegISelLowering.cpp
  LegSelectionDAGInfo.cpp
  LegISelDAGToDAG.cpp
  LegAsmPrinter.cpp
  LegMCInstLower.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
