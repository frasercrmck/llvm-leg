======================
LLVM 3.9 Release Notes
======================

.. contents::
    :local:

.. warning::
   These are in-progress notes for the upcoming LLVM 3.9 release.  You may
   prefer the `LLVM 3.8 Release Notes <http://llvm.org/releases/3.8.0/docs
   /ReleaseNotes.html>`_.


Introduction
============

This document contains the release notes for the LLVM Compiler Infrastructure,
release 3.9.  Here we describe the status of LLVM, including major improvements
from the previous release, improvements in various subprojects of LLVM, and
some of the current users of the code.  All LLVM releases may be downloaded
from the `LLVM releases web site <http://llvm.org/releases/>`_.

For more information about LLVM, including information about the latest
release, please check out the `main LLVM web site <http://llvm.org/>`_.  If you
have questions or comments, the `LLVM Developer's Mailing List
<http://lists.llvm.org/mailman/listinfo/llvm-dev>`_ is a good place to send
them.

Note that if you are reading this file from a Subversion checkout or the main
LLVM web page, this document applies to the *next* release, not the current
one.  To see the release notes for a specific release, please see the `releases
page <http://llvm.org/releases/>`_.

Non-comprehensive list of changes in this release
=================================================
* The LLVMContext gains a new runtime check (see
  LLVMContext::discardValueNames()) that can be set to discard Value names
  (other than GlobalValue). This is intended to be used in release builds by
  clients that are interested in saving CPU/memory as much as possible.

* There is no longer a "global context" available in LLVM, except for the C API.

* .. note about autoconf build having been removed.

* .. note about C API functions LLVMParseBitcode,
   LLVMParseBitcodeInContext, LLVMGetBitcodeModuleInContext and
   LLVMGetBitcodeModule having been removed. LLVMGetTargetMachineData has been
   removed (use LLVMGetDataLayout instead).

* The C API function LLVMLinkModules has been removed.

* The C API function LLVMAddTargetData has been removed.

* The C API function LLVMGetDataLayout is deprecated
  in favor of LLVMGetDataLayoutStr.

* The C API enum LLVMAttribute and associated API is deprecated in favor of
  the new LLVMAttributeRef API. The deprecated functions are
  LLVMAddFunctionAttr, LLVMAddTargetDependentFunctionAttr,
  LLVMRemoveFunctionAttr, LLVMGetFunctionAttr, LLVMAddAttribute,
  LLVMRemoveAttribute, LLVMGetAttribute, LLVMAddInstrAttribute,
  LLVMRemoveInstrAttribute and LLVMSetInstrParamAlignment.

* ``TargetFrameLowering::eliminateCallFramePseudoInstr`` now returns an
  iterator to the next instruction instead of ``void``. Targets that previously
  did ``MBB.erase(I); return;`` now probably want ``return MBB.erase(I);``.

* ``SelectionDAGISel::Select`` now returns ``void``. Out of tree targets will
  need to be updated to replace the argument node and remove any dead nodes in
  cases where they currently return an ``SDNode *`` from this interface.

* Introduction of ThinLTO: [FIXME: needs to be documented more extensively in
  /docs/ ; ping Mehdi/Teresa before the release if not done]

* Raised the minimum required CMake version to 3.4.3.

.. NOTE
   For small 1-3 sentence descriptions, just add an entry at the end of
   this list. If your description won't fit comfortably in one bullet
   point (e.g. maybe you would like to give an example of the
   functionality, or simply have a lot to talk about), see the `NOTE` below
   for adding a new subsection.

* ... next change ...

.. NOTE
   If you would like to document a larger change, then you can add a
   subsection about it right here. You can copy the following boilerplate
   and un-indent it (the indentation causes it to be inside this comment).

   Special New Feature
   -------------------

   Makes programs 10x faster by doing Special New Thing.

GCC ABI Tag
-----------

Recently, many of the Linux distributions (ex. `Fedora <http://developerblog.redhat.com/2015/02/10/gcc-5-in-fedora/>`_,
`Debian <https://wiki.debian.org/GCC5>`_, `Ubuntu <https://wiki.ubuntu.com/GCC5>`_)
have moved on to use the new `GCC ABI <https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Attributes.html>`_
to work around `C++11 incompatibilities in libstdc++ <https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html>`_.
This caused `incompatibility problems <https://gcc.gnu.org/ml/gcc-patches/2015-04/msg00153.html>`_
with other compilers (ex. Clang), which needed to be fixed, but due to the
experimental nature of GCC's own implementation, it took a long time for it to
land in LLVM (`here <https://reviews.llvm.org/D18035>`_ and
`here <https://reviews.llvm.org/D17567>`_), not in time for the 3.8 release.

