

project('libjson', 'cpp', license: 'LicenseRef-Jonathan-Wallace-BSD-2-Clause-Variant', version: '7.6.2',
    default_options: ['b_ndebug=if-release', 'b_staticpic=true', 'b_pie=true', 'warning_level=2'])

cpp = meson.get_compiler('cpp')
cpp_version = cpp.version()

machine = target_machine.system()
architecture = target_machine.cpu_family()

incdir = include_directories('_internal/Source', '.')

#TODO: Check meson buildtype to set the flags below instead of using custom options
# cxxflags_default = release
# cxxflags_shared should be applied only when building the shared library which is handled below
# cxxflags_small is the same as meson's minsize
# cxxflags_debug is debug
# the -DNDEBUG should be changed to meson's b_ndebug option and probably set to if-release. Maybe also for the minsize target.

# Defaults: per-buildtype flags are derived from Meson `buildtype` option.
# Avoid forcing -DNDEBUG or -fPIC here; Meson controls those via `b_ndebug` and `b_staticpic`.

# Keep a small set of project-specific flags per buildtype.
cpp_args = []

# Ensure older GCC compilers get gnu++11 if they don't default to C++11
if cpp.get_id().contains('gcc')
    if cpp.version().version_compare('<5.0')
        if cpp.has_argument('-std=gnu++11')
            # Apply project-local C++11 flag for old compilers only
            add_project_arguments('-std=gnu++11', language: 'cpp')
        else
            error('C++11/GNU++11 standard is required but was not able to be set!')
        endif
    endif
endif

# Build-type specific flags
buildtype = get_option('buildtype')
if buildtype == 'debug'
    if get_option('json_safe').disabled()
        # This is always turned on for debug builds.
        cpp_args += ['-DJSON_SAFE']
    endif
    cpp_args += ['-DJSON_DEBUG']
elif buildtype == 'minsize'
    if cpp.get_id().contains('gcc') or cpp.get_id().contains('clang')
        cpp_args += ['-ffast-math']
    endif
    cpp_args += ['-DJSON_LESS_MEMORY']
else
    # release, debugoptimized, or other
    if cpp.get_id().contains('gcc') or cpp.get_id().contains('clang')
        cpp_args += ['-ffast-math', '-fexpensive-optimizations']
    endif
endif

warning_flags = []
linker_flags = []  #additional linker flags to add per-compiler for hardening.

