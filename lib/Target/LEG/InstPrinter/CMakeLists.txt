add_llvm_library(LLVMLEGAsmPrinter
  LEGInstPrinter.cpp
  )

add_dependencies(LLVMLEGAsmPrinter LEGCommonTableGen)
