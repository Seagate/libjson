# Meson build for libjson TestSuite

# Sources for the main TestSuite
test_sources = [
    'main.cpp',
    'RunTestSuite2.cpp',
    'TestAssign.cpp',
    'TestBinary.cpp',
    'TestChildren.cpp',
    'TestComments.cpp',
    'TestConverters.cpp',
    'TestCtors.cpp',
    'TestEquality.cpp',
    'TestFunctions.cpp',
    'TestInequality.cpp',
    'TestInspectors.cpp',
    'TestIterators.cpp',
    'TestMutex.cpp',
    'TestNamespace.cpp',
    'TestRefCounting.cpp',
    'TestSharedString.cpp',
    'TestStreams.cpp',
    'TestString.cpp',
    'TestSuite.cpp',
    'TestValidator.cpp',
    'TestWriter.cpp',
    'UnitTest.cpp',
]

# Build test binary linked against the libjson static library from this subproject
libjson_tests = executable(
    'libjson_testsuite',
    test_sources,
    include_directories: ['../Source', '../../'],
    dependencies: [libjson_dep, libjson_tests2_dep],
)

test('libjson_testsuite', libjson_tests)