if cpp.get_id().contains('gcc') or cpp.get_id().contains('clang')
    #TODO: Add -Wcast-align=strict and fix these issues to help ensure better portability
    #NOTE: -Wsign-conversion can be useful while debugging, but there are numerous places this shows up
    #      and it is not useful, so only add it while debugging.
    #NOTE: 4/4/2024 - adding flags from https://best.openssf.org/Compiler-Hardening-Guides/Compiler-Options-Hardening-Guide-for-C-and-C++
    warning_flags = [
        #	'-Wcast-align=strict',
        '-Wshadow=compatible-local',
        '-Wvla',
        '-Wfloat-equal',
        '-Wnull-dereference',
        '-Wunused-const-variable',
        '-Wunused-parameter',
        '-Wunused-value',
        '-Wduplicated-cond',
        '-Wjump-misses-init',
        '-Wstringop-overflow',
        '-Wlogical-op',
        '-Wshift-overflow',
        '-Wshift-overflow=1',
        '-Wshift-overflow=2',
        '-Wdouble-promotion',
        '-Wformat-security',
        '-Wold-style-definition',
        '-Wstrict-prototypes',
        '-Wmissing-declarations',
        '-Wmissing-prototypes',
        '-Wchar-subscripts',
        '-Wundef',
        '-Wformat',
        '-Wformat=2',
        '-Wint-conversion',  #-Warith-conversion
        '-Wenum-conversion',
        '-Wfloat-conversion',
        '-Wint-to-pointer-cast',
        '-Wimplicit-fallthrough',
        '-D_GLIBCXX_ASSERTIONS',
        '-fstrict-flex-arrays=3',
        '-fno-delete-null-pointer-checks',
        '-fno-strict-overflow',
        '-fno-strict-aliasing',
        '-ftrivial-auto-var-init=zero',
        '-Wtrampolines',  #GCC only at this time
        '-Wincompatible-pointer-types-discards-qualifiers',
        '-Woverlength-strings',
        '-Wnewline-eof',
        '-Wno-c23-extensions',  #We do not want this warning since we are already checking for when C23 extensions are available before we use them. If not, we use a compiler specific definition, or make it an empty definition.
        '-Wparentheses',
        '-Wno-extra-semi',  # codebase widely uses 'ctor() {};' style; too noisy to be useful here
        '-Wcast-qual',
        '-Werror=sometimes-uninitialized',
        '-Wuninitialized',
        '-Wunevaluated-expression',
        '-Wunsequenced',
        '-Wvarargs',
        '-Wwrite-strings',
        '-Wrestrict',
        '-Wstringop-truncation',
        '-Werror=trigraphs',
        '-Wunreachable-code',
        '-Wcomment',
        '-Wsequence-point',
        '-Wreturn-type',
        '-Wpointer-bool-conversion',
        '-Wmismatched-dealloc',
        '-Wfree-nonheap-object',
        '-fnullability',
        '-Wnullability',  # Warnings for nullability annotations
        '-Wnullability-completeness',  # Warn if nullability annotations are incomplete
        '-Wnullability-completeness-on-arrays',  # Warn if nullability annotations are incomplete
        # '-Wnullable-to-nonnull-conversion', # Basically warns so you have to cast to the correct nullability type. Not needed today -TJE
        '-Wswitch-enum',
        '-Walloc-zero',
        '-Walloc-size',
        '-Wcalloc-transposed-args',
        '-Werror=alloca',  # Do not allow use of the alloca function
        '-Wstring-compare',  # Helps detect when strcmp is used when the programmer likely meant to use strncmp
        '-Wno-template-body',  # GCC 14+ -Wtemplate-body false-positives/hard-errors in libstdc++ headers (e.g. exception_ptr.h make_exception_ptr)
    ]

    if cpp.get_id().contains('gcc') and cpp.version().version_compare('>=10.0')
        #only enable the sign conversion warning on versions 10 and up because it is way too noisy on earlier GCC versions than it is useful-TJE
        warning_flags += '-Wsign-conversion'
    endif

    if target_machine.system() != 'sunos'
        warning_flags += '-fstack-protector-strong'
    else
        #Illumos will support this, but solaris will not due to library differences in the systems
        if meson.version().version_compare('>=1.2.0')
            if target_machine.kernel() == 'illumos'
                warning_flags += '-fstack-protector-strong'
            endif
        else
            # TODO: Backup method to detect illumos vs solaris
        endif
    endif

    if cpp.get_id().contains('gcc') and target_machine.system() == 'windows'
        #According to the link below, this is not needed in Windows...it also causes a bug in some versions of GCC for Windows.
        #https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90458
        #NOTE: While this appears to be fixed for versions 11 and 12, the current CI is failing on this with
        #      version 12.2. If we want to or need to enable this, it should be done based on which versions we
        #      know have been patched for this. -TJE
    else
        warning_flags += '-fstack-clash-protection'
    endif

    if target_machine.cpu_family() == 'ppc64'
        #power pc builds generate a lot of warnings/notes about ABI changes since GCC-5
        #this flag is disabling them because this is way too noisy.
        warning_flags += ['-Wno-psabi']
    elif target_machine.cpu_family() == 'x86_64'
        warning_flags += ['-fcf-protection=full']        #this may be linux only at this time.
    elif target_machine.cpu_family() == 'aarch64'
        warning_flags += ['-mbranch-protection=standard']
    endif

    linker_flags += ['-Wl,-z,noexecstack', '-Wl,-z,relro', '-Wl,-z,now']

    #test if setting _FORTIFY_SOURCE higher than default generates a warning
    #this issue showed up on Rocky Linux 8 and we don't want to generate extra noise
    #due to trying to follow openssf best practices
    fortifytest = ''' #include <stdio.h>
                      int main() {
                          return 0;
                      }
                  '''
    fortifyresult = cpp.compiles(
        fortifytest,
        name: 'Set _FORTIFY_SOURCE to highest possible level without warnings',
        args: ['-U_FORTIFY_SOURCE', '-D_FORTIFY_SOURCE=5', '-Werror'],
    )
    if fortifyresult == true
        warning_flags += ['-U_FORTIFY_SOURCE', '-D_FORTIFY_SOURCE=3']
    endif

