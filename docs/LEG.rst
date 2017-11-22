===================
LEG Example Backend
===================

This is a simple example LLVM backend for an ARM-like architecture: 'LEG'.

Check out the slides from the tutorial given at Euro LLVM Developers' Conference 2014, Edinburgh:
    http://llvm.org/devmtg/2014-04/PDFs/Talks/Building%20an%20LLVM%20backend.pdf

Look out for us at the USA LLVM Developers' Conference, October 28-29, San Jose:
    http://llvm.org/devmtg/2014-10/


Fraser Cormack & Pierre-André Saulais, Codeplay Software

Getting Started
===============

The LEG repositories (llvm-leg, clang-leg) both track tip, and aren't tied to any specific revision of LLVM/Clang.

Tried & tested build systems:
*  CMake

Building with in-tree clang (optional)
--------------------------------------

    cd llvm-leg
    git clone git@github.com:codeplaysoftware/clang-leg.git tools/clang

Building with make
------------------

    cd llvm-leg
    mkdir build && cd build
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=DEBUG(/RELEASE) -DLLVM_TARGETS_TO_BUILD:STRING=LEG(,X86,...) ../
    make -j9

Building with ninja
-------------------

    cd llvm-leg
    mkdir build && cd build
    cmake -G "Ninja" -DCMAKE_BUILD_TYPE:STRING=DEBUG(/RELEASE) -DLLVM_TARGETS_TO_BUILD:STRING=LEG(,X86,...) ../
    ninja


Compiling some code
-------------------

    cd llvm-leg

    ### Compile IR -> Assembly ###
    ./build/bin/llc -march leg -o ex1.s docs/Codeplay/examples/ex1.ll
    cat ex1.s

    ### Compile IR -> ELF Object File ###
    ./build/bin/llc -march leg -filetype=obj -o ex1.o docs/Codeplay/examples/ex1.ll
    xxd ex1.o

    ### Compile Source -> Assembly with in-tree clang (via clang-leg) ###
    ./build/bin/clang -cc1 -triple leg-unknown-unknown -S -o ex1.s docs/Codeplay/example/ex1.c
    cat ex1.s

    ### Compile Source -> Assembly with in-tree clang (via clang-leg) ###
    ./build/bin/clang -cc1 -triple leg-unknown-unknown -o ex1.o docs/Codeplay/example/ex1.c
    xxd ex1.0