Those patches are now present in the 3.9.0 release and should be working on the
majority of cases, as they have been tested thoroughly. However, some bugs were
`filled in GCC <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=71712>`_ and have not
yet been fixed, so there may be corner cases not covered by either GCC or Clang.
Bug fixes to those problems should be reported in Bugzilla (either LLVM or GCC),
and patches to LLVM's trunk are very likely to be back-ported to future 3.9.x
releases (depends on how destructive it is).

Unfortunately, these patches won't be back-ported to 3.8.x or earlier, so we
strongly recommend people to use 3.9.x when GCC ABI cases are at stake.

For a more in-depth view of the issue, check our `Bugzilla entry <https://llvm.org/bugs/show_bug.cgi?id=23529>`_.

Changes to the LLVM IR
----------------------

* New intrinsics ``llvm.masked.load``, ``llvm.masked.store``,
  ``llvm.masked.gather`` and ``llvm.masked.scatter`` were introduced to the
  LLVM IR to allow selective memory access for vector data types.

Changes to LLVM's IPO model
---------------------------

LLVM no longer does inter-procedural analysis and optimization (except
inlining) on functions with comdat linkage.  Doing IPO over such
functions is unsound because the implementation the linker chooses at
link-time may be differently optimized than the one what was visible
during optimization, and may have arbitrarily different observable
behavior.  See `PR26774 <http://llvm.org/PR26774>`_ for more details.

Changes to the ARM Targets
--------------------------

**During this release the AArch64 backend has:**

* Gained support for Qualcomm's Kryo and Broadcom's Vulcan CPUs, including
  scheduling models
* Landed a scheduling model for Samsung's Exynos M1
* Seen a lot of work on GlobalISel
* Learned a few more useful combines (fadd and fmul into fmadd, adjustments to the
  stack pointer for callee-save stack memory and local stack memory etc)
* Gained support for the Swift calling convention
* Switched to using SubtargetFeatures rather than testing for specific CPUs and
  to using TableGen for handling system instruction operands
* Like ARM, AArch64 is now using the TargetParser, so no more StringSwitches
  matching CPU, FPU or feature names will be accepted in normal code.
* Gained a big batch of tests from Halide

 Furthermore, LLDB now supports AArch64 compact unwind tables, as used on iOS,
 tvos and watchos.

**During this release the ARM target has:**

* ARMv8.2-A can now be targeted directly via Clang flags.
* Adding preliminary support for Cortex-R8
* LLDB can now parse EABI attributes for an ELF input.
* Initial ARM/Thumb support was added to LLD.
* The ExecutionEngine now supports COFF/ARM
* Swift calling convention was ported to ARM
* A large number of codegen fixes around ARMv8, DSP, correct sub-target support,
  relocations, EABI, EHABI, Windows on ARM, atomics.
* Improved assembler support for Linux/Android/Chromium sub-projects.
* Initial support for MUSL (libc) on ARM
* Support for Thumb1 targets in libunwind
* Gained a big batch of tests from Halide


Changes to the MIPS Target
--------------------------

 During this release ...


Changes to the PowerPC Target
-----------------------------

 Moved some optimizations from O3 to O2 (D18562)

* Enable sibling call optimization on ppc64 ELFv1/ELFv2 abi

Changes to the X86 Target
-------------------------

* LLVM now supports the Intel CPU codenamed Skylake Server with AVX-512
  extensions using ``-march=skylake-avx512``. The switch enables the
  ISA extensions AVX-512{F, CD, VL, BW, DQ}.

* LLVM now supports the Intel CPU codenamed Knights Landing with AVX-512
  extensions using ``-march=knl``. The switch enables the ISA extensions
  AVX-512{F, CD, ER, PF}.

Changes to the AMDGPU Target
-----------------------------

 * Mesa 11.0.x is no longer supported


Changes to the OCaml bindings
-----------------------------

 During this release ...

Support for attribute 'notail' has been added
---------------------------------------------

This marker prevents optimization passes from adding 'tail' or
'musttail' markers to a call. It is used to prevent tail call
optimization from being performed on the call.

External Open Source Projects Using LLVM 3.9
============================================

An exciting aspect of LLVM is that it is used as an enabling technology for
a lot of other language and tools projects. This section lists some of the
projects that have already been updated to work with LLVM 3.9.

* A project


Additional Information
======================

A wide variety of additional information is available on the `LLVM web page
<http://llvm.org/>`_, in particular in the `documentation
<http://llvm.org/docs/>`_ section.  The web page also contains versions of the
API documentation which is up-to-date with the Subversion version of the source
code.  You can access versions of these documents specific to this release by
going into the ``llvm/docs/`` directory in the LLVM tree.

If you have any questions or comments about LLVM, please feel free to contact
us via the `mailing lists <http://llvm.org/docs/#maillist>`_.