elif cpp.get_id().contains('msvc')
    #See here for enabling/disabling msvc warnings:
    #https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170
    #warnings off by default: https://learn.microsoft.com/en-us/cpp/preprocessor/compiler-warnings-that-are-off-by-default?view=msvc-170
    warning_flags = [
        #Turn off the following warnings. If using /wall in Windows, many of these show all over the Windows API
        #This is likely not an issue with meson, but matching VS project files for now
        '/wd4214',  # nonstandard extension used : bit field types other than int
        '/wd4201',  # nonstandard extension used : nameless struct/union
        '/wd4668',  # 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'. While like -Wundef, this creates too many warnings in system headers to use
        '/wd4820',  # 'bytes' bytes padding added after construct 'member_name'
        '/wd4710',  # 'function' : function not inlined
        '/wd5045',  # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        '/wd4711',  # function 'function' selected for inline expansion
        '/wd4324',  # 'struct_name' : structure was padded due to __declspec(align())
        '/wd4221',  # nonstandard extension used : 'identifier' : cannot be initialized using address of automatic variable
        '/wd4204',  # nonstandard extension used : non-constant aggregate initializer
        '/wd5105',  # macro expansion producing 'defined' has undefined behavior
        '/wd4746',  # volatile access of '<expression>' is subject to /volatile:[iso|ms] setting; consider using __iso_volatile_load/store intrinsic functions.
        #Turn on the following warnings to make the output more useful or like GCC/clang
        '/w14255',  # 'function' : no function prototype given: converting '()' to '(void)'
        '/w14062',  # enumerator 'identifier' in switch of enum 'enumeration' is not handled
        '/w14101',  # 'identifier' : unreferenced local variable
        '/w14189',  # 'identifier' : local variable is initialized but not referenced
        '/w15031',  # #pragma warning(pop): likely mismatch, popping warning state pushed in different file
        '/w15032',  # detected #pragma warning(push) with no corresponding #pragma warning(pop)
        '/w15262',  # implicit fall-through occurs here; are you missing a break statement? Use [[fallthrough]] when a break statement is intentionally omitted between cases
        '/w14255',  # 'function' : no function prototype given: converting '()' to '(void)' #NOTE: Only needed for /Wall, otherwise enabling can be good-TJE
        '/w14242',  # identifier conversion from type 1 to type 2, possible loss of data (matches -wconversion above)
        '/w14254',  # operator conversion from type 1 to type 2, possible loss of data (matches -wconversion above)
        '/w14287',  # operator: unsigned/negative constant mismatch (matches -wconversion above)
        '/w14296',  # operator: expression is always false
        '/w14365',  # action: conversion from type 1 to type 2, signed/unsigned mismatch (matches -wconversion above)
        '/w14388',  # implicit conversion warning during a comparison (matches -wconversion above)
        '/w14545',  # expression before comma evaluates to a function which is missing an argument list
        '/w14546',  # function call before comma missing argument list
        '/w14547',  # 'operator' : operator before comma has no effect; expected operator with side-effect
        '/w14548',  # expression before comma has no effect; expected expression with side-effect
        '/w14549',  # 'operator1': operator before comma has no effect; did you intend 'operator2'?
        '/w14574',  # 'identifier' is defined to be '0': did you mean to use '#if identifier'?
        '/w14605',  # 	'/Dmacro' specified on current command line, but was not specified when precompiled header was built
        '/w14555',  # expression has no effect; expected expression with side-effect
        '/w14774',  # 'string' : format string expected in argument number is not a string literal
        '/w14777',  # 'function' : format string 'string' requires an argument of type 'type1', but variadic argument number has type 'type2'
        '/w14826',  # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior (more -wconversion)
        '/w15219',  # implicit conversion from 'type-1' to 'type-2', possible loss of data (-wconversion)
        '/w15240',  # 'attribute-name': attribute is ignored in this syntactic position
        '/w15245',  # 'function': unreferenced function with internal linkage has been removed
        '/w14555',  # expression has no effect; expected expression with side-effect
        '/w15264',  # 'variable-name': 'const' variable is not used
        '/w24302',  # 'conversion': truncation from 'type1' to 'type2'
        '/w14311',  # 'variable': pointer truncation from 'type' to 'type'
        '/w14312',  # 'operation': conversion from 'type1' to 'type2' of greater size
        '/w14319',  # 'operator': zero extending 'type1' to 'type2' of greater size
        '/w14702',  # warning for unreachable code
        #Treat the following as errors
        '/we4431',  # missing type specifier - int assumed. Note: C no longer supports default-int
        '/we4905',  # wide string literal cast to 'LPSTR'
        '/we4906',  # string literal cast to 'LPWSTR'
        '/we4837',  # trigraph detected: '??character' replaced by 'character'
        '/we4628',  # digraphs not supported with -Ze. Character sequence 'digraph' not interpreted as alternate token for 'char'
        '/we4289',  # nonstandard extension used : 'var' : loop control variable declared in the for-loop is used outside the for-loop scope
        #'/we4464',  # relative include path contains '..'
        '/GS',  #security cookie for stack protection
        '/sdl',  #adds recommended security development lifecycle checks
        '/Qspectre',
        '/guard:cf',  #control flow guard
        '/d2guard4',  #control flow guard
    ]

    if target_machine.cpu_family() == 'x86_64' or target_machine.cpu_family() == 'x86'
        # https://learn.microsoft.com/en-us/cpp/build/reference/qintel-jcc-erratum?view=msvc-170
        warning_flags += '/QIntel-jcc-erratum'
    endif

    linker_flags += [
        '/guard:cf',  #control flow guard
        '/SafeSEH',  #on by default in x64 so it is unrecognized otherwise.
        '/NXCOMPAT',  #data execution prevention
        '/dynamicbase',  #address space randomization
    ]

    #TODO: check compiler version to handle warnings that were off by default in earlier versions
    #ex: C4431 (level 4)	missing type specifier - int assumed. Note: C no longer supports default-int
    #    This was off by default in compilers before VS2012.
