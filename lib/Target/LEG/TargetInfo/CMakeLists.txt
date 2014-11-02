add_llvm_library(LLVMLEGInfo
  LEGTargetInfo.cpp
  )

add_dependencies(LLVMLEGInfo LEGCommonTableGen)
