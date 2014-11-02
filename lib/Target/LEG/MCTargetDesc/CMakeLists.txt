add_llvm_library(LLVMLEGDesc
  LEGMCTargetDesc.cpp
  LEGMCAsmInfo.cpp
  LEGMCCodeEmitter.cpp
  LEGAsmBackend.cpp
  LEGELFObjectWriter.cpp
  )

add_dependencies(LLVMLEGDesc LEGCommonTableGen)