elif cpp.get_id().contains('xlc')
    #This section is for IBM's xlc compiler and warning options it may need.
    #NOTE: xlcclang should be handled above
    #See following links:
    #https://www.ibm.com/docs/en/xl-c-and-cpp-aix/16.1?topic=reference-supported-xl-compiler-options-by-different-invocations
    #https://www.ibm.com/docs/en/xl-c-and-cpp-aix/16.1?topic=end-mapping-legacy-xl-compiler-options-gcc-options
    #https://www.ibm.com/docs/en/xl-c-and-cpp-aix/16.1?topic=reference-individual-xl-compiler-option-descriptions
    warning_flags = []
endif

add_project_arguments(cpp.get_supported_arguments(warning_flags), language: 'cpp')
add_project_link_arguments(cpp.get_supported_link_arguments(linker_flags), language: 'cpp')


# Platform-specific adjustments (example: darwin fast flag for release builds)
if machine == 'darwin' and buildtype != 'debug'
    cpp_args += ['-fast']
endif

# Project option: json_safe
if get_option('json_safe').enabled()
    cpp_args += ['-DJSON_SAFE']
endif

# Adjustments based on architecture
# This ad-hoc architecture logic is commented out in favor of a proper native/cross file.
# Example native-file snippet to force 32-bit toolchain (pass with `meson --native-file=my32.native`):
# [binaries]
# c = '/usr/bin/gcc'
# cpp = '/usr/bin/g++'
# and include -m32 via the compiler invocation if needed.
# Example cross-file snippet (host_machine):
# [host_machine]
# cpu_family = 'x86'
# cpu = 'i686'
# endian = 'little'
# The following ad-hoc logic is intentionally disabled:
# if architecture == 'x86_32'
#     cpp_args += ['-m32']
# endif

if target_machine.system() == 'windows'
    add_project_arguments('-D_CRT_SECURE_NO_WARNINGS', language: 'c')
endif

common_sources = [
    '_internal/Source/internalJSONNode.cpp',
    '_internal/Source/JSONAllocator.cpp',
    '_internal/Source/JSONChildren.cpp',
    '_internal/Source/JSONDebug.cpp',
    '_internal/Source/JSONIterators.cpp',
    '_internal/Source/JSONMemory.cpp',
    '_internal/Source/JSONNode.cpp',
    '_internal/Source/JSONNode_Mutex.cpp',
    '_internal/Source/JSONPreparse.cpp',
    '_internal/Source/JSONStream.cpp',
    '_internal/Source/JSONValidator.cpp',
    '_internal/Source/JSONWorker.cpp',
    '_internal/Source/JSONWriter.cpp',
    '_internal/Source/libjson.cpp',
]

# TODO: Handle when passed building static versus dynamic library. Do not allow the "both" option.


libjson = static_library('libjson', common_sources, cpp_args: cpp_args, include_directories: incdir)
libjson_dep = declare_dependency(link_with: libjson, include_directories: incdir)

# Gate legacy test suites with a project option in meson_options.txt
if get_option('enable_legacy_tests')
    subdir('_internal/TestSuite2')
    subdir('_internal/TestSuite')
endif
