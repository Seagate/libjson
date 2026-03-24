# Meson build for libjson TestSuite2

test2_sources = [
    '../TestSuite/UnitTest.cpp',
    # JSON_Base64
    'JSON_Base64/json_encode64.cpp',
    'JSON_Base64/json_decode64.cpp',
    # NumberToString
    'NumberToString/getLenSize.cpp',
    'NumberToString/isNumeric.cpp',
    'NumberToString/_uitoa.cpp',
    'NumberToString/_itoa.cpp',
    'NumberToString/_ftoa.cpp',
    'NumberToString/_atof.cpp',
    'NumberToString/_areFloatsEqual.cpp',
    # JSONValidator
    'JSONValidator/securityTest.cpp',
    'JSONValidator/isValidString.cpp',
    'JSONValidator/isValidRoot.cpp',
    'JSONValidator/isValidPartialRoot.cpp',
    'JSONValidator/isValidObject.cpp',
    'JSONValidator/isValidNumber.cpp',
    'JSONValidator/isValidNamedObject.cpp',
    'JSONValidator/isValidMember.cpp',
    'JSONValidator/isValidArray.cpp',
    # JSONDebug / JSONGlobals
    'JSONDebug/JSON_ASSERT.cpp',
    'JSONDebug/JSON_ASSERT_SAFE.cpp',
    'JSONDebug/JSON_FAIL_SAFE.cpp',
    'JSONDebug/JSON_FAIL.cpp',
    'JSONGlobals/jsonSingleton.cpp',
]

test_args = []
if target_machine.system() == 'windows'
    test_args += ['-D_CRT_SECURE_NO_WARNINGS']
endif

libjson_tests2 = static_library(
    'libjson_testsuite2',
    test2_sources,
    include_directories: ['../Source', '../../'],
    dependencies: [libjson_dep],
    cpp_args: test_args,
)

libjson_tests2_dep = declare_dependency(link_with: libjson_tests2, include_directories: ['../Source', '../../'])
