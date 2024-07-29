#-------------------------------------#
#  This is the meson file             #
#-------------------------------------#

project('libjson', 'cpp')

# Configuration

# adds c compiler
cpp = meson.get_compiler('cpp')
cpp_version = cpp.version()

machine = target_machine.system()
architecture = target_machine.cpu_family()


# Targets
libname = 'json'
#major_version = '7'
#minor_version = '6.1'


incdir = ['_internal/Source','./']
objdir = 'Objects'

# Defaults
cxxflags_default = ['-O3', '-ffast-math', '-fexpensive-optimizations', '-DNDEBUG']
cxxflags_shared = ['-fPIC']
cxxflags_small = ['-Os', '-ffast-math', '-DJSON_LESS_MEMORY', '-DNDEBUG', '-fPIC']
cxxflags_debug = ['-g', '-DJSON_SAFE', '-DJSON_DEBUG', '-fPIC']


if cpp.get_id().contains('gcc')
  if cpp.version().version_compare('<5.0')
    if cpp.has_argument('-std=gnu++11')
      # Add this argument to the list since C++11 is a minimum required C++ compiler standard
      add_project_arguments('-std=gnu++11', language: 'cpp')
    else
      error('C++11/GNU++11 standard is required but was not able to be set!')
    endif
  endif
endif

# Adjustments based on OS
if machine == 'darwin'
    cxxflags_default += ['-fast']
else
    cxxflags_default += ['-fPIC']
endif


# This is for cpp arguments
cpp_args = []

# Adjustments based on architecture
if architecture == 'x86_32'
    cpp_args += ['-m32']
endif

# Build type specific flags
#build_type = get_option('build_type')
#if build_type == 'small'
    cpp_args += cxxflags_small
#elif build_type == 'debug'
    cpp_args += cxxflags_debug
#else
    cpp_args += cxxflags_default
#endif

# Shared library settings
#shared = get_option('shared')

#lib_target = libname + '.' + (shared ? 'so' : 'a')

#libdir = join_paths(get_option('prefix'), 'lib')

#includedir = join_paths(get_option('prefix'), 'include', libname)

# Targets
#objs = []

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
    '_internal/Source/libjson.cpp'
]


libjson = static_library('libjson', common_sources, c_args: cpp_args,include_directories : incdir)
libjson_dep = declare_dependency(link_with : libjson, include_directories : incdir)
