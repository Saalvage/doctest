//
// doctest.h - the lightest feature-rich C++ single-header testing framework for unit tests and TDD
//
// Copyright (c) 2016-2021 Viktor Kirilov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
// The documentation can be found at the library's page:
// https://github.com/onqtam/doctest/blob/master/doc/markdown/readme.md
//
// =================================================================================================
// =================================================================================================
// =================================================================================================
//
// The library is heavily influenced by Catch - https://github.com/catchorg/Catch2
// which uses the Boost Software License - Version 1.0
// see here - https://github.com/catchorg/Catch2/blob/master/LICENSE.txt
//
// The concept of subcases (sections in Catch) and expression decomposition are from there.
// Some parts of the code are taken directly:
// - stringification - the detection of "ostream& operator<<(ostream&, const T&)" and StringMaker<>
// - the Approx() helper class for floating point comparison
// - colors in the console
// - breaking into a debugger
// - signal / SEH handling
// - timer
// - XmlWriter class - thanks to Phil Nash for allowing the direct reuse (AKA copy/paste)
//
// The expression decomposing templates are taken from lest - https://github.com/martinmoene/lest
// which uses the Boost Software License - Version 1.0
// see here - https://github.com/martinmoene/lest/blob/master/LICENSE.txt
//
// =================================================================================================
// =================================================================================================
// =================================================================================================

#ifndef DOCTEST_LIBRARY_INCLUDED
#define DOCTEST_LIBRARY_INCLUDED

// =================================================================================================
// == VERSION ======================================================================================
// =================================================================================================

#define DOCTEST_VERSION_MAJOR 2
#define DOCTEST_VERSION_MINOR 4
#define DOCTEST_VERSION_PATCH 7
#define DOCTEST_VERSION_STR "2.4.7"

#define DOCTEST_VERSION                                                                            \
    (DOCTEST_VERSION_MAJOR * 10000 + DOCTEST_VERSION_MINOR * 100 + DOCTEST_VERSION_PATCH)

// =================================================================================================
// == COMPILER VERSION =============================================================================
// =================================================================================================

// ideas for the version stuff are taken from here: https://github.com/cxxstuff/cxx_detect

#define DOCTEST_COMPILER(MAJOR, MINOR, PATCH) ((MAJOR)*10000000 + (MINOR)*100000 + (PATCH))

// GCC/Clang and GCC/MSVC are mutually exclusive, but Clang/MSVC are not because of clang-cl...
#if defined(_MSC_VER) && defined(_MSC_FULL_VER)
#if _MSC_VER == _MSC_FULL_VER / 10000
#define DOCTEST_MSVC DOCTEST_COMPILER(_MSC_VER / 100, _MSC_VER % 100, _MSC_FULL_VER % 10000)
#else // MSVC
#define DOCTEST_MSVC                                                                               \
    DOCTEST_COMPILER(_MSC_VER / 100, (_MSC_FULL_VER / 100000) % 100, _MSC_FULL_VER % 100000)
#endif // MSVC
#endif // MSVC
#if defined(__clang__) && defined(__clang_minor__)
#define DOCTEST_CLANG DOCTEST_COMPILER(__clang_major__, __clang_minor__, __clang_patchlevel__)
#elif defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__) &&              \
        !defined(__INTEL_COMPILER)
#define DOCTEST_GCC DOCTEST_COMPILER(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#endif // GCC

#ifndef DOCTEST_MSVC
#define DOCTEST_MSVC 0
#endif // DOCTEST_MSVC
#ifndef DOCTEST_CLANG
#define DOCTEST_CLANG 0
#endif // DOCTEST_CLANG
#ifndef DOCTEST_GCC
#define DOCTEST_GCC 0
#endif // DOCTEST_GCC

#ifdef _MSVC_LANG
#define DOCTEST_CPP_VER_DATE _MSVC_LANG
#else
#define DOCTEST_CPP_VER_DATE __cplusplus
#endif

#if DOCTEST_CPP_VER_DATE >= 202000L
#define DOCTEST_CPP 20
#elif DOCTEST_CPP_VER_DATE >= 201700L
#define DOCTEST_CPP 17
#elif DOCTEST_CPP_VER_DATE >= 201400L
#define DOCTEST_CPP 14
#elif DOCTEST_CPP_VER_DATE >= 201100L
#define DOCTEST_CPP 11
#else
#define DOCTEST_CPP 03
#endif

// =================================================================================================
// == COMPILER WARNINGS HELPERS ====================================================================
// =================================================================================================

#if DOCTEST_CLANG
#define DOCTEST_PRAGMA_TO_STR(x) _Pragma(#x)
#define DOCTEST_CLANG_SUPPRESS_WARNING_PUSH _Pragma("clang diagnostic push")
#define DOCTEST_CLANG_SUPPRESS_WARNING(w) DOCTEST_PRAGMA_TO_STR(clang diagnostic ignored w)
#define DOCTEST_CLANG_SUPPRESS_WARNING_POP _Pragma("clang diagnostic pop")
#define DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH(w)                                                \
    DOCTEST_CLANG_SUPPRESS_WARNING_PUSH DOCTEST_CLANG_SUPPRESS_WARNING(w)
#else // DOCTEST_CLANG
#define DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
#define DOCTEST_CLANG_SUPPRESS_WARNING(w)
#define DOCTEST_CLANG_SUPPRESS_WARNING_POP
#define DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH(w)
#endif // DOCTEST_CLANG

#if DOCTEST_GCC
#define DOCTEST_PRAGMA_TO_STR(x) _Pragma(#x)
#define DOCTEST_GCC_SUPPRESS_WARNING_PUSH _Pragma("GCC diagnostic push")
#define DOCTEST_GCC_SUPPRESS_WARNING(w) DOCTEST_PRAGMA_TO_STR(GCC diagnostic ignored w)
#define DOCTEST_GCC_SUPPRESS_WARNING_POP _Pragma("GCC diagnostic pop")
#define DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH(w)                                                  \
    DOCTEST_GCC_SUPPRESS_WARNING_PUSH DOCTEST_GCC_SUPPRESS_WARNING(w)
#else // DOCTEST_GCC
#define DOCTEST_GCC_SUPPRESS_WARNING_PUSH
#define DOCTEST_GCC_SUPPRESS_WARNING(w)
#define DOCTEST_GCC_SUPPRESS_WARNING_POP
#define DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif // DOCTEST_GCC

#if DOCTEST_MSVC
#define DOCTEST_MSVC_SUPPRESS_WARNING_PUSH __pragma(warning(push))
#define DOCTEST_MSVC_SUPPRESS_WARNING(w) __pragma(warning(disable : w))
#define DOCTEST_MSVC_SUPPRESS_WARNING_POP __pragma(warning(pop))
#define DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)                                                 \
    DOCTEST_MSVC_SUPPRESS_WARNING_PUSH DOCTEST_MSVC_SUPPRESS_WARNING(w)
#else // DOCTEST_MSVC
#define DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
#define DOCTEST_MSVC_SUPPRESS_WARNING(w)
#define DOCTEST_MSVC_SUPPRESS_WARNING_POP
#define DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif // DOCTEST_MSVC

// =================================================================================================
// == COMPILER WARNINGS ============================================================================
// =================================================================================================

DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
DOCTEST_CLANG_SUPPRESS_WARNING("-Wunknown-pragmas")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wnon-virtual-dtor")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wweak-vtables")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wpadded")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wdeprecated")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wunused-local-typedef")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wc++98-compat")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")

DOCTEST_GCC_SUPPRESS_WARNING_PUSH
DOCTEST_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")
DOCTEST_GCC_SUPPRESS_WARNING("-Wpragmas")
DOCTEST_GCC_SUPPRESS_WARNING("-Weffc++")
DOCTEST_GCC_SUPPRESS_WARNING("-Wstrict-overflow")
DOCTEST_GCC_SUPPRESS_WARNING("-Wstrict-aliasing")
DOCTEST_GCC_SUPPRESS_WARNING("-Wctor-dtor-privacy")
DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_GCC_SUPPRESS_WARNING("-Wnon-virtual-dtor")
DOCTEST_GCC_SUPPRESS_WARNING("-Wunused-local-typedefs")
DOCTEST_GCC_SUPPRESS_WARNING("-Wuseless-cast")
DOCTEST_GCC_SUPPRESS_WARNING("-Wnoexcept")
DOCTEST_GCC_SUPPRESS_WARNING("-Wsign-promo")

DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
DOCTEST_MSVC_SUPPRESS_WARNING(4616) // invalid compiler warning
DOCTEST_MSVC_SUPPRESS_WARNING(4619) // invalid compiler warning
DOCTEST_MSVC_SUPPRESS_WARNING(4996) // The compiler encountered a deprecated declaration
DOCTEST_MSVC_SUPPRESS_WARNING(4706) // assignment within conditional expression
DOCTEST_MSVC_SUPPRESS_WARNING(4512) // 'class' : assignment operator could not be generated
DOCTEST_MSVC_SUPPRESS_WARNING(4127) // conditional expression is constant
DOCTEST_MSVC_SUPPRESS_WARNING(4820) // padding
DOCTEST_MSVC_SUPPRESS_WARNING(4625) // copy constructor was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(4626) // assignment operator was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(5027) // move assignment operator was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(5026) // move constructor was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(4623) // default constructor was implicitly defined as deleted
DOCTEST_MSVC_SUPPRESS_WARNING(4640) // construction of local static object is not thread-safe
DOCTEST_MSVC_SUPPRESS_WARNING(5045) // Spectre mitigation for memory load
// static analysis
DOCTEST_MSVC_SUPPRESS_WARNING(26439) // This kind of function may not throw. Declare it 'noexcept'
DOCTEST_MSVC_SUPPRESS_WARNING(26495) // Always initialize a member variable
DOCTEST_MSVC_SUPPRESS_WARNING(26451) // Arithmetic overflow ...
DOCTEST_MSVC_SUPPRESS_WARNING(26444) // Avoid unnamed objects with custom construction and dtr...
DOCTEST_MSVC_SUPPRESS_WARNING(26812) // Prefer 'enum class' over 'enum'

// 4548 - expression before comma has no effect; expected expression with side - effect
// 4265 - class has virtual functions, but destructor is not virtual
// 4986 - exception specification does not match previous declaration
// 4350 - behavior change: 'member1' called instead of 'member2'
// 4668 - 'x' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
// 4365 - conversion from 'int' to 'unsigned long', signed/unsigned mismatch
// 4774 - format string expected in argument 'x' is not a string literal
// 4820 - padding in structs

// only 4 should be disabled globally:
// - 4514 # unreferenced inline function has been removed
// - 4571 # SEH related
// - 4710 # function not inlined
// - 4711 # function 'x' selected for automatic inline expansion

#define DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN                                 \
    DOCTEST_MSVC_SUPPRESS_WARNING_PUSH                                                             \
    DOCTEST_MSVC_SUPPRESS_WARNING(4548)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4265)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4986)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4350)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4668)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4365)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4774)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4820)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4625)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4626)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(5027)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(5026)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4623)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(5039)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(5045)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(5105)                                                            \
    DOCTEST_MSVC_SUPPRESS_WARNING(4738)

#define DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END DOCTEST_MSVC_SUPPRESS_WARNING_POP

// =================================================================================================
// == FEATURE DETECTION ============================================================================
// =================================================================================================

// general compiler feature support table: https://en.cppreference.com/w/cpp/compiler_support
// MSVC C++11 feature support table: https://msdn.microsoft.com/en-us/library/hh567368.aspx
// GCC C++11 feature support table: https://gcc.gnu.org/projects/cxx-status.html
// MSVC version table:
// https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B#Internal_version_numbering
// MSVC++ 14.2 (16) _MSC_VER == 1920 (Visual Studio 2019)
// MSVC++ 14.1 (15) _MSC_VER == 1910 (Visual Studio 2017)
// MSVC++ 14.0      _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 12.0      _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 11.0      _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 10.0      _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 9.0       _MSC_VER == 1500 (Visual Studio 2008)
// MSVC++ 8.0       _MSC_VER == 1400 (Visual Studio 2005)

#if DOCTEST_MSVC && !defined(DOCTEST_CONFIG_WINDOWS_SEH)
#define DOCTEST_CONFIG_WINDOWS_SEH
#endif // MSVC
#if defined(DOCTEST_CONFIG_NO_WINDOWS_SEH) && defined(DOCTEST_CONFIG_WINDOWS_SEH)
#undef DOCTEST_CONFIG_WINDOWS_SEH
#endif // DOCTEST_CONFIG_NO_WINDOWS_SEH

#if !defined(_WIN32) && !defined(__QNX__) && !defined(DOCTEST_CONFIG_POSIX_SIGNALS) &&             \
        !defined(__EMSCRIPTEN__)
#define DOCTEST_CONFIG_POSIX_SIGNALS
#endif // _WIN32
#if defined(DOCTEST_CONFIG_NO_POSIX_SIGNALS) && defined(DOCTEST_CONFIG_POSIX_SIGNALS)
#undef DOCTEST_CONFIG_POSIX_SIGNALS
#endif // DOCTEST_CONFIG_NO_POSIX_SIGNALS

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
#if !defined(__cpp_exceptions) && !defined(__EXCEPTIONS) && !defined(_CPPUNWIND)
#define DOCTEST_CONFIG_NO_EXCEPTIONS
#endif // no exceptions
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

#ifdef DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
#define DOCTEST_CONFIG_NO_EXCEPTIONS
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#if defined(DOCTEST_CONFIG_NO_EXCEPTIONS) && !defined(DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS)
#define DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS && !DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS

#if defined(DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN) && !defined(DOCTEST_CONFIG_IMPLEMENT)
#define DOCTEST_CONFIG_IMPLEMENT
#endif // DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#if defined(_WIN32) || defined(__CYGWIN__)
#if DOCTEST_MSVC
#define DOCTEST_SYMBOL_EXPORT __declspec(dllexport)
#define DOCTEST_SYMBOL_IMPORT __declspec(dllimport)
#else // MSVC
#define DOCTEST_SYMBOL_EXPORT __attribute__((dllexport))
#define DOCTEST_SYMBOL_IMPORT __attribute__((dllimport))
#endif // MSVC
#else  // _WIN32
#define DOCTEST_SYMBOL_EXPORT __attribute__((visibility("default")))
#define DOCTEST_SYMBOL_IMPORT
#endif // _WIN32

#ifdef DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#ifdef DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_INTERFACE DOCTEST_SYMBOL_EXPORT
#else // DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_INTERFACE DOCTEST_SYMBOL_IMPORT
#endif // DOCTEST_CONFIG_IMPLEMENT
#else  // DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#define DOCTEST_INTERFACE
#endif // DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL

#define DOCTEST_EMPTY

#if DOCTEST_MSVC
#define DOCTEST_NOINLINE __declspec(noinline)
#define DOCTEST_UNUSED
#define DOCTEST_ALIGNMENT(x)
#elif DOCTEST_CLANG && DOCTEST_CLANG < DOCTEST_COMPILER(3, 5, 0)
#define DOCTEST_NOINLINE
#define DOCTEST_UNUSED
#define DOCTEST_ALIGNMENT(x)
#else
#define DOCTEST_NOINLINE __attribute__((noinline))
#define DOCTEST_UNUSED __attribute__((unused))
#define DOCTEST_ALIGNMENT(x) __attribute__((aligned(x)))
#endif

#ifndef DOCTEST_NORETURN
#if DOCTEST_MSVC && (DOCTEST_MSVC < DOCTEST_COMPILER(19, 0, 0))
#define DOCTEST_NORETURN
#else // DOCTEST_MSVC
#define DOCTEST_NORETURN [[noreturn]]
#endif // DOCTEST_MSVC
#endif // DOCTEST_NORETURN

#ifndef DOCTEST_NOEXCEPT
#if DOCTEST_MSVC && (DOCTEST_MSVC < DOCTEST_COMPILER(19, 0, 0))
#define DOCTEST_NOEXCEPT
#else // DOCTEST_MSVC
#define DOCTEST_NOEXCEPT noexcept
#endif // DOCTEST_MSVC
#endif // DOCTEST_NOEXCEPT

#ifndef DOCTEST_CONSTEXPR
#if DOCTEST_MSVC && (DOCTEST_MSVC < DOCTEST_COMPILER(19, 0, 0))
#define DOCTEST_CONSTEXPR const
#else // DOCTEST_MSVC
#define DOCTEST_CONSTEXPR constexpr
#endif // DOCTEST_MSVC
#endif // DOCTEST_CONSTEXPR

// =================================================================================================
// == FEATURE DETECTION END ========================================================================
// =================================================================================================

// internal macros for string concatenation and anonymous variable name generation
#define DOCTEST_CAT_IMPL(s1, s2) s1##s2
#define DOCTEST_CAT(s1, s2) DOCTEST_CAT_IMPL(s1, s2)
#ifdef __COUNTER__ // not standard and may be missing for some compilers
#define DOCTEST_ANONYMOUS(x) DOCTEST_CAT(x, __COUNTER__)
#else // __COUNTER__
#define DOCTEST_ANONYMOUS(x) DOCTEST_CAT(x, __LINE__)
#endif // __COUNTER__

#define DOCTEST_TOSTR(x) #x

#ifndef DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE
#define DOCTEST_REF_WRAP(x) x&
#else // DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE
#define DOCTEST_REF_WRAP(x) x
#endif // DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE

// not using __APPLE__ because... this is how Catch does it
#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED
#define DOCTEST_PLATFORM_MAC
#elif defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
#define DOCTEST_PLATFORM_IPHONE
#elif defined(_WIN32)
#define DOCTEST_PLATFORM_WINDOWS
#else // DOCTEST_PLATFORM
#define DOCTEST_PLATFORM_LINUX
#endif // DOCTEST_PLATFORM

#define DOCTEST_GLOBAL_NO_WARNINGS(var)                                                            \
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wglobal-constructors")                              \
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wunused-variable")                                            \
    static const int var DOCTEST_UNUSED // NOLINT(fuchsia-statically-constructed-objects,cert-err58-cpp)
#define DOCTEST_GLOBAL_NO_WARNINGS_END() DOCTEST_CLANG_SUPPRESS_WARNING_POP

#ifndef DOCTEST_BREAK_INTO_DEBUGGER
// should probably take a look at https://github.com/scottt/debugbreak
#ifdef DOCTEST_PLATFORM_LINUX
#if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
// Break at the location of the failing check if possible
#define DOCTEST_BREAK_INTO_DEBUGGER() __asm__("int $3\n" : :) // NOLINT (hicpp-no-assembler)
#else
#include <signal.h>
#define DOCTEST_BREAK_INTO_DEBUGGER() raise(SIGTRAP)
#endif
#elif defined(DOCTEST_PLATFORM_MAC)
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__i386)
#define DOCTEST_BREAK_INTO_DEBUGGER() __asm__("int $3\n" : :) // NOLINT (hicpp-no-assembler)
#else
#define DOCTEST_BREAK_INTO_DEBUGGER() __asm__("brk #0"); // NOLINT (hicpp-no-assembler)
#endif
#elif DOCTEST_MSVC
#define DOCTEST_BREAK_INTO_DEBUGGER() __debugbreak()
#elif defined(__MINGW32__)
DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wredundant-decls")
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
DOCTEST_GCC_SUPPRESS_WARNING_POP
#define DOCTEST_BREAK_INTO_DEBUGGER() ::DebugBreak()
#else // linux
#define DOCTEST_BREAK_INTO_DEBUGGER() (static_cast<void>(0))
#endif // linux
#endif // DOCTEST_BREAK_INTO_DEBUGGER

// this is kept here for backwards compatibility since the config option was changed
#ifdef DOCTEST_CONFIG_USE_IOSFWD
#define DOCTEST_CONFIG_USE_STD_HEADERS
#endif // DOCTEST_CONFIG_USE_IOSFWD

#ifdef DOCTEST_CONFIG_USE_STD_HEADERS
#ifndef DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#define DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#endif // DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#include <iosfwd>
#include <cstddef>
#include <ostream>
#include <istream>
#else // DOCTEST_CONFIG_USE_STD_HEADERS

#if DOCTEST_CLANG
// to detect if libc++ is being used with clang (the _LIBCPP_VERSION identifier)
#include <ciso646>
#endif // clang

#ifdef _LIBCPP_VERSION
#define DOCTEST_STD_NAMESPACE_BEGIN _LIBCPP_BEGIN_NAMESPACE_STD
#define DOCTEST_STD_NAMESPACE_END _LIBCPP_END_NAMESPACE_STD
#else // _LIBCPP_VERSION
#define DOCTEST_STD_NAMESPACE_BEGIN namespace std {
#define DOCTEST_STD_NAMESPACE_END }
#endif // _LIBCPP_VERSION

// Forward declaring 'X' in namespace std is not permitted by the C++ Standard.
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4643)

DOCTEST_STD_NAMESPACE_BEGIN // NOLINT (cert-dcl58-cpp)
typedef decltype(nullptr) nullptr_t;
template <class charT>
struct char_traits;
template <>
struct char_traits<char>;
template <class charT, class traits>
class basic_ostream;
typedef basic_ostream<char, char_traits<char>> ostream;
template <class charT, class traits>
class basic_istream;
typedef basic_istream<char, char_traits<char>> istream;
template <class... Types>
class tuple;
#if DOCTEST_MSVC >= DOCTEST_COMPILER(19, 20, 0)
DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wreserved-identifier")
// see this issue on why this is needed: https://github.com/onqtam/doctest/issues/183
template <class _Ty>
class allocator;
template <class _Elem, class _Traits, class _Alloc>
class basic_string;
using string = basic_string<char, char_traits<char>, allocator<char>>;
DOCTEST_CLANG_SUPPRESS_WARNING_POP
#endif // VS 2019
DOCTEST_STD_NAMESPACE_END

DOCTEST_MSVC_SUPPRESS_WARNING_POP

#endif // DOCTEST_CONFIG_USE_STD_HEADERS

#ifdef DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#include <type_traits>
#endif // DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS

namespace doctest {

DOCTEST_INTERFACE extern bool is_running_in_test;

// A 24 byte string class (can be as small as 17 for x64 and 13 for x86) that can hold strings with length
// of up to 23 chars on the stack before going on the heap - the last byte of the buffer is used for:
// - "is small" bit - the highest bit - if "0" then it is small - otherwise its "1" (128)
// - if small - capacity left before going on the heap - using the lowest 5 bits
// - if small - 2 bits are left unused - the second and third highest ones
// - if small - acts as a null terminator if strlen() is 23 (24 including the null terminator)
//              and the "is small" bit remains "0" ("as well as the capacity left") so its OK
// Idea taken from this lecture about the string implementation of facebook/folly - fbstring
// https://www.youtube.com/watch?v=kPR8h4-qZdk
// TODO:
// - optimizations - like not deleting memory unnecessarily in operator= and etc.
// - resize/reserve/clear
// - substr
// - replace
// - back/front
// - iterator stuff
// - find & friends
// - push_back/pop_back
// - assign/insert/erase
// - relational operators as free functions - taking const char* as one of the params
class DOCTEST_INTERFACE String
{
    static const unsigned len  = 24;      //!OCLINT avoid private static members
    static const unsigned last = len - 1; //!OCLINT avoid private static members

    struct view // len should be more than sizeof(view) - because of the final byte for flags
    {
        char*    ptr;
        unsigned size;
        unsigned capacity;
    };

    union
    {
        char buf[len];
        view data;
    };

    char* allocate(unsigned sz);

    bool isOnStack() const { return (buf[last] & 128) == 0; }
    void setOnHeap();
    void setLast(unsigned in = last);

    void copy(const String& other);

public:
    String();
    ~String();

    // cppcheck-suppress noExplicitConstructor
    String(const char* in);
    String(const char* in, unsigned in_size);

    String(std::istream& in, unsigned in_size);

    String(const String& other);
    String& operator=(const String& other);

    String& operator+=(const String& other);

    String(String&& other);
    String& operator=(String&& other);

    char  operator[](unsigned i) const;
    char& operator[](unsigned i);

    // the only functions I'm willing to leave in the interface - available for inlining
    const char* c_str() const { return const_cast<String*>(this)->c_str(); } // NOLINT
    char*       c_str() {
        if(isOnStack())
            return reinterpret_cast<char*>(buf);
        return data.ptr;
    }

    unsigned size() const;
    unsigned capacity() const;

    int compare(const char* other, bool no_case = false) const;
    int compare(const String& other, bool no_case = false) const;
};

DOCTEST_INTERFACE String operator+(const String& lhs, const String& rhs);

DOCTEST_INTERFACE bool operator==(const String& lhs, const String& rhs);
DOCTEST_INTERFACE bool operator!=(const String& lhs, const String& rhs);
DOCTEST_INTERFACE bool operator<(const String& lhs, const String& rhs);
DOCTEST_INTERFACE bool operator>(const String& lhs, const String& rhs);
DOCTEST_INTERFACE bool operator<=(const String& lhs, const String& rhs);
DOCTEST_INTERFACE bool operator>=(const String& lhs, const String& rhs);

DOCTEST_INTERFACE std::ostream& operator<<(std::ostream& s, const String& in);

namespace Color {
    enum Enum
    {
        None = 0,
        White,
        Red,
        Green,
        Blue,
        Cyan,
        Yellow,
        Grey,

        Bright = 0x10,

        BrightRed   = Bright | Red,
        BrightGreen = Bright | Green,
        LightGrey   = Bright | Grey,
        BrightWhite = Bright | White
    };

    DOCTEST_INTERFACE std::ostream& operator<<(std::ostream& s, Color::Enum code);
} // namespace Color

namespace assertType {
    enum Enum
    {
        // macro traits

        is_warn    = 1,
        is_check   = 2 * is_warn,
        is_require = 2 * is_check,

        is_normal      = 2 * is_require,
        is_throws      = 2 * is_normal,
        is_throws_as   = 2 * is_throws,
        is_throws_with = 2 * is_throws_as,
        is_nothrow     = 2 * is_throws_with,

        is_false = 2 * is_nothrow,
        is_unary = 2 * is_false, // not checked anywhere - used just to distinguish the types

        is_eq = 2 * is_unary,
        is_ne = 2 * is_eq,

        is_lt = 2 * is_ne,
        is_gt = 2 * is_lt,

        is_ge = 2 * is_gt,
        is_le = 2 * is_ge,

        // macro types

        DT_WARN    = is_normal | is_warn,
        DT_CHECK   = is_normal | is_check,
        DT_REQUIRE = is_normal | is_require,

        DT_WARN_FALSE    = is_normal | is_false | is_warn,
        DT_CHECK_FALSE   = is_normal | is_false | is_check,
        DT_REQUIRE_FALSE = is_normal | is_false | is_require,

        DT_WARN_THROWS    = is_throws | is_warn,
        DT_CHECK_THROWS   = is_throws | is_check,
        DT_REQUIRE_THROWS = is_throws | is_require,

        DT_WARN_THROWS_AS    = is_throws_as | is_warn,
        DT_CHECK_THROWS_AS   = is_throws_as | is_check,
        DT_REQUIRE_THROWS_AS = is_throws_as | is_require,

        DT_WARN_THROWS_WITH    = is_throws_with | is_warn,
        DT_CHECK_THROWS_WITH   = is_throws_with | is_check,
        DT_REQUIRE_THROWS_WITH = is_throws_with | is_require,
        
        DT_WARN_THROWS_WITH_AS    = is_throws_with | is_throws_as | is_warn,
        DT_CHECK_THROWS_WITH_AS   = is_throws_with | is_throws_as | is_check,
        DT_REQUIRE_THROWS_WITH_AS = is_throws_with | is_throws_as | is_require,

        DT_WARN_NOTHROW    = is_nothrow | is_warn,
        DT_CHECK_NOTHROW   = is_nothrow | is_check,
        DT_REQUIRE_NOTHROW = is_nothrow | is_require,

        DT_WARN_EQ    = is_normal | is_eq | is_warn,
        DT_CHECK_EQ   = is_normal | is_eq | is_check,
        DT_REQUIRE_EQ = is_normal | is_eq | is_require,

        DT_WARN_NE    = is_normal | is_ne | is_warn,
        DT_CHECK_NE   = is_normal | is_ne | is_check,
        DT_REQUIRE_NE = is_normal | is_ne | is_require,

        DT_WARN_GT    = is_normal | is_gt | is_warn,
        DT_CHECK_GT   = is_normal | is_gt | is_check,
        DT_REQUIRE_GT = is_normal | is_gt | is_require,

        DT_WARN_LT    = is_normal | is_lt | is_warn,
        DT_CHECK_LT   = is_normal | is_lt | is_check,
        DT_REQUIRE_LT = is_normal | is_lt | is_require,

        DT_WARN_GE    = is_normal | is_ge | is_warn,
        DT_CHECK_GE   = is_normal | is_ge | is_check,
        DT_REQUIRE_GE = is_normal | is_ge | is_require,

        DT_WARN_LE    = is_normal | is_le | is_warn,
        DT_CHECK_LE   = is_normal | is_le | is_check,
        DT_REQUIRE_LE = is_normal | is_le | is_require,

        DT_WARN_UNARY    = is_normal | is_unary | is_warn,
        DT_CHECK_UNARY   = is_normal | is_unary | is_check,
        DT_REQUIRE_UNARY = is_normal | is_unary | is_require,

        DT_WARN_UNARY_FALSE    = is_normal | is_false | is_unary | is_warn,
        DT_CHECK_UNARY_FALSE   = is_normal | is_false | is_unary | is_check,
        DT_REQUIRE_UNARY_FALSE = is_normal | is_false | is_unary | is_require,
    };
} // namespace assertType

DOCTEST_INTERFACE const char* assertString(assertType::Enum at);
DOCTEST_INTERFACE const char* failureString(assertType::Enum at);
DOCTEST_INTERFACE const char* skipPathFromFilename(const char* file);

struct DOCTEST_INTERFACE TestCaseData
{
    String      m_file;       // the file in which the test was registered (using String - see #350)
    unsigned    m_line;       // the line where the test was registered
    const char* m_name;       // name of the test case
    const char* m_test_suite; // the test suite in which the test was added
    const char* m_description;
    bool        m_skip;
    bool        m_no_breaks;
    bool        m_no_output;
    bool        m_may_fail;
    bool        m_should_fail;
    int         m_expected_failures;
    double      m_timeout;
};

struct DOCTEST_INTERFACE AssertData
{
    // common - for all asserts
    const TestCaseData* m_test_case;
    assertType::Enum    m_at;
    const char*         m_file;
    int                 m_line;
    const char*         m_expr;
    bool                m_failed;

    // exception-related - for all asserts
    bool   m_threw;
    String m_exception;

    // for normal asserts
    String m_decomp;

    // for specific exception-related asserts
    bool        m_threw_as;
    const char* m_exception_type;
    const char* m_exception_string;
};

struct DOCTEST_INTERFACE MessageData
{
    String           m_string;
    const char*      m_file;
    int              m_line;
    assertType::Enum m_severity;
};

struct DOCTEST_INTERFACE SubcaseSignature
{
    String      m_name;
    const char* m_file;
    int         m_line;

    bool operator<(const SubcaseSignature& other) const;
};

struct DOCTEST_INTERFACE IContextScope
{
    IContextScope();
    virtual ~IContextScope();
    virtual void stringify(std::ostream*) const = 0;
};

namespace detail {
    struct DOCTEST_INTERFACE TestCase;
} // namespace detail

struct ContextOptions //!OCLINT too many fields
{
    std::ostream* cout = nullptr; // stdout stream
    String        binary_name;    // the test binary name

    const detail::TestCase* currentTest = nullptr;

    // == parameters from the command line
    String   out;       // output filename
    String   order_by;  // how tests should be ordered
    unsigned rand_seed; // the seed for rand ordering

    unsigned first; // the first (matching) test to be executed
    unsigned last;  // the last (matching) test to be executed

    int abort_after;           // stop tests after this many failed assertions
    int subcase_filter_levels; // apply the subcase filters for the first N levels

    bool success;              // include successful assertions in output
    bool case_sensitive;       // if filtering should be case sensitive
    bool exit;                 // if the program should be exited after the tests are ran/whatever
    bool duration;             // print the time duration of each test case
    bool minimal;              // minimal console output (only test failures)
    bool quiet;                // no console output
    bool no_throw;             // to skip exceptions-related assertion macros
    bool no_exitcode;          // if the framework should return 0 as the exitcode
    bool no_run;               // to not run the tests at all (can be done with an "*" exclude)
    bool no_intro;             // to not print the intro of the framework
    bool no_version;           // to not print the version of the framework
    bool no_colors;            // if output to the console should be colorized
    bool force_colors;         // forces the use of colors even when a tty cannot be detected
    bool no_breaks;            // to not break into the debugger
    bool no_skip;              // don't skip test cases which are marked to be skipped
    bool gnu_file_line;        // if line numbers should be surrounded with :x: and not (x):
    bool no_path_in_filenames; // if the path to files should be removed from the output
    bool no_line_numbers;      // if source code line numbers should be omitted from the output
    bool no_debug_output;      // no output in the debug console when a debugger is attached
    bool no_skipped_summary;   // don't print "skipped" in the summary !!! UNDOCUMENTED !!!
    bool no_time_in_output;    // omit any time/timestamps from output !!! UNDOCUMENTED !!!

    bool help;             // to print the help
    bool version;          // to print the version
    bool count;            // if only the count of matching tests is to be retrieved
    bool list_test_cases;  // to list all tests matching the filters
    bool list_test_suites; // to list all suites matching the filters
    bool list_reporters;   // lists all registered reporters
};

namespace detail {
    template <bool CONDITION, typename TYPE = void>
    struct enable_if
    {};

    template <typename TYPE>
    struct enable_if<true, TYPE>
    { typedef TYPE type; };

    // clang-format off
    template<class T> struct remove_reference      { typedef T type; };
    template<class T> struct remove_reference<T&>  { typedef T type; };
    template<class T> struct remove_reference<T&&> { typedef T type; };

    template<typename T, typename U = T&&> U declval(int); 

    template<typename T> T declval(long); 

    template<typename T> auto declval() DOCTEST_NOEXCEPT -> decltype(declval<T>(0)) ;

    template<class T> struct is_lvalue_reference { const static bool value=false; };
    template<class T> struct is_lvalue_reference<T&> { const static bool value=true; };

    template<class T> struct is_rvalue_reference { const static bool value=false; };
    template<class T> struct is_rvalue_reference<T&&> { const static bool value=true; };

    template <class T>
    inline T&& forward(typename remove_reference<T>::type& t) DOCTEST_NOEXCEPT
    {
        return static_cast<T&&>(t);
    }

    template <class T>
    inline T&& forward(typename remove_reference<T>::type&& t) DOCTEST_NOEXCEPT
    {
        static_assert(!is_lvalue_reference<T>::value,
                        "Can not forward an rvalue as an lvalue.");
        return static_cast<T&&>(t);
    }

    template<class T> struct remove_const          { typedef T type; };
    template<class T> struct remove_const<const T> { typedef T type; };
#ifdef DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
    template<class T> struct is_enum : public std::is_enum<T> {};
    template<class T> struct underlying_type : public std::underlying_type<T> {};
#else
    // Use compiler intrinsics
    template<class T> struct is_enum { DOCTEST_CONSTEXPR static bool value = __is_enum(T); };
    template<class T> struct underlying_type { typedef __underlying_type(T) type; };
#endif
    // clang-format on

    template <typename T>
    struct deferred_false
    // cppcheck-suppress unusedStructMember
    { static const bool value = false; };

    namespace has_insertion_operator_impl {
        std::ostream &os();
        template<class T>
        DOCTEST_REF_WRAP(T) val();

        template<class, class = void>
        struct check {
            static DOCTEST_CONSTEXPR bool value = false;
        };

        template<class T>
        struct check<T, decltype(os() << val<T>(), void())> {
            static DOCTEST_CONSTEXPR bool value = true;
        };
    } // namespace has_insertion_operator_impl

    template<class T>
    using has_insertion_operator = has_insertion_operator_impl::check<const T>;

    DOCTEST_INTERFACE std::ostream& tlssPush();
    DOCTEST_INTERFACE String tlssPop();


    template <bool C>
    struct StringMakerBase
    {
        template <typename T>
        static String convert(const DOCTEST_REF_WRAP(T)) {
            return "{?}";
        }
    };

    // Vector<int> and various type other than pointer or array.
    template<typename T>
    struct filldata
    {
        static void fill(std::ostream& stream, const  T &in) {
          stream << in;
        }
    };

    template<typename T,unsigned long N>
    struct filldata<T[N]>
    {
        static void fill(std::ostream& stream, const T (&in)[N]) {
            for (unsigned long i = 0; i < N; i++) {
                stream << in[i];
            }
        }
    };

    // Specialized since we don't want the terminating null byte!
    template<unsigned long N>
    struct filldata<const char[N]>
    {
        static void fill(std::ostream& stream, const char(&in)[N]) {
            stream << in;
        }
    };

    template<typename T>
    void filloss(std::ostream& stream, const T& in){
	    filldata<T>::fill(stream, in);
    }

    template<typename T,unsigned long N>
    void filloss(std::ostream& stream, const T (&in)[N]) {
        // T[N], T(&)[N], T(&&)[N] have same behaviour.
        // Hence remove reference.
        filldata<typename remove_reference<decltype(in)>::type>::fill(stream, in);
    }

    template <>
    struct StringMakerBase<true>
    {
        template <typename T>
        static String convert(const DOCTEST_REF_WRAP(T) in) {
            /* When parameter "in" is a null terminated const char* it works.
	         * When parameter "in" is a T arr[N] without '\0' we can fill the
             * stringstream with N objects (T=char).If in is char pointer *
             * without '\0' , it would cause segfault
	         * stepping over unaccessible memory.
             */

            std::ostream& stream = tlssPush();
            filloss(stream, in);
            return tlssPop();
        }
    };

    DOCTEST_INTERFACE String rawMemoryToString(const void* object, unsigned size);

    template <typename T>
    String rawMemoryToString(const DOCTEST_REF_WRAP(T) object) {
        return rawMemoryToString(&object, sizeof(object));
    }

    template <typename T>
    const char* type_to_string() {
        return "<>";
    }
} // namespace detail

template <typename T>
struct StringMaker : public detail::StringMakerBase<detail::has_insertion_operator<T>::value>
{};

template <typename T>
struct StringMaker<T*>
{
    template <typename U>
    static String convert(U* p) {
        if(p)
            return detail::rawMemoryToString(p);
        return "NULL";
    }
};

template <typename R, typename C>
struct StringMaker<R C::*>
{
    static String convert(R C::*p) {
        if(p)
            return detail::rawMemoryToString(p);
        return "NULL";
    }
};

template <typename T, typename detail::enable_if<!detail::is_enum<T>::value, bool>::type = true>
String toString(const DOCTEST_REF_WRAP(T) value) {
    return StringMaker<T>::convert(value);
}

#ifdef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
DOCTEST_INTERFACE String toString(char* in);
DOCTEST_INTERFACE String toString(const char* in);
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
DOCTEST_INTERFACE String toString(bool in);
DOCTEST_INTERFACE String toString(float in);
DOCTEST_INTERFACE String toString(double in);
DOCTEST_INTERFACE String toString(double long in);

DOCTEST_INTERFACE String toString(char in);
DOCTEST_INTERFACE String toString(char signed in);
DOCTEST_INTERFACE String toString(char unsigned in);
DOCTEST_INTERFACE String toString(int short in);
DOCTEST_INTERFACE String toString(int short unsigned in);
DOCTEST_INTERFACE String toString(int in);
DOCTEST_INTERFACE String toString(int unsigned in);
DOCTEST_INTERFACE String toString(int long in);
DOCTEST_INTERFACE String toString(int long unsigned in);
DOCTEST_INTERFACE String toString(int long long in);
DOCTEST_INTERFACE String toString(int long long unsigned in);
DOCTEST_INTERFACE String toString(std::nullptr_t in);

template <typename T, typename detail::enable_if<detail::is_enum<T>::value, bool>::type = true>
String toString(const DOCTEST_REF_WRAP(T) value) {
    typedef typename detail::underlying_type<T>::type UT;
    return toString(static_cast<UT>(value));
}

#if DOCTEST_MSVC >= DOCTEST_COMPILER(19, 20, 0)
// see this issue on why this is needed: https://github.com/onqtam/doctest/issues/183
DOCTEST_INTERFACE String toString(const std::string& in);
#endif // VS 2019

class DOCTEST_INTERFACE Approx
{
public:
    explicit Approx(double value);

    Approx operator()(double value) const;

#ifdef DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
    template <typename T>
    explicit Approx(const T& value,
                    typename detail::enable_if<std::is_constructible<double, T>::value>::type* =
                            static_cast<T*>(nullptr)) {
        *this = Approx(static_cast<double>(value));
    }
#endif // DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS

    Approx& epsilon(double newEpsilon);

#ifdef DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
    template <typename T>
    typename detail::enable_if<std::is_constructible<double, T>::value, Approx&>::type epsilon(
            const T& newEpsilon) {
        m_epsilon = static_cast<double>(newEpsilon);
        return *this;
    }
#endif //  DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS

    Approx& scale(double newScale);

#ifdef DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
    template <typename T>
    typename detail::enable_if<std::is_constructible<double, T>::value, Approx&>::type scale(
            const T& newScale) {
        m_scale = static_cast<double>(newScale);
        return *this;
    }
#endif // DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS

    // clang-format off
    DOCTEST_INTERFACE friend bool operator==(double lhs, const Approx & rhs);
    DOCTEST_INTERFACE friend bool operator==(const Approx & lhs, double rhs);
    DOCTEST_INTERFACE friend bool operator!=(double lhs, const Approx & rhs);
    DOCTEST_INTERFACE friend bool operator!=(const Approx & lhs, double rhs);
    DOCTEST_INTERFACE friend bool operator<=(double lhs, const Approx & rhs);
    DOCTEST_INTERFACE friend bool operator<=(const Approx & lhs, double rhs);
    DOCTEST_INTERFACE friend bool operator>=(double lhs, const Approx & rhs);
    DOCTEST_INTERFACE friend bool operator>=(const Approx & lhs, double rhs);
    DOCTEST_INTERFACE friend bool operator< (double lhs, const Approx & rhs);
    DOCTEST_INTERFACE friend bool operator< (const Approx & lhs, double rhs);
    DOCTEST_INTERFACE friend bool operator> (double lhs, const Approx & rhs);
    DOCTEST_INTERFACE friend bool operator> (const Approx & lhs, double rhs);

    DOCTEST_INTERFACE friend String toString(const Approx& in);

#ifdef DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#define DOCTEST_APPROX_PREFIX \
    template <typename T> friend typename detail::enable_if<std::is_constructible<double, T>::value, bool>::type

    DOCTEST_APPROX_PREFIX operator==(const T& lhs, const Approx& rhs) { return operator==(double(lhs), rhs); }
    DOCTEST_APPROX_PREFIX operator==(const Approx& lhs, const T& rhs) { return operator==(rhs, lhs); }
    DOCTEST_APPROX_PREFIX operator!=(const T& lhs, const Approx& rhs) { return !operator==(lhs, rhs); }
    DOCTEST_APPROX_PREFIX operator!=(const Approx& lhs, const T& rhs) { return !operator==(rhs, lhs); }
    DOCTEST_APPROX_PREFIX operator<=(const T& lhs, const Approx& rhs) { return double(lhs) < rhs.m_value || lhs == rhs; }
    DOCTEST_APPROX_PREFIX operator<=(const Approx& lhs, const T& rhs) { return lhs.m_value < double(rhs) || lhs == rhs; }
    DOCTEST_APPROX_PREFIX operator>=(const T& lhs, const Approx& rhs) { return double(lhs) > rhs.m_value || lhs == rhs; }
    DOCTEST_APPROX_PREFIX operator>=(const Approx& lhs, const T& rhs) { return lhs.m_value > double(rhs) || lhs == rhs; }
    DOCTEST_APPROX_PREFIX operator< (const T& lhs, const Approx& rhs) { return double(lhs) < rhs.m_value && lhs != rhs; }
    DOCTEST_APPROX_PREFIX operator< (const Approx& lhs, const T& rhs) { return lhs.m_value < double(rhs) && lhs != rhs; }
    DOCTEST_APPROX_PREFIX operator> (const T& lhs, const Approx& rhs) { return double(lhs) > rhs.m_value && lhs != rhs; }
    DOCTEST_APPROX_PREFIX operator> (const Approx& lhs, const T& rhs) { return lhs.m_value > double(rhs) && lhs != rhs; }
#undef DOCTEST_APPROX_PREFIX
#endif // DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS

    // clang-format on

private:
    double m_epsilon;
    double m_scale;
    double m_value;
};

DOCTEST_INTERFACE String toString(const Approx& in);

DOCTEST_INTERFACE const ContextOptions* getContextOptions();

#if !defined(DOCTEST_CONFIG_DISABLE)

namespace detail {
    // clang-format off
#ifdef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    template<class T>               struct decay_array       { typedef T type; };
    template<class T, unsigned N>   struct decay_array<T[N]> { typedef T* type; };
    template<class T>               struct decay_array<T[]>  { typedef T* type; };

    template<class T>   struct not_char_pointer              { enum { value = 1 }; };
    template<>          struct not_char_pointer<char*>       { enum { value = 0 }; };
    template<>          struct not_char_pointer<const char*> { enum { value = 0 }; };

    template<class T> struct can_use_op : public not_char_pointer<typename decay_array<T>::type> {};
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    // clang-format on

    struct DOCTEST_INTERFACE TestFailureException
    {
    };

    DOCTEST_INTERFACE bool checkIfShouldThrow(assertType::Enum at);

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
    DOCTEST_NORETURN
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    DOCTEST_INTERFACE void throwException();

    struct DOCTEST_INTERFACE Subcase
    {
        SubcaseSignature m_signature;
        bool             m_entered = false;

        Subcase(const String& name, const char* file, int line);
        ~Subcase();

        operator bool() const;
    };

    template <typename L, typename R>
    String stringifyBinaryExpr(const DOCTEST_REF_WRAP(L) lhs, const char* op,
                               const DOCTEST_REF_WRAP(R) rhs) {
        // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
        return toString(lhs) + op + toString(rhs);
    }

#if DOCTEST_CLANG && DOCTEST_CLANG < DOCTEST_COMPILER(3, 6, 0)
DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wunused-comparison")
#endif

// This will check if there is any way it could find a operator like member or friend and uses it.
// If not it doesn't find the operator or if the operator at global scope is defined after
// this template, the template won't be instantiated due to SFINAE. Once the template is not
// instantiated it can look for global operator using normal conversions.
#define SFINAE_OP(ret,op) decltype((void)(doctest::detail::declval<L>() op doctest::detail::declval<R>()),ret{})

#define DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(op, op_str, op_macro)                              \
    template <typename R>                                                                          \
    DOCTEST_NOINLINE SFINAE_OP(Result,op) operator op(const R&& rhs) {             \
    bool res = op_macro(doctest::detail::forward<const L>(lhs), doctest::detail::forward<const R>(rhs));                                                             \
        if(m_at & assertType::is_false)                                                            \
            res = !res;                                                                            \
        if(!res || doctest::getContextOptions()->success)                                          \
            return Result(res, stringifyBinaryExpr(lhs, op_str, rhs));                             \
        return Result(res);                                                                        \
    }												   \
    template <typename R ,typename enable_if<  !doctest::detail::is_rvalue_reference<R>::value   , void >::type* = nullptr>                                         \
    DOCTEST_NOINLINE SFINAE_OP(Result,op) operator op(const R& rhs) {             \
    bool res = op_macro(doctest::detail::forward<const L>(lhs), doctest::detail::forward<const R>(rhs));                                                             \
        if(m_at & assertType::is_false)                                                            \
            res = !res;                                                                            \
        if(!res || doctest::getContextOptions()->success)                                          \
            return Result(res, stringifyBinaryExpr(lhs, op_str, rhs));                             \
        return Result(res);                                                                        \
    }


    // more checks could be added - like in Catch:
    // https://github.com/catchorg/Catch2/pull/1480/files
    // https://github.com/catchorg/Catch2/pull/1481/files
#define DOCTEST_FORBIT_EXPRESSION(rt, op)                                                          \
    template <typename R>                                                                          \
    rt& operator op(const R&) {                                                                    \
        static_assert(deferred_false<R>::value,                                                    \
                      "Expression Too Complex Please Rewrite As Binary Comparison!");              \
        return *this;                                                                              \
    }

    struct DOCTEST_INTERFACE Result
    {
        bool   m_passed;
        String m_decomp;

        Result() = default;
        Result(bool passed, const String& decomposition = String());

        // forbidding some expressions based on this table: https://en.cppreference.com/w/cpp/language/operator_precedence
        DOCTEST_FORBIT_EXPRESSION(Result, &)
        DOCTEST_FORBIT_EXPRESSION(Result, ^)
        DOCTEST_FORBIT_EXPRESSION(Result, |)
        DOCTEST_FORBIT_EXPRESSION(Result, &&)
        DOCTEST_FORBIT_EXPRESSION(Result, ||)
        DOCTEST_FORBIT_EXPRESSION(Result, ==)
        DOCTEST_FORBIT_EXPRESSION(Result, !=)
        DOCTEST_FORBIT_EXPRESSION(Result, <)
        DOCTEST_FORBIT_EXPRESSION(Result, >)
        DOCTEST_FORBIT_EXPRESSION(Result, <=)
        DOCTEST_FORBIT_EXPRESSION(Result, >=)
        DOCTEST_FORBIT_EXPRESSION(Result, =)
        DOCTEST_FORBIT_EXPRESSION(Result, +=)
        DOCTEST_FORBIT_EXPRESSION(Result, -=)
        DOCTEST_FORBIT_EXPRESSION(Result, *=)
        DOCTEST_FORBIT_EXPRESSION(Result, /=)
        DOCTEST_FORBIT_EXPRESSION(Result, %=)
        DOCTEST_FORBIT_EXPRESSION(Result, <<=)
        DOCTEST_FORBIT_EXPRESSION(Result, >>=)
        DOCTEST_FORBIT_EXPRESSION(Result, &=)
        DOCTEST_FORBIT_EXPRESSION(Result, ^=)
        DOCTEST_FORBIT_EXPRESSION(Result, |=)
    };

#ifndef DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

    DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wsign-conversion")
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wsign-compare")
    //DOCTEST_CLANG_SUPPRESS_WARNING("-Wdouble-promotion")
    //DOCTEST_CLANG_SUPPRESS_WARNING("-Wconversion")
    //DOCTEST_CLANG_SUPPRESS_WARNING("-Wfloat-equal")

    DOCTEST_GCC_SUPPRESS_WARNING_PUSH
    DOCTEST_GCC_SUPPRESS_WARNING("-Wsign-conversion")
    DOCTEST_GCC_SUPPRESS_WARNING("-Wsign-compare")
    //DOCTEST_GCC_SUPPRESS_WARNING("-Wdouble-promotion")
    //DOCTEST_GCC_SUPPRESS_WARNING("-Wconversion")
    //DOCTEST_GCC_SUPPRESS_WARNING("-Wfloat-equal")

    DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
    // https://stackoverflow.com/questions/39479163 what's the difference between 4018 and 4389
    DOCTEST_MSVC_SUPPRESS_WARNING(4388) // signed/unsigned mismatch
    DOCTEST_MSVC_SUPPRESS_WARNING(4389) // 'operator' : signed/unsigned mismatch
    DOCTEST_MSVC_SUPPRESS_WARNING(4018) // 'expression' : signed/unsigned mismatch
    //DOCTEST_MSVC_SUPPRESS_WARNING(4805) // 'operation' : unsafe mix of type 'type' and type 'type' in operation

#endif // DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

    // clang-format off
#ifndef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_COMPARISON_RETURN_TYPE bool
#else // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_COMPARISON_RETURN_TYPE typename enable_if<can_use_op<L>::value || can_use_op<R>::value, bool>::type
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    inline bool eq(const char* lhs, const char* rhs) { return String(lhs) == String(rhs); }
    inline bool ne(const char* lhs, const char* rhs) { return String(lhs) != String(rhs); }
    inline bool lt(const char* lhs, const char* rhs) { return String(lhs) <  String(rhs); }
    inline bool gt(const char* lhs, const char* rhs) { return String(lhs) >  String(rhs); }
    inline bool le(const char* lhs, const char* rhs) { return String(lhs) <= String(rhs); }
    inline bool ge(const char* lhs, const char* rhs) { return String(lhs) >= String(rhs); }
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    // clang-format on

#define DOCTEST_RELATIONAL_OP(name, op)                                                            \
    template <typename L, typename R>                                                              \
    DOCTEST_COMPARISON_RETURN_TYPE name(const DOCTEST_REF_WRAP(L) lhs,                             \
                                        const DOCTEST_REF_WRAP(R) rhs) {                           \
        return lhs op rhs;                                                                         \
    }

    DOCTEST_RELATIONAL_OP(eq, ==)
    DOCTEST_RELATIONAL_OP(ne, !=)
    DOCTEST_RELATIONAL_OP(lt, <)
    DOCTEST_RELATIONAL_OP(gt, >)
    DOCTEST_RELATIONAL_OP(le, <=)
    DOCTEST_RELATIONAL_OP(ge, >=)

#ifndef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_CMP_EQ(l, r) l == r
#define DOCTEST_CMP_NE(l, r) l != r
#define DOCTEST_CMP_GT(l, r) l > r
#define DOCTEST_CMP_LT(l, r) l < r
#define DOCTEST_CMP_GE(l, r) l >= r
#define DOCTEST_CMP_LE(l, r) l <= r
#else // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_CMP_EQ(l, r) eq(l, r)
#define DOCTEST_CMP_NE(l, r) ne(l, r)
#define DOCTEST_CMP_GT(l, r) gt(l, r)
#define DOCTEST_CMP_LT(l, r) lt(l, r)
#define DOCTEST_CMP_GE(l, r) ge(l, r)
#define DOCTEST_CMP_LE(l, r) le(l, r)
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING

    template <typename L>
    // cppcheck-suppress copyCtorAndEqOperator
    struct Expression_lhs
    {
        L                lhs;
        assertType::Enum m_at;

        explicit Expression_lhs(L&& in, assertType::Enum at)
                : lhs(doctest::detail::forward<L>(in))
                , m_at(at) {}

        DOCTEST_NOINLINE operator Result() {
// this is needed only for MSVC 2015:
// https://ci.appveyor.com/project/onqtam/doctest/builds/38181202
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4800) // 'int': forcing value to bool
            bool res = static_cast<bool>(lhs);
DOCTEST_MSVC_SUPPRESS_WARNING_POP
            if(m_at & assertType::is_false) //!OCLINT bitwise operator in conditional
                res = !res;

            if(!res || getContextOptions()->success)
                return Result(res, toString(lhs));
            return Result(res);
        }

	/* This is required for user-defined conversions from Expression_lhs to L */
	//operator L() const { return lhs; }
	operator L() const { return lhs; }

        // clang-format off
        DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(==, " == ", DOCTEST_CMP_EQ) //!OCLINT bitwise operator in conditional
        DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(!=, " != ", DOCTEST_CMP_NE) //!OCLINT bitwise operator in conditional
        DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(>,  " >  ", DOCTEST_CMP_GT) //!OCLINT bitwise operator in conditional
        DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(<,  " <  ", DOCTEST_CMP_LT) //!OCLINT bitwise operator in conditional
        DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(>=, " >= ", DOCTEST_CMP_GE) //!OCLINT bitwise operator in conditional
        DOCTEST_DO_BINARY_EXPRESSION_COMPARISON(<=, " <= ", DOCTEST_CMP_LE) //!OCLINT bitwise operator in conditional
        // clang-format on

        // forbidding some expressions based on this table: https://en.cppreference.com/w/cpp/language/operator_precedence
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, &)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, ^)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, |)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, &&)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, ||)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, =)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, +=)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, -=)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, *=)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, /=)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, %=)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, <<=)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, >>=)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, &=)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, ^=)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, |=)
        // these 2 are unfortunate because they should be allowed - they have higher precedence over the comparisons, but the
        // ExpressionDecomposer class uses the left shift operator to capture the left operand of the binary expression...
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, <<)
        DOCTEST_FORBIT_EXPRESSION(Expression_lhs, >>)
    };

#ifndef DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

    DOCTEST_CLANG_SUPPRESS_WARNING_POP
    DOCTEST_MSVC_SUPPRESS_WARNING_POP
    DOCTEST_GCC_SUPPRESS_WARNING_POP

#endif // DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION

#if DOCTEST_CLANG && DOCTEST_CLANG < DOCTEST_COMPILER(3, 6, 0)
DOCTEST_CLANG_SUPPRESS_WARNING_POP
#endif

    struct DOCTEST_INTERFACE ExpressionDecomposer
    {
        assertType::Enum m_at;

        ExpressionDecomposer(assertType::Enum at);

        // The right operator for capturing expressions is "<=" instead of "<<" (based on the operator precedence table)
        // but then there will be warnings from GCC about "-Wparentheses" and since "_Pragma()" is problematic this will stay for now...
        // https://github.com/catchorg/Catch2/issues/870
        // https://github.com/catchorg/Catch2/issues/565
        template <typename L>
	Expression_lhs<const L> operator<<(const L &&operand) {
            return Expression_lhs<const L>(doctest::detail::forward<const L>(operand), m_at);
        }

        template <typename L,typename enable_if<!doctest::detail::is_rvalue_reference<L>::value,void >::type* = nullptr>
	Expression_lhs<const L&> operator<<(const L &operand) {
            return Expression_lhs<const L&>(operand, m_at);
        }
    };

    struct DOCTEST_INTERFACE TestSuite
    {
        const char* m_test_suite = nullptr;
        const char* m_description = nullptr;
        bool        m_skip = false;
        bool        m_no_breaks = false;
        bool        m_no_output = false;
        bool        m_may_fail = false;
        bool        m_should_fail = false;
        int         m_expected_failures = 0;
        double      m_timeout = 0;

        TestSuite& operator*(const char* in);

        template <typename T>
        TestSuite& operator*(const T& in) {
            in.fill(*this);
            return *this;
        }
    };

    typedef void (*funcType)();

    struct DOCTEST_INTERFACE TestCase : public TestCaseData
    {
        funcType m_test; // a function pointer to the test case

        const char* m_type; // for templated test cases - gets appended to the real name
        int m_template_id; // an ID used to distinguish between the different versions of a templated test case
        String m_full_name; // contains the name (only for templated test cases!) + the template type

        TestCase(funcType test, const char* file, unsigned line, const TestSuite& test_suite,
                 const char* type = "", int template_id = -1);

        TestCase(const TestCase& other);

        DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(26434) // hides a non-virtual function
        TestCase& operator=(const TestCase& other);
        DOCTEST_MSVC_SUPPRESS_WARNING_POP

        TestCase& operator*(const char* in);

        template <typename T>
        TestCase& operator*(const T& in) {
            in.fill(*this);
            return *this;
        }

        bool operator<(const TestCase& other) const;
    };

    // forward declarations of functions used by the macros
    DOCTEST_INTERFACE int  regTest(const TestCase& tc);
    DOCTEST_INTERFACE int  setTestSuite(const TestSuite& ts);
    DOCTEST_INTERFACE bool isDebuggerActive();

    template<typename T>
    int instantiationHelper(const T&) { return 0; }

    namespace binaryAssertComparison {
        enum Enum
        {
            eq = 0,
            ne,
            gt,
            lt,
            ge,
            le
        };
    } // namespace binaryAssertComparison

    // clang-format off
    template <int, class L, class R> struct RelationalComparator     { bool operator()(const DOCTEST_REF_WRAP(L),     const DOCTEST_REF_WRAP(R)    ) const { return false;        } };

#define DOCTEST_BINARY_RELATIONAL_OP(n, op) \
    template <class L, class R> struct RelationalComparator<n, L, R> { bool operator()(const DOCTEST_REF_WRAP(L) lhs, const DOCTEST_REF_WRAP(R) rhs) const { return op(lhs, rhs); } };
    // clang-format on

    DOCTEST_BINARY_RELATIONAL_OP(0, doctest::detail::eq)
    DOCTEST_BINARY_RELATIONAL_OP(1, doctest::detail::ne)
    DOCTEST_BINARY_RELATIONAL_OP(2, doctest::detail::gt)
    DOCTEST_BINARY_RELATIONAL_OP(3, doctest::detail::lt)
    DOCTEST_BINARY_RELATIONAL_OP(4, doctest::detail::ge)
    DOCTEST_BINARY_RELATIONAL_OP(5, doctest::detail::le)

    struct DOCTEST_INTERFACE ResultBuilder : public AssertData
    {
        ResultBuilder(assertType::Enum at, const char* file, int line, const char* expr,
                      const char* exception_type = "", const char* exception_string = "");

        void setResult(const Result& res);

        template <int comparison, typename L, typename R>
        DOCTEST_NOINLINE void binary_assert(const DOCTEST_REF_WRAP(L) lhs,
                                            const DOCTEST_REF_WRAP(R) rhs) {
            m_failed = !RelationalComparator<comparison, L, R>()(lhs, rhs);
            if(m_failed || getContextOptions()->success)
                m_decomp = stringifyBinaryExpr(lhs, ", ", rhs);
        }

        template <typename L>
        DOCTEST_NOINLINE void unary_assert(const DOCTEST_REF_WRAP(L) val) {
            m_failed = !val;

            if(m_at & assertType::is_false) //!OCLINT bitwise operator in conditional
                m_failed = !m_failed;

            if(m_failed || getContextOptions()->success)
                m_decomp = toString(val);
        }

        void translateException();

        bool log();
        void react() const;
    };

    namespace assertAction {
        enum Enum
        {
            nothing     = 0,
            dbgbreak    = 1,
            shouldthrow = 2
        };
    } // namespace assertAction

    DOCTEST_INTERFACE void failed_out_of_a_testing_context(const AssertData& ad);

    DOCTEST_INTERFACE void decomp_assert(assertType::Enum at, const char* file, int line,
                                         const char* expr, Result result);

#define DOCTEST_ASSERT_OUT_OF_TESTS(decomp)                                                        \
    do {                                                                                           \
        if(!is_running_in_test) {                                                                  \
            if(failed) {                                                                           \
                ResultBuilder rb(at, file, line, expr);                                            \
                rb.m_failed = failed;                                                              \
                rb.m_decomp = decomp;                                                              \
                failed_out_of_a_testing_context(rb);                                               \
                if(isDebuggerActive() && !getContextOptions()->no_breaks)                          \
                    DOCTEST_BREAK_INTO_DEBUGGER();                                                 \
                if(checkIfShouldThrow(at))                                                         \
                    throwException();                                                              \
            }                                                                                      \
            return;                                                                                \
        }                                                                                          \
    } while(false)

#define DOCTEST_ASSERT_IN_TESTS(decomp)                                                            \
    ResultBuilder rb(at, file, line, expr);                                                        \
    rb.m_failed = failed;                                                                          \
    if(rb.m_failed || getContextOptions()->success)                                                \
        rb.m_decomp = decomp;                                                                      \
    if(rb.log())                                                                                   \
        DOCTEST_BREAK_INTO_DEBUGGER();                                                             \
    if(rb.m_failed && checkIfShouldThrow(at))                                                      \
    throwException()

    template <int comparison, typename L, typename R>
    DOCTEST_NOINLINE void binary_assert(assertType::Enum at, const char* file, int line,
                                        const char* expr, const DOCTEST_REF_WRAP(L) lhs,
                                        const DOCTEST_REF_WRAP(R) rhs) {
        bool failed = !RelationalComparator<comparison, L, R>()(lhs, rhs);

        // ###################################################################################
        // IF THE DEBUGGER BREAKS HERE - GO 1 LEVEL UP IN THE CALLSTACK FOR THE FAILING ASSERT
        // THIS IS THE EFFECT OF HAVING 'DOCTEST_CONFIG_SUPER_FAST_ASSERTS' DEFINED
        // ###################################################################################
        DOCTEST_ASSERT_OUT_OF_TESTS(stringifyBinaryExpr(lhs, ", ", rhs));
        DOCTEST_ASSERT_IN_TESTS(stringifyBinaryExpr(lhs, ", ", rhs));
    }

    template <typename L>
    DOCTEST_NOINLINE void unary_assert(assertType::Enum at, const char* file, int line,
                                       const char* expr, const DOCTEST_REF_WRAP(L) val) {
        bool failed = !val;

        if(at & assertType::is_false) //!OCLINT bitwise operator in conditional
            failed = !failed;

        // ###################################################################################
        // IF THE DEBUGGER BREAKS HERE - GO 1 LEVEL UP IN THE CALLSTACK FOR THE FAILING ASSERT
        // THIS IS THE EFFECT OF HAVING 'DOCTEST_CONFIG_SUPER_FAST_ASSERTS' DEFINED
        // ###################################################################################
        DOCTEST_ASSERT_OUT_OF_TESTS(toString(val));
        DOCTEST_ASSERT_IN_TESTS(toString(val));
    }

    struct DOCTEST_INTERFACE IExceptionTranslator
    {
        IExceptionTranslator();
        virtual ~IExceptionTranslator();
        virtual bool translate(String&) const = 0;
    };

    template <typename T>
    class ExceptionTranslator : public IExceptionTranslator //!OCLINT destructor of virtual class
    {
    public:
        explicit ExceptionTranslator(String (*translateFunction)(T))
                : m_translateFunction(translateFunction) {}

        bool translate(String& res) const override {
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
            try {
                throw; // lgtm [cpp/rethrow-no-exception]
                // cppcheck-suppress catchExceptionByValue
            } catch(T ex) {                    // NOLINT
                res = m_translateFunction(ex); //!OCLINT parameter reassignment
                return true;
            } catch(...) {}         //!OCLINT -  empty catch statement
#endif                              // DOCTEST_CONFIG_NO_EXCEPTIONS
            static_cast<void>(res); // to silence -Wunused-parameter
            return false;
        }

    private:
        String (*m_translateFunction)(T);
    };

    DOCTEST_INTERFACE void registerExceptionTranslatorImpl(const IExceptionTranslator* et);

    template <bool C>
    struct StringStreamBase
    {
        template <typename T>
        static void convert(std::ostream& s, const T& in) {
            s << toString(in);
        }

        // always treat char* as a string in this context - no matter
        // if DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING is defined
        static void convert(std::ostream& s, const char* in) { s << String(in); }
    };

    template <>
    struct StringStreamBase<true>
    {
        template <typename T>
        static void convert(std::ostream& s, const T& in) {
            s << in;
        }
    };

    template <typename T>
    struct StringStream : public StringStreamBase<has_insertion_operator<T>::value>
    {};

    template <typename T>
    void toStream(std::ostream& s, const T& value) {
        StringStream<T>::convert(s, value);
    }

#ifdef DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    DOCTEST_INTERFACE void toStream(std::ostream* s, char* in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, const char* in);
#endif // DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
    DOCTEST_INTERFACE void toStream(std::ostream* s, bool in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, float in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, double in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, double long in);

    DOCTEST_INTERFACE void toStream(std::ostream* s, char in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, char signed in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, char unsigned in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, int short in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, int short unsigned in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, int in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, int unsigned in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, int long in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, int long unsigned in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, int long long in);
    DOCTEST_INTERFACE void toStream(std::ostream* s, int long long unsigned in);

    // ContextScope base class used to allow implementing methods of ContextScope 
    // that don't depend on the template parameter in doctest.cpp.
    class DOCTEST_INTERFACE ContextScopeBase : public IContextScope {
    protected:
        ContextScopeBase();
        ContextScopeBase(ContextScopeBase&& other);

        void destroy();
        bool need_to_destroy{true};
    };

    template <typename L> class ContextScope : public ContextScopeBase
    {
        const L lambda_;

    public:
        explicit ContextScope(const L &lambda) : lambda_(lambda) {}

        ContextScope(ContextScope &&other) : ContextScopeBase(static_cast<ContextScopeBase&&>(other)), lambda_(other.lambda_) {}

        void stringify(std::ostream* s) const override { lambda_(s); }

        ~ContextScope() override {
            if (need_to_destroy) {
                destroy();
            }
        }
    };

    struct DOCTEST_INTERFACE MessageBuilder : public MessageData
    {
        std::ostream* m_stream;
        bool          logged = false;

        MessageBuilder(const char* file, int line, assertType::Enum severity);
        MessageBuilder() = delete;
        ~MessageBuilder();

        // the preferred way of chaining parameters for stringification
        template <typename T>
        MessageBuilder& operator,(const T& in) {
            toStream(*m_stream, in);
            return *this;
        }

        // kept here just for backwards-compatibility - the comma operator should be preferred now
        template <typename T>
        MessageBuilder& operator<<(const T& in) { return this->operator,(in); }

        // the `,` operator has the lowest operator precedence - if `<<` is used by the user then
        // the `,` operator will be called last which is not what we want and thus the `*` operator
        // is used first (has higher operator precedence compared to `<<`) so that we guarantee that
        // an operator of the MessageBuilder class is called first before the rest of the parameters
        template <typename T>
        MessageBuilder& operator*(const T& in) { return this->operator,(in); }

        bool log();
        void react();
    };
    
    template <typename L>
    ContextScope<L> MakeContextScope(const L &lambda) {
        return ContextScope<L>(lambda);
    }
} // namespace detail

#define DOCTEST_DEFINE_DECORATOR(name, type, def)                                                  \
    struct name                                                                                    \
    {                                                                                              \
        type data;                                                                                 \
        name(type in = def)                                                                        \
                : data(in) {}                                                                      \
        void fill(detail::TestCase& state) const { state.DOCTEST_CAT(m_, name) = data; }           \
        void fill(detail::TestSuite& state) const { state.DOCTEST_CAT(m_, name) = data; }          \
    }

DOCTEST_DEFINE_DECORATOR(test_suite, const char*, "");
DOCTEST_DEFINE_DECORATOR(description, const char*, "");
DOCTEST_DEFINE_DECORATOR(skip, bool, true);
DOCTEST_DEFINE_DECORATOR(no_breaks, bool, true);
DOCTEST_DEFINE_DECORATOR(no_output, bool, true);
DOCTEST_DEFINE_DECORATOR(timeout, double, 0);
DOCTEST_DEFINE_DECORATOR(may_fail, bool, true);
DOCTEST_DEFINE_DECORATOR(should_fail, bool, true);
DOCTEST_DEFINE_DECORATOR(expected_failures, int, 0);

template <typename T>
int registerExceptionTranslator(String (*translateFunction)(T)) {
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wexit-time-destructors")
    static detail::ExceptionTranslator<T> exceptionTranslator(translateFunction);
    DOCTEST_CLANG_SUPPRESS_WARNING_POP
    detail::registerExceptionTranslatorImpl(&exceptionTranslator);
    return 0;
}

} // namespace doctest

// in a separate namespace outside of doctest because the DOCTEST_TEST_SUITE macro
// introduces an anonymous namespace in which getCurrentTestSuite gets overridden
namespace doctest_detail_test_suite_ns {
DOCTEST_INTERFACE doctest::detail::TestSuite& getCurrentTestSuite();
} // namespace doctest_detail_test_suite_ns

namespace doctest {
#else  // DOCTEST_CONFIG_DISABLE
template <typename T>
int registerExceptionTranslator(String (*)(T)) {
    return 0;
}
#endif // DOCTEST_CONFIG_DISABLE

namespace detail {
    typedef void (*assert_handler)(const AssertData&);
    struct ContextState;
} // namespace detail

class DOCTEST_INTERFACE Context
{
    detail::ContextState* p;

    void parseArgs(int argc, const char* const* argv, bool withDefaults = false);

public:
    explicit Context(int argc = 0, const char* const* argv = nullptr);

    ~Context();

    void applyCommandLine(int argc, const char* const* argv);

    void addFilter(const char* filter, const char* value);
    void clearFilters();
    void setOption(const char* option, bool value);
    void setOption(const char* option, int value);
    void setOption(const char* option, const char* value);

    bool shouldExit();

    void setAsDefaultForAssertsOutOfTestCases();

    void setAssertHandler(detail::assert_handler ah);

    void setCout(std::ostream* out);

    int run();
};

namespace TestCaseFailureReason {
    enum Enum
    {
        None                     = 0,
        AssertFailure            = 1,   // an assertion has failed in the test case
        Exception                = 2,   // test case threw an exception
        Crash                    = 4,   // a crash...
        TooManyFailedAsserts     = 8,   // the abort-after option
        Timeout                  = 16,  // see the timeout decorator
        ShouldHaveFailedButDidnt = 32,  // see the should_fail decorator
        ShouldHaveFailedAndDid   = 64,  // see the should_fail decorator
        DidntFailExactlyNumTimes = 128, // see the expected_failures decorator
        FailedExactlyNumTimes    = 256, // see the expected_failures decorator
        CouldHaveFailedAndDid    = 512  // see the may_fail decorator
    };
} // namespace TestCaseFailureReason

struct DOCTEST_INTERFACE CurrentTestCaseStats
{
    int    numAssertsCurrentTest;
    int    numAssertsFailedCurrentTest;
    double seconds;
    int    failure_flags; // use TestCaseFailureReason::Enum
    bool   testCaseSuccess;
};

struct DOCTEST_INTERFACE TestCaseException
{
    String error_string;
    bool   is_crash;
};

struct DOCTEST_INTERFACE TestRunStats
{
    unsigned numTestCases;
    unsigned numTestCasesPassingFilters;
    unsigned numTestSuitesPassingFilters;
    unsigned numTestCasesFailed;
    int      numAsserts;
    int      numAssertsFailed;
};

struct QueryData
{
    const TestRunStats*  run_stats = nullptr;
    const TestCaseData** data      = nullptr;
    unsigned             num_data  = 0;
};

struct DOCTEST_INTERFACE IReporter
{
    // The constructor has to accept "const ContextOptions&" as a single argument
    // which has most of the options for the run + a pointer to the stdout stream
    // Reporter(const ContextOptions& in)

    // called when a query should be reported (listing test cases, printing the version, etc.)
    virtual void report_query(const QueryData&) = 0;

    // called when the whole test run starts
    virtual void test_run_start() = 0;
    // called when the whole test run ends (caching a pointer to the input doesn't make sense here)
    virtual void test_run_end(const TestRunStats&) = 0;

    // called when a test case is started (safe to cache a pointer to the input)
    virtual void test_case_start(const TestCaseData&) = 0;
    // called when a test case is reentered because of unfinished subcases (safe to cache a pointer to the input)
    virtual void test_case_reenter(const TestCaseData&) = 0;
    // called when a test case has ended
    virtual void test_case_end(const CurrentTestCaseStats&) = 0;

    // called when an exception is thrown from the test case (or it crashes)
    virtual void test_case_exception(const TestCaseException&) = 0;

    // called whenever a subcase is entered (don't cache pointers to the input)
    virtual void subcase_start(const SubcaseSignature&) = 0;
    // called whenever a subcase is exited (don't cache pointers to the input)
    virtual void subcase_end() = 0;

    // called for each assert (don't cache pointers to the input)
    virtual void log_assert(const AssertData&) = 0;
    // called for each message (don't cache pointers to the input)
    virtual void log_message(const MessageData&) = 0;

    // called when a test case is skipped either because it doesn't pass the filters, has a skip decorator
    // or isn't in the execution range (between first and last) (safe to cache a pointer to the input)
    virtual void test_case_skipped(const TestCaseData&) = 0;

    // doctest will not be managing the lifetimes of reporters given to it but this would still be nice to have
    virtual ~IReporter();

    // can obtain all currently active contexts and stringify them if one wishes to do so
    static int                         get_num_active_contexts();
    static const IContextScope* const* get_active_contexts();

    // can iterate through contexts which have been stringified automatically in their destructors when an exception has been thrown
    static int           get_num_stringified_contexts();
    static const String* get_stringified_contexts();
};

namespace detail {
    typedef IReporter* (*reporterCreatorFunc)(const ContextOptions&);

    DOCTEST_INTERFACE void registerReporterImpl(const char* name, int prio, reporterCreatorFunc c, bool isReporter);

    template <typename Reporter>
    IReporter* reporterCreator(const ContextOptions& o) {
        return new Reporter(o);
    }
} // namespace detail

template <typename Reporter>
int registerReporter(const char* name, int priority, bool isReporter) {
    detail::registerReporterImpl(name, priority, detail::reporterCreator<Reporter>, isReporter);
    return 0;
}
} // namespace doctest

// if registering is not disabled
#if !defined(DOCTEST_CONFIG_DISABLE)

// common code in asserts - for convenience
#define DOCTEST_ASSERT_LOG_AND_REACT(b)                                                            \
    if(b.log())                                                                                    \
        DOCTEST_BREAK_INTO_DEBUGGER();                                                             \
    b.react()

#ifdef DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#define DOCTEST_WRAP_IN_TRY(x) x;
#else // DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#define DOCTEST_WRAP_IN_TRY(x)                                                                     \
    try {                                                                                          \
        x;                                                                                         \
    } catch(...) { DOCTEST_RB.translateException(); }
#endif // DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS

#ifdef DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#define DOCTEST_CAST_TO_VOID(...)                                                                  \
    DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wuseless-cast")                                       \
    static_cast<void>(__VA_ARGS__);                                                                \
    DOCTEST_GCC_SUPPRESS_WARNING_POP
#else // DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#define DOCTEST_CAST_TO_VOID(...) __VA_ARGS__;
#endif // DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS

// registers the test by initializing a dummy var with a function
#define DOCTEST_REGISTER_FUNCTION(global_prefix, f, decorators)                                    \
    global_prefix DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_VAR_)) =               \
            doctest::detail::regTest(                                                              \
                    doctest::detail::TestCase(                                                     \
                            f, __FILE__, __LINE__,                                                 \
                            doctest_detail_test_suite_ns::getCurrentTestSuite()) *                 \
                    decorators);                                                                   \
    DOCTEST_GLOBAL_NO_WARNINGS_END()

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, decorators)                                     \
    namespace {                                                                                    \
        struct der : public base                                                                   \
        {                                                                                          \
            void f();                                                                              \
        };                                                                                         \
        static void func() {                                                                       \
            der v;                                                                                 \
            v.f();                                                                                 \
        }                                                                                          \
        DOCTEST_REGISTER_FUNCTION(DOCTEST_EMPTY, func, decorators)                                 \
    }                                                                                              \
    inline DOCTEST_NOINLINE void der::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, decorators)                                        \
    static void f();                                                                               \
    DOCTEST_REGISTER_FUNCTION(DOCTEST_EMPTY, f, decorators)                                        \
    static void f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION_IN_CLASS(f, proxy, decorators)                        \
    static doctest::detail::funcType proxy() { return f; }                                         \
    DOCTEST_REGISTER_FUNCTION(inline, proxy(), decorators)                                   \
    static void f()

// for registering tests
#define DOCTEST_TEST_CASE(decorators)                                                              \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_), decorators)

// for registering tests in classes - requires C++17 for inline variables!
#if DOCTEST_CPP >= 17
#define DOCTEST_TEST_CASE_CLASS(decorators)                                                        \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION_IN_CLASS(DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_),           \
                                                  DOCTEST_ANONYMOUS(DOCTEST_ANON_PROXY_),          \
                                                  decorators)
#else // DOCTEST_TEST_CASE_CLASS
#define DOCTEST_TEST_CASE_CLASS(...)                                                               \
    TEST_CASES_CAN_BE_REGISTERED_IN_CLASSES_ONLY_IN_CPP17_MODE_OR_WITH_VS_2017_OR_NEWER
#endif // DOCTEST_TEST_CASE_CLASS

// for registering tests with a fixture
#define DOCTEST_TEST_CASE_FIXTURE(c, decorators)                                                   \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_ANON_CLASS_), c,                           \
                              DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_), decorators)

// for converting types to strings without the <typeinfo> header and demangling
#define DOCTEST_TYPE_TO_STRING_IMPL(...)                                                           \
    template <>                                                                                    \
    inline const char* type_to_string<__VA_ARGS__>() {                                             \
        return "<" #__VA_ARGS__ ">";                                                               \
    }
#define DOCTEST_TYPE_TO_STRING(...)                                                                \
    namespace doctest { namespace detail {                                                         \
            DOCTEST_TYPE_TO_STRING_IMPL(__VA_ARGS__)                                               \
        }                                                                                          \
    }                                                                                              \
    typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

#define DOCTEST_TEST_CASE_TEMPLATE_DEFINE_IMPL(dec, T, iter, func)                                 \
    template <typename T>                                                                          \
    static void func();                                                                            \
    namespace {                                                                                    \
        template <typename Tuple>                                                                  \
        struct iter;                                                                               \
        template <typename Type, typename... Rest>                                                 \
        struct iter<std::tuple<Type, Rest...>>                                                     \
        {                                                                                          \
            iter(const char* file, unsigned line, int index) {                                     \
                doctest::detail::regTest(doctest::detail::TestCase(func<Type>, file, line,         \
                                            doctest_detail_test_suite_ns::getCurrentTestSuite(),   \
                                            doctest::detail::type_to_string<Type>(),               \
                                            int(line) * 1000 + index)                              \
                                         * dec);                                                   \
                iter<std::tuple<Rest...>>(file, line, index + 1);                                  \
            }                                                                                      \
        };                                                                                         \
        template <>                                                                                \
        struct iter<std::tuple<>>                                                                  \
        {                                                                                          \
            iter(const char*, unsigned, int) {}                                                    \
        };                                                                                         \
    }                                                                                              \
    template <typename T>                                                                          \
    static void func()

#define DOCTEST_TEST_CASE_TEMPLATE_DEFINE(dec, T, id)                                              \
    DOCTEST_TEST_CASE_TEMPLATE_DEFINE_IMPL(dec, T, DOCTEST_CAT(id, ITERATOR),                      \
                                           DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_))

#define DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE_IMPL(id, anon, ...)                                 \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_CAT(anon, DUMMY)) =                                         \
        doctest::detail::instantiationHelper(DOCTEST_CAT(id, ITERATOR)<__VA_ARGS__>(__FILE__, __LINE__, 0));\
    DOCTEST_GLOBAL_NO_WARNINGS_END()

#define DOCTEST_TEST_CASE_TEMPLATE_INVOKE(id, ...)                                                 \
    DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE_IMPL(id, DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_), std::tuple<__VA_ARGS__>) \
    typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

#define DOCTEST_TEST_CASE_TEMPLATE_APPLY(id, ...)                                                  \
    DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE_IMPL(id, DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_), __VA_ARGS__) \
    typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

#define DOCTEST_TEST_CASE_TEMPLATE_IMPL(dec, T, anon, ...)                                         \
    DOCTEST_TEST_CASE_TEMPLATE_DEFINE_IMPL(dec, T, DOCTEST_CAT(anon, ITERATOR), anon);             \
    DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE_IMPL(anon, anon, std::tuple<__VA_ARGS__>)               \
    template <typename T>                                                                          \
    static void anon()

#define DOCTEST_TEST_CASE_TEMPLATE(dec, T, ...)                                                    \
    DOCTEST_TEST_CASE_TEMPLATE_IMPL(dec, T, DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_), __VA_ARGS__)

// for subcases
#define DOCTEST_SUBCASE(name)                                                                      \
    if(const doctest::detail::Subcase & DOCTEST_ANONYMOUS(DOCTEST_ANON_SUBCASE_) DOCTEST_UNUSED =  \
               doctest::detail::Subcase(name, __FILE__, __LINE__))

// for grouping tests in test suites by using code blocks
#define DOCTEST_TEST_SUITE_IMPL(decorators, ns_name)                                               \
    namespace ns_name { namespace doctest_detail_test_suite_ns {                                   \
            static DOCTEST_NOINLINE doctest::detail::TestSuite& getCurrentTestSuite() {            \
                DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4640)                                      \
                DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wexit-time-destructors")                \
                DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wmissing-field-initializers")             \
                static doctest::detail::TestSuite data{};                                          \
                static bool                       inited = false;                                  \
                DOCTEST_MSVC_SUPPRESS_WARNING_POP                                                  \
                DOCTEST_CLANG_SUPPRESS_WARNING_POP                                                 \
                DOCTEST_GCC_SUPPRESS_WARNING_POP                                                   \
                if(!inited) {                                                                      \
                    data* decorators;                                                              \
                    inited = true;                                                                 \
                }                                                                                  \
                return data;                                                                       \
            }                                                                                      \
        }                                                                                          \
    }                                                                                              \
    namespace ns_name

#define DOCTEST_TEST_SUITE(decorators)                                                             \
    DOCTEST_TEST_SUITE_IMPL(decorators, DOCTEST_ANONYMOUS(DOCTEST_ANON_SUITE_))

// for starting a testsuite block
#define DOCTEST_TEST_SUITE_BEGIN(decorators)                                                       \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_VAR_)) =                             \
            doctest::detail::setTestSuite(doctest::detail::TestSuite() * decorators);              \
    DOCTEST_GLOBAL_NO_WARNINGS_END()                                                               \
    typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

// for ending a testsuite block
#define DOCTEST_TEST_SUITE_END                                                                     \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_VAR_)) =                             \
            doctest::detail::setTestSuite(doctest::detail::TestSuite() * "");                      \
    DOCTEST_GLOBAL_NO_WARNINGS_END()                                                               \
    typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

// for registering exception translators
#define DOCTEST_REGISTER_EXCEPTION_TRANSLATOR_IMPL(translatorName, signature)                      \
    inline doctest::String translatorName(signature);                                              \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_TRANSLATOR_)) =                      \
            doctest::registerExceptionTranslator(translatorName);                                  \
    DOCTEST_GLOBAL_NO_WARNINGS_END()                                                               \
    doctest::String translatorName(signature)

#define DOCTEST_REGISTER_EXCEPTION_TRANSLATOR(signature)                                           \
    DOCTEST_REGISTER_EXCEPTION_TRANSLATOR_IMPL(DOCTEST_ANONYMOUS(DOCTEST_ANON_TRANSLATOR_),        \
                                               signature)

// for registering reporters
#define DOCTEST_REGISTER_REPORTER(name, priority, reporter)                                        \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_REPORTER_)) =                        \
            doctest::registerReporter<reporter>(name, priority, true);                             \
    DOCTEST_GLOBAL_NO_WARNINGS_END() typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

// for registering listeners
#define DOCTEST_REGISTER_LISTENER(name, priority, reporter)                                        \
    DOCTEST_GLOBAL_NO_WARNINGS(DOCTEST_ANONYMOUS(DOCTEST_ANON_REPORTER_)) =                        \
            doctest::registerReporter<reporter>(name, priority, false);                            \
    DOCTEST_GLOBAL_NO_WARNINGS_END() typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

// clang-format off
// for logging - disabling formatting because it's important to have these on 2 separate lines - see PR #557
#define DOCTEST_INFO(...)                                                                          \
    DOCTEST_INFO_IMPL(DOCTEST_ANONYMOUS(DOCTEST_CAPTURE_),                                         \
                      DOCTEST_ANONYMOUS(DOCTEST_CAPTURE_OTHER_),                                   \
                      __VA_ARGS__)
// clang-format on

#define DOCTEST_INFO_IMPL(mb_name, s_name, ...)                                       \
    auto DOCTEST_ANONYMOUS(DOCTEST_CAPTURE_) = doctest::detail::MakeContextScope(                  \
        [&](std::ostream* s_name) {                                                                \
        doctest::detail::MessageBuilder mb_name(__FILE__, __LINE__, doctest::assertType::is_warn); \
        mb_name.m_stream = s_name;                                                                 \
        mb_name * __VA_ARGS__;                                                                     \
    })

#define DOCTEST_CAPTURE(x) DOCTEST_INFO(#x " := ", x)

#define DOCTEST_ADD_AT_IMPL(type, file, line, mb, ...)                                             \
    do {                                                                                           \
        doctest::detail::MessageBuilder mb(file, line, doctest::assertType::type);                 \
        mb * __VA_ARGS__;                                                                          \
        DOCTEST_ASSERT_LOG_AND_REACT(mb);                                                          \
    } while(false)

// clang-format off
#define DOCTEST_ADD_MESSAGE_AT(file, line, ...) DOCTEST_ADD_AT_IMPL(is_warn, file, line, DOCTEST_ANONYMOUS(DOCTEST_MESSAGE_), __VA_ARGS__)
#define DOCTEST_ADD_FAIL_CHECK_AT(file, line, ...) DOCTEST_ADD_AT_IMPL(is_check, file, line, DOCTEST_ANONYMOUS(DOCTEST_MESSAGE_), __VA_ARGS__)
#define DOCTEST_ADD_FAIL_AT(file, line, ...) DOCTEST_ADD_AT_IMPL(is_require, file, line, DOCTEST_ANONYMOUS(DOCTEST_MESSAGE_), __VA_ARGS__)
// clang-format on

#define DOCTEST_MESSAGE(...) DOCTEST_ADD_MESSAGE_AT(__FILE__, __LINE__, __VA_ARGS__)
#define DOCTEST_FAIL_CHECK(...) DOCTEST_ADD_FAIL_CHECK_AT(__FILE__, __LINE__, __VA_ARGS__)
#define DOCTEST_FAIL(...) DOCTEST_ADD_FAIL_AT(__FILE__, __LINE__, __VA_ARGS__)

#define DOCTEST_TO_LVALUE(...) __VA_ARGS__ // Not removed to keep backwards compatibility.

#ifndef DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#define DOCTEST_ASSERT_IMPLEMENT_2(assert_type, ...)                                               \
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Woverloaded-shift-op-parentheses")                  \
    doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,          \
                                               __LINE__, #__VA_ARGS__);                            \
    DOCTEST_WRAP_IN_TRY(DOCTEST_RB.setResult(                                                      \
            doctest::detail::ExpressionDecomposer(doctest::assertType::assert_type)                \
            << __VA_ARGS__))                                                                       \
    DOCTEST_ASSERT_LOG_AND_REACT(DOCTEST_RB)                                                       \
    DOCTEST_CLANG_SUPPRESS_WARNING_POP

#define DOCTEST_ASSERT_IMPLEMENT_1(assert_type, ...)                                               \
    do {                                                                                           \
        DOCTEST_ASSERT_IMPLEMENT_2(assert_type, __VA_ARGS__);                                      \
    } while(false)

#else // DOCTEST_CONFIG_SUPER_FAST_ASSERTS

// necessary for <ASSERT>_MESSAGE
#define DOCTEST_ASSERT_IMPLEMENT_2 DOCTEST_ASSERT_IMPLEMENT_1

#define DOCTEST_ASSERT_IMPLEMENT_1(assert_type, ...)                                               \
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Woverloaded-shift-op-parentheses")                  \
    doctest::detail::decomp_assert(                                                                \
            doctest::assertType::assert_type, __FILE__, __LINE__, #__VA_ARGS__,                    \
            doctest::detail::ExpressionDecomposer(doctest::assertType::assert_type)                \
                    << __VA_ARGS__) DOCTEST_CLANG_SUPPRESS_WARNING_POP

#endif // DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#define DOCTEST_WARN(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_WARN, __VA_ARGS__)
#define DOCTEST_CHECK(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_CHECK, __VA_ARGS__)
#define DOCTEST_REQUIRE(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_REQUIRE, __VA_ARGS__)
#define DOCTEST_WARN_FALSE(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_WARN_FALSE, __VA_ARGS__)
#define DOCTEST_CHECK_FALSE(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_CHECK_FALSE, __VA_ARGS__)
#define DOCTEST_REQUIRE_FALSE(...) DOCTEST_ASSERT_IMPLEMENT_1(DT_REQUIRE_FALSE, __VA_ARGS__)

// clang-format off
#define DOCTEST_WARN_MESSAGE(cond, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_WARN, cond); } while(false)
#define DOCTEST_CHECK_MESSAGE(cond, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_CHECK, cond); } while(false)
#define DOCTEST_REQUIRE_MESSAGE(cond, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_REQUIRE, cond); } while(false)
#define DOCTEST_WARN_FALSE_MESSAGE(cond, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_WARN_FALSE, cond); } while(false)
#define DOCTEST_CHECK_FALSE_MESSAGE(cond, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_CHECK_FALSE, cond); } while(false)
#define DOCTEST_REQUIRE_FALSE_MESSAGE(cond, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_ASSERT_IMPLEMENT_2(DT_REQUIRE_FALSE, cond); } while(false)
// clang-format on

#define DOCTEST_ASSERT_THROWS_AS(expr, assert_type, message, ...)                                  \
    do {                                                                                           \
        if(!doctest::getContextOptions()->no_throw) {                                              \
            doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,  \
                                                       __LINE__, #expr, #__VA_ARGS__, message);    \
            try {                                                                                  \
                DOCTEST_CAST_TO_VOID(expr)                                                         \
            } catch(const typename doctest::detail::remove_const<                                  \
                    typename doctest::detail::remove_reference<__VA_ARGS__>::type>::type&) {       \
                DOCTEST_RB.translateException();                                                   \
                DOCTEST_RB.m_threw_as = true;                                                      \
            } catch(...) { DOCTEST_RB.translateException(); }                                      \
            DOCTEST_ASSERT_LOG_AND_REACT(DOCTEST_RB);                                              \
        }                                                                                          \
    } while(false)

#define DOCTEST_ASSERT_THROWS_WITH(expr, expr_str, assert_type, ...)                               \
    do {                                                                                           \
        if(!doctest::getContextOptions()->no_throw) {                                              \
            doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,  \
                                                       __LINE__, expr_str, "", __VA_ARGS__);       \
            try {                                                                                  \
                DOCTEST_CAST_TO_VOID(expr)                                                         \
            } catch(...) { DOCTEST_RB.translateException(); }                                      \
            DOCTEST_ASSERT_LOG_AND_REACT(DOCTEST_RB);                                              \
        }                                                                                          \
    } while(false)

#define DOCTEST_ASSERT_NOTHROW(assert_type, ...)                                                   \
    do {                                                                                           \
        doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,      \
                                                   __LINE__, #__VA_ARGS__);                        \
        try {                                                                                      \
            DOCTEST_CAST_TO_VOID(__VA_ARGS__)                                                      \
        } catch(...) { DOCTEST_RB.translateException(); }                                          \
        DOCTEST_ASSERT_LOG_AND_REACT(DOCTEST_RB);                                                  \
    } while(false)

// clang-format off
#define DOCTEST_WARN_THROWS(...) DOCTEST_ASSERT_THROWS_WITH((__VA_ARGS__), #__VA_ARGS__, DT_WARN_THROWS, "")
#define DOCTEST_CHECK_THROWS(...) DOCTEST_ASSERT_THROWS_WITH((__VA_ARGS__), #__VA_ARGS__, DT_CHECK_THROWS, "")
#define DOCTEST_REQUIRE_THROWS(...) DOCTEST_ASSERT_THROWS_WITH((__VA_ARGS__), #__VA_ARGS__, DT_REQUIRE_THROWS, "")

#define DOCTEST_WARN_THROWS_AS(expr, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_WARN_THROWS_AS, "", __VA_ARGS__)
#define DOCTEST_CHECK_THROWS_AS(expr, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_CHECK_THROWS_AS, "", __VA_ARGS__)
#define DOCTEST_REQUIRE_THROWS_AS(expr, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_REQUIRE_THROWS_AS, "", __VA_ARGS__)

#define DOCTEST_WARN_THROWS_WITH(expr, ...) DOCTEST_ASSERT_THROWS_WITH(expr, #expr, DT_WARN_THROWS_WITH, __VA_ARGS__)
#define DOCTEST_CHECK_THROWS_WITH(expr, ...) DOCTEST_ASSERT_THROWS_WITH(expr, #expr, DT_CHECK_THROWS_WITH, __VA_ARGS__)
#define DOCTEST_REQUIRE_THROWS_WITH(expr, ...) DOCTEST_ASSERT_THROWS_WITH(expr, #expr, DT_REQUIRE_THROWS_WITH, __VA_ARGS__)

#define DOCTEST_WARN_THROWS_WITH_AS(expr, message, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_WARN_THROWS_WITH_AS, message, __VA_ARGS__)
#define DOCTEST_CHECK_THROWS_WITH_AS(expr, message, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_CHECK_THROWS_WITH_AS, message, __VA_ARGS__)
#define DOCTEST_REQUIRE_THROWS_WITH_AS(expr, message, ...) DOCTEST_ASSERT_THROWS_AS(expr, DT_REQUIRE_THROWS_WITH_AS, message, __VA_ARGS__)

#define DOCTEST_WARN_NOTHROW(...) DOCTEST_ASSERT_NOTHROW(DT_WARN_NOTHROW, __VA_ARGS__)
#define DOCTEST_CHECK_NOTHROW(...) DOCTEST_ASSERT_NOTHROW(DT_CHECK_NOTHROW, __VA_ARGS__)
#define DOCTEST_REQUIRE_NOTHROW(...) DOCTEST_ASSERT_NOTHROW(DT_REQUIRE_NOTHROW, __VA_ARGS__)

#define DOCTEST_WARN_THROWS_MESSAGE(expr, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_WARN_THROWS(expr); } while(false)
#define DOCTEST_CHECK_THROWS_MESSAGE(expr, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_CHECK_THROWS(expr); } while(false)
#define DOCTEST_REQUIRE_THROWS_MESSAGE(expr, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_REQUIRE_THROWS(expr); } while(false)
#define DOCTEST_WARN_THROWS_AS_MESSAGE(expr, ex, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_WARN_THROWS_AS(expr, ex); } while(false)
#define DOCTEST_CHECK_THROWS_AS_MESSAGE(expr, ex, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_CHECK_THROWS_AS(expr, ex); } while(false)
#define DOCTEST_REQUIRE_THROWS_AS_MESSAGE(expr, ex, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_REQUIRE_THROWS_AS(expr, ex); } while(false)
#define DOCTEST_WARN_THROWS_WITH_MESSAGE(expr, with, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_WARN_THROWS_WITH(expr, with); } while(false)
#define DOCTEST_CHECK_THROWS_WITH_MESSAGE(expr, with, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_CHECK_THROWS_WITH(expr, with); } while(false)
#define DOCTEST_REQUIRE_THROWS_WITH_MESSAGE(expr, with, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_REQUIRE_THROWS_WITH(expr, with); } while(false)
#define DOCTEST_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_WARN_THROWS_WITH_AS(expr, with, ex); } while(false)
#define DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_CHECK_THROWS_WITH_AS(expr, with, ex); } while(false)
#define DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_REQUIRE_THROWS_WITH_AS(expr, with, ex); } while(false)
#define DOCTEST_WARN_NOTHROW_MESSAGE(expr, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_WARN_NOTHROW(expr); } while(false)
#define DOCTEST_CHECK_NOTHROW_MESSAGE(expr, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_CHECK_NOTHROW(expr); } while(false)
#define DOCTEST_REQUIRE_NOTHROW_MESSAGE(expr, ...) do { DOCTEST_INFO(__VA_ARGS__); DOCTEST_REQUIRE_NOTHROW(expr); } while(false)
// clang-format on

#ifndef DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#define DOCTEST_BINARY_ASSERT(assert_type, comp, ...)                                              \
    do {                                                                                           \
        doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,      \
                                                   __LINE__, #__VA_ARGS__);                        \
        DOCTEST_WRAP_IN_TRY(                                                                       \
                DOCTEST_RB.binary_assert<doctest::detail::binaryAssertComparison::comp>(           \
                        __VA_ARGS__))                                                              \
        DOCTEST_ASSERT_LOG_AND_REACT(DOCTEST_RB);                                                  \
    } while(false)

#define DOCTEST_UNARY_ASSERT(assert_type, ...)                                                     \
    do {                                                                                           \
        doctest::detail::ResultBuilder DOCTEST_RB(doctest::assertType::assert_type, __FILE__,      \
                                                   __LINE__, #__VA_ARGS__);                        \
        DOCTEST_WRAP_IN_TRY(DOCTEST_RB.unary_assert(__VA_ARGS__))                                  \
        DOCTEST_ASSERT_LOG_AND_REACT(DOCTEST_RB);                                                  \
    } while(false)

#else // DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#define DOCTEST_BINARY_ASSERT(assert_type, comparison, ...)                                        \
    doctest::detail::binary_assert<doctest::detail::binaryAssertComparison::comparison>(           \
            doctest::assertType::assert_type, __FILE__, __LINE__, #__VA_ARGS__, __VA_ARGS__)

#define DOCTEST_UNARY_ASSERT(assert_type, ...)                                                     \
    doctest::detail::unary_assert(doctest::assertType::assert_type, __FILE__, __LINE__,            \
                                  #__VA_ARGS__, __VA_ARGS__)

#endif // DOCTEST_CONFIG_SUPER_FAST_ASSERTS

#define DOCTEST_WARN_EQ(...) DOCTEST_BINARY_ASSERT(DT_WARN_EQ, eq, __VA_ARGS__)
#define DOCTEST_CHECK_EQ(...) DOCTEST_BINARY_ASSERT(DT_CHECK_EQ, eq, __VA_ARGS__)
#define DOCTEST_REQUIRE_EQ(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_EQ, eq, __VA_ARGS__)
#define DOCTEST_WARN_NE(...) DOCTEST_BINARY_ASSERT(DT_WARN_NE, ne, __VA_ARGS__)
#define DOCTEST_CHECK_NE(...) DOCTEST_BINARY_ASSERT(DT_CHECK_NE, ne, __VA_ARGS__)
#define DOCTEST_REQUIRE_NE(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_NE, ne, __VA_ARGS__)
#define DOCTEST_WARN_GT(...) DOCTEST_BINARY_ASSERT(DT_WARN_GT, gt, __VA_ARGS__)
#define DOCTEST_CHECK_GT(...) DOCTEST_BINARY_ASSERT(DT_CHECK_GT, gt, __VA_ARGS__)
#define DOCTEST_REQUIRE_GT(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_GT, gt, __VA_ARGS__)
#define DOCTEST_WARN_LT(...) DOCTEST_BINARY_ASSERT(DT_WARN_LT, lt, __VA_ARGS__)
#define DOCTEST_CHECK_LT(...) DOCTEST_BINARY_ASSERT(DT_CHECK_LT, lt, __VA_ARGS__)
#define DOCTEST_REQUIRE_LT(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_LT, lt, __VA_ARGS__)
#define DOCTEST_WARN_GE(...) DOCTEST_BINARY_ASSERT(DT_WARN_GE, ge, __VA_ARGS__)
#define DOCTEST_CHECK_GE(...) DOCTEST_BINARY_ASSERT(DT_CHECK_GE, ge, __VA_ARGS__)
#define DOCTEST_REQUIRE_GE(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_GE, ge, __VA_ARGS__)
#define DOCTEST_WARN_LE(...) DOCTEST_BINARY_ASSERT(DT_WARN_LE, le, __VA_ARGS__)
#define DOCTEST_CHECK_LE(...) DOCTEST_BINARY_ASSERT(DT_CHECK_LE, le, __VA_ARGS__)
#define DOCTEST_REQUIRE_LE(...) DOCTEST_BINARY_ASSERT(DT_REQUIRE_LE, le, __VA_ARGS__)

#define DOCTEST_WARN_UNARY(...) DOCTEST_UNARY_ASSERT(DT_WARN_UNARY, __VA_ARGS__)
#define DOCTEST_CHECK_UNARY(...) DOCTEST_UNARY_ASSERT(DT_CHECK_UNARY, __VA_ARGS__)
#define DOCTEST_REQUIRE_UNARY(...) DOCTEST_UNARY_ASSERT(DT_REQUIRE_UNARY, __VA_ARGS__)
#define DOCTEST_WARN_UNARY_FALSE(...) DOCTEST_UNARY_ASSERT(DT_WARN_UNARY_FALSE, __VA_ARGS__)
#define DOCTEST_CHECK_UNARY_FALSE(...) DOCTEST_UNARY_ASSERT(DT_CHECK_UNARY_FALSE, __VA_ARGS__)
#define DOCTEST_REQUIRE_UNARY_FALSE(...) DOCTEST_UNARY_ASSERT(DT_REQUIRE_UNARY_FALSE, __VA_ARGS__)

#ifdef DOCTEST_CONFIG_NO_EXCEPTIONS

#undef DOCTEST_WARN_THROWS
#undef DOCTEST_CHECK_THROWS
#undef DOCTEST_REQUIRE_THROWS
#undef DOCTEST_WARN_THROWS_AS
#undef DOCTEST_CHECK_THROWS_AS
#undef DOCTEST_REQUIRE_THROWS_AS
#undef DOCTEST_WARN_THROWS_WITH
#undef DOCTEST_CHECK_THROWS_WITH
#undef DOCTEST_REQUIRE_THROWS_WITH
#undef DOCTEST_WARN_THROWS_WITH_AS
#undef DOCTEST_CHECK_THROWS_WITH_AS
#undef DOCTEST_REQUIRE_THROWS_WITH_AS
#undef DOCTEST_WARN_NOTHROW
#undef DOCTEST_CHECK_NOTHROW
#undef DOCTEST_REQUIRE_NOTHROW

#undef DOCTEST_WARN_THROWS_MESSAGE
#undef DOCTEST_CHECK_THROWS_MESSAGE
#undef DOCTEST_REQUIRE_THROWS_MESSAGE
#undef DOCTEST_WARN_THROWS_AS_MESSAGE
#undef DOCTEST_CHECK_THROWS_AS_MESSAGE
#undef DOCTEST_REQUIRE_THROWS_AS_MESSAGE
#undef DOCTEST_WARN_THROWS_WITH_MESSAGE
#undef DOCTEST_CHECK_THROWS_WITH_MESSAGE
#undef DOCTEST_REQUIRE_THROWS_WITH_MESSAGE
#undef DOCTEST_WARN_THROWS_WITH_AS_MESSAGE
#undef DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE
#undef DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE
#undef DOCTEST_WARN_NOTHROW_MESSAGE
#undef DOCTEST_CHECK_NOTHROW_MESSAGE
#undef DOCTEST_REQUIRE_NOTHROW_MESSAGE

#ifdef DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#define DOCTEST_WARN_THROWS(...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS(...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_AS(expr, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_AS(expr, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_AS(expr, ...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_WITH(expr, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_WITH(expr, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_WITH(expr, ...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_WITH_AS(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_WITH_AS(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_WITH_AS(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_WARN_NOTHROW(...) (static_cast<void>(0))
#define DOCTEST_CHECK_NOTHROW(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_NOTHROW(...) (static_cast<void>(0))

#define DOCTEST_WARN_THROWS_MESSAGE(expr, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_MESSAGE(expr, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_MESSAGE(expr, ...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_AS_MESSAGE(expr, ex, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_AS_MESSAGE(expr, ex, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_AS_MESSAGE(expr, ex, ...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_WITH_MESSAGE(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_WITH_MESSAGE(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_WITH_MESSAGE(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) (static_cast<void>(0))
#define DOCTEST_WARN_NOTHROW_MESSAGE(expr, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_NOTHROW_MESSAGE(expr, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_NOTHROW_MESSAGE(expr, ...) (static_cast<void>(0))

#else // DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#undef DOCTEST_REQUIRE
#undef DOCTEST_REQUIRE_FALSE
#undef DOCTEST_REQUIRE_MESSAGE
#undef DOCTEST_REQUIRE_FALSE_MESSAGE
#undef DOCTEST_REQUIRE_EQ
#undef DOCTEST_REQUIRE_NE
#undef DOCTEST_REQUIRE_GT
#undef DOCTEST_REQUIRE_LT
#undef DOCTEST_REQUIRE_GE
#undef DOCTEST_REQUIRE_LE
#undef DOCTEST_REQUIRE_UNARY
#undef DOCTEST_REQUIRE_UNARY_FALSE

#endif // DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

// =================================================================================================
// == WHAT FOLLOWS IS VERSIONS OF THE MACROS THAT DO NOT DO ANY REGISTERING!                      ==
// == THIS CAN BE ENABLED BY DEFINING DOCTEST_CONFIG_DISABLE GLOBALLY!                            ==
// =================================================================================================
#else // DOCTEST_CONFIG_DISABLE

#define DOCTEST_IMPLEMENT_FIXTURE(der, base, func, name)                                           \
    namespace {                                                                                    \
        template <typename DOCTEST_UNUSED_TEMPLATE_TYPE>                                           \
        struct der : public base                                                                   \
        { void f(); };                                                                             \
    }                                                                                              \
    template <typename DOCTEST_UNUSED_TEMPLATE_TYPE>                                               \
    inline void der<DOCTEST_UNUSED_TEMPLATE_TYPE>::f()

#define DOCTEST_CREATE_AND_REGISTER_FUNCTION(f, name)                                              \
    template <typename DOCTEST_UNUSED_TEMPLATE_TYPE>                                               \
    static inline void f()

// for registering tests
#define DOCTEST_TEST_CASE(name)                                                                    \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_), name)

// for registering tests in classes
#define DOCTEST_TEST_CASE_CLASS(name)                                                              \
    DOCTEST_CREATE_AND_REGISTER_FUNCTION(DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_), name)

// for registering tests with a fixture
#define DOCTEST_TEST_CASE_FIXTURE(x, name)                                                         \
    DOCTEST_IMPLEMENT_FIXTURE(DOCTEST_ANONYMOUS(DOCTEST_ANON_CLASS_), x,                           \
                              DOCTEST_ANONYMOUS(DOCTEST_ANON_FUNC_), name)

// for converting types to strings without the <typeinfo> header and demangling
#define DOCTEST_TYPE_TO_STRING(...) typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)
#define DOCTEST_TYPE_TO_STRING_IMPL(...)

// for typed tests
#define DOCTEST_TEST_CASE_TEMPLATE(name, type, ...)                                                \
    template <typename type>                                                                       \
    inline void DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_)()

#define DOCTEST_TEST_CASE_TEMPLATE_DEFINE(name, type, id)                                          \
    template <typename type>                                                                       \
    inline void DOCTEST_ANONYMOUS(DOCTEST_ANON_TMP_)()

#define DOCTEST_TEST_CASE_TEMPLATE_INVOKE(id, ...)                                                 \
    typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

#define DOCTEST_TEST_CASE_TEMPLATE_APPLY(id, ...)                                                  \
    typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

// for subcases
#define DOCTEST_SUBCASE(name)

// for a testsuite block
#define DOCTEST_TEST_SUITE(name) namespace

// for starting a testsuite block
#define DOCTEST_TEST_SUITE_BEGIN(name) typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

// for ending a testsuite block
#define DOCTEST_TEST_SUITE_END typedef int DOCTEST_ANONYMOUS(DOCTEST_ANON_FOR_SEMICOLON_)

#define DOCTEST_REGISTER_EXCEPTION_TRANSLATOR(signature)                                           \
    template <typename DOCTEST_UNUSED_TEMPLATE_TYPE>                                               \
    static inline doctest::String DOCTEST_ANONYMOUS(DOCTEST_ANON_TRANSLATOR_)(signature)

#define DOCTEST_REGISTER_REPORTER(name, priority, reporter)
#define DOCTEST_REGISTER_LISTENER(name, priority, reporter)

#define DOCTEST_INFO(...) (static_cast<void>(0))
#define DOCTEST_CAPTURE(x) (static_cast<void>(0))
#define DOCTEST_ADD_MESSAGE_AT(file, line, ...) (static_cast<void>(0))
#define DOCTEST_ADD_FAIL_CHECK_AT(file, line, ...) (static_cast<void>(0))
#define DOCTEST_ADD_FAIL_AT(file, line, ...) (static_cast<void>(0))
#define DOCTEST_MESSAGE(...) (static_cast<void>(0))
#define DOCTEST_FAIL_CHECK(...) (static_cast<void>(0))
#define DOCTEST_FAIL(...) (static_cast<void>(0))

#define DOCTEST_WARN(...) (static_cast<void>(0))
#define DOCTEST_CHECK(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE(...) (static_cast<void>(0))
#define DOCTEST_WARN_FALSE(...) (static_cast<void>(0))
#define DOCTEST_CHECK_FALSE(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_FALSE(...) (static_cast<void>(0))

#define DOCTEST_WARN_MESSAGE(cond, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_MESSAGE(cond, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_MESSAGE(cond, ...) (static_cast<void>(0))
#define DOCTEST_WARN_FALSE_MESSAGE(cond, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_FALSE_MESSAGE(cond, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_FALSE_MESSAGE(cond, ...) (static_cast<void>(0))

#define DOCTEST_WARN_THROWS(...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS(...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_AS(expr, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_AS(expr, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_AS(expr, ...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_WITH(expr, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_WITH(expr, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_WITH(expr, ...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_WITH_AS(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_WITH_AS(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_WITH_AS(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_WARN_NOTHROW(...) (static_cast<void>(0))
#define DOCTEST_CHECK_NOTHROW(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_NOTHROW(...) (static_cast<void>(0))

#define DOCTEST_WARN_THROWS_MESSAGE(expr, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_MESSAGE(expr, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_MESSAGE(expr, ...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_AS_MESSAGE(expr, ex, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_AS_MESSAGE(expr, ex, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_AS_MESSAGE(expr, ex, ...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_WITH_MESSAGE(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_WITH_MESSAGE(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_WITH_MESSAGE(expr, with, ...) (static_cast<void>(0))
#define DOCTEST_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) (static_cast<void>(0))
#define DOCTEST_WARN_NOTHROW_MESSAGE(expr, ...) (static_cast<void>(0))
#define DOCTEST_CHECK_NOTHROW_MESSAGE(expr, ...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_NOTHROW_MESSAGE(expr, ...) (static_cast<void>(0))

#define DOCTEST_WARN_EQ(...) (static_cast<void>(0))
#define DOCTEST_CHECK_EQ(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_EQ(...) (static_cast<void>(0))
#define DOCTEST_WARN_NE(...) (static_cast<void>(0))
#define DOCTEST_CHECK_NE(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_NE(...) (static_cast<void>(0))
#define DOCTEST_WARN_GT(...) (static_cast<void>(0))
#define DOCTEST_CHECK_GT(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_GT(...) (static_cast<void>(0))
#define DOCTEST_WARN_LT(...) (static_cast<void>(0))
#define DOCTEST_CHECK_LT(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_LT(...) (static_cast<void>(0))
#define DOCTEST_WARN_GE(...) (static_cast<void>(0))
#define DOCTEST_CHECK_GE(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_GE(...) (static_cast<void>(0))
#define DOCTEST_WARN_LE(...) (static_cast<void>(0))
#define DOCTEST_CHECK_LE(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_LE(...) (static_cast<void>(0))

#define DOCTEST_WARN_UNARY(...) (static_cast<void>(0))
#define DOCTEST_CHECK_UNARY(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_UNARY(...) (static_cast<void>(0))
#define DOCTEST_WARN_UNARY_FALSE(...) (static_cast<void>(0))
#define DOCTEST_CHECK_UNARY_FALSE(...) (static_cast<void>(0))
#define DOCTEST_REQUIRE_UNARY_FALSE(...) (static_cast<void>(0))

#endif // DOCTEST_CONFIG_DISABLE

// clang-format off
// KEPT FOR BACKWARDS COMPATIBILITY - FORWARDING TO THE RIGHT MACROS
#define DOCTEST_FAST_WARN_EQ             DOCTEST_WARN_EQ
#define DOCTEST_FAST_CHECK_EQ            DOCTEST_CHECK_EQ
#define DOCTEST_FAST_REQUIRE_EQ          DOCTEST_REQUIRE_EQ
#define DOCTEST_FAST_WARN_NE             DOCTEST_WARN_NE
#define DOCTEST_FAST_CHECK_NE            DOCTEST_CHECK_NE
#define DOCTEST_FAST_REQUIRE_NE          DOCTEST_REQUIRE_NE
#define DOCTEST_FAST_WARN_GT             DOCTEST_WARN_GT
#define DOCTEST_FAST_CHECK_GT            DOCTEST_CHECK_GT
#define DOCTEST_FAST_REQUIRE_GT          DOCTEST_REQUIRE_GT
#define DOCTEST_FAST_WARN_LT             DOCTEST_WARN_LT
#define DOCTEST_FAST_CHECK_LT            DOCTEST_CHECK_LT
#define DOCTEST_FAST_REQUIRE_LT          DOCTEST_REQUIRE_LT
#define DOCTEST_FAST_WARN_GE             DOCTEST_WARN_GE
#define DOCTEST_FAST_CHECK_GE            DOCTEST_CHECK_GE
#define DOCTEST_FAST_REQUIRE_GE          DOCTEST_REQUIRE_GE
#define DOCTEST_FAST_WARN_LE             DOCTEST_WARN_LE
#define DOCTEST_FAST_CHECK_LE            DOCTEST_CHECK_LE
#define DOCTEST_FAST_REQUIRE_LE          DOCTEST_REQUIRE_LE

#define DOCTEST_FAST_WARN_UNARY          DOCTEST_WARN_UNARY
#define DOCTEST_FAST_CHECK_UNARY         DOCTEST_CHECK_UNARY
#define DOCTEST_FAST_REQUIRE_UNARY       DOCTEST_REQUIRE_UNARY
#define DOCTEST_FAST_WARN_UNARY_FALSE    DOCTEST_WARN_UNARY_FALSE
#define DOCTEST_FAST_CHECK_UNARY_FALSE   DOCTEST_CHECK_UNARY_FALSE
#define DOCTEST_FAST_REQUIRE_UNARY_FALSE DOCTEST_REQUIRE_UNARY_FALSE

#define DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE(id, ...) DOCTEST_TEST_CASE_TEMPLATE_INVOKE(id,__VA_ARGS__)
// clang-format on

// BDD style macros
// clang-format off
#define DOCTEST_SCENARIO(name) DOCTEST_TEST_CASE("  Scenario: " name)
#define DOCTEST_SCENARIO_CLASS(name) DOCTEST_TEST_CASE_CLASS("  Scenario: " name)
#define DOCTEST_SCENARIO_TEMPLATE(name, T, ...)  DOCTEST_TEST_CASE_TEMPLATE("  Scenario: " name, T, __VA_ARGS__)
#define DOCTEST_SCENARIO_TEMPLATE_DEFINE(name, T, id) DOCTEST_TEST_CASE_TEMPLATE_DEFINE("  Scenario: " name, T, id)

#define DOCTEST_GIVEN(name)     DOCTEST_SUBCASE("   Given: " name)
#define DOCTEST_WHEN(name)      DOCTEST_SUBCASE("    When: " name)
#define DOCTEST_AND_WHEN(name)  DOCTEST_SUBCASE("And when: " name)
#define DOCTEST_THEN(name)      DOCTEST_SUBCASE("    Then: " name)
#define DOCTEST_AND_THEN(name)  DOCTEST_SUBCASE("     And: " name)
// clang-format on

// == SHORT VERSIONS OF THE MACROS
#if !defined(DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES)

#define TEST_CASE(name) DOCTEST_TEST_CASE(name)
#define TEST_CASE_CLASS(name) DOCTEST_TEST_CASE_CLASS(name)
#define TEST_CASE_FIXTURE(x, name) DOCTEST_TEST_CASE_FIXTURE(x, name)
#define TYPE_TO_STRING(...) DOCTEST_TYPE_TO_STRING(__VA_ARGS__)
#define TEST_CASE_TEMPLATE(name, T, ...) DOCTEST_TEST_CASE_TEMPLATE(name, T, __VA_ARGS__)
#define TEST_CASE_TEMPLATE_DEFINE(name, T, id) DOCTEST_TEST_CASE_TEMPLATE_DEFINE(name, T, id)
#define TEST_CASE_TEMPLATE_INVOKE(id, ...) DOCTEST_TEST_CASE_TEMPLATE_INVOKE(id, __VA_ARGS__)
#define TEST_CASE_TEMPLATE_APPLY(id, ...) DOCTEST_TEST_CASE_TEMPLATE_APPLY(id, __VA_ARGS__)
#define SUBCASE(name) DOCTEST_SUBCASE(name)
#define TEST_SUITE(decorators) DOCTEST_TEST_SUITE(decorators)
#define TEST_SUITE_BEGIN(name) DOCTEST_TEST_SUITE_BEGIN(name)
#define TEST_SUITE_END DOCTEST_TEST_SUITE_END
#define REGISTER_EXCEPTION_TRANSLATOR(signature) DOCTEST_REGISTER_EXCEPTION_TRANSLATOR(signature)
#define REGISTER_REPORTER(name, priority, reporter) DOCTEST_REGISTER_REPORTER(name, priority, reporter)
#define REGISTER_LISTENER(name, priority, reporter) DOCTEST_REGISTER_LISTENER(name, priority, reporter)
#define INFO(...) DOCTEST_INFO(__VA_ARGS__)
#define CAPTURE(x) DOCTEST_CAPTURE(x)
#define ADD_MESSAGE_AT(file, line, ...) DOCTEST_ADD_MESSAGE_AT(file, line, __VA_ARGS__)
#define ADD_FAIL_CHECK_AT(file, line, ...) DOCTEST_ADD_FAIL_CHECK_AT(file, line, __VA_ARGS__)
#define ADD_FAIL_AT(file, line, ...) DOCTEST_ADD_FAIL_AT(file, line, __VA_ARGS__)
#define MESSAGE(...) DOCTEST_MESSAGE(__VA_ARGS__)
#define FAIL_CHECK(...) DOCTEST_FAIL_CHECK(__VA_ARGS__)
#define FAIL(...) DOCTEST_FAIL(__VA_ARGS__)
#define TO_LVALUE(...) DOCTEST_TO_LVALUE(__VA_ARGS__)

#define WARN(...) DOCTEST_WARN(__VA_ARGS__)
#define WARN_FALSE(...) DOCTEST_WARN_FALSE(__VA_ARGS__)
#define WARN_THROWS(...) DOCTEST_WARN_THROWS(__VA_ARGS__)
#define WARN_THROWS_AS(expr, ...) DOCTEST_WARN_THROWS_AS(expr, __VA_ARGS__)
#define WARN_THROWS_WITH(expr, ...) DOCTEST_WARN_THROWS_WITH(expr, __VA_ARGS__)
#define WARN_THROWS_WITH_AS(expr, with, ...) DOCTEST_WARN_THROWS_WITH_AS(expr, with, __VA_ARGS__)
#define WARN_NOTHROW(...) DOCTEST_WARN_NOTHROW(__VA_ARGS__)
#define CHECK(...) DOCTEST_CHECK(__VA_ARGS__)
#define CHECK_FALSE(...) DOCTEST_CHECK_FALSE(__VA_ARGS__)
#define CHECK_THROWS(...) DOCTEST_CHECK_THROWS(__VA_ARGS__)
#define CHECK_THROWS_AS(expr, ...) DOCTEST_CHECK_THROWS_AS(expr, __VA_ARGS__)
#define CHECK_THROWS_WITH(expr, ...) DOCTEST_CHECK_THROWS_WITH(expr, __VA_ARGS__)
#define CHECK_THROWS_WITH_AS(expr, with, ...) DOCTEST_CHECK_THROWS_WITH_AS(expr, with, __VA_ARGS__)
#define CHECK_NOTHROW(...) DOCTEST_CHECK_NOTHROW(__VA_ARGS__)
#define REQUIRE(...) DOCTEST_REQUIRE(__VA_ARGS__)
#define REQUIRE_FALSE(...) DOCTEST_REQUIRE_FALSE(__VA_ARGS__)
#define REQUIRE_THROWS(...) DOCTEST_REQUIRE_THROWS(__VA_ARGS__)
#define REQUIRE_THROWS_AS(expr, ...) DOCTEST_REQUIRE_THROWS_AS(expr, __VA_ARGS__)
#define REQUIRE_THROWS_WITH(expr, ...) DOCTEST_REQUIRE_THROWS_WITH(expr, __VA_ARGS__)
#define REQUIRE_THROWS_WITH_AS(expr, with, ...) DOCTEST_REQUIRE_THROWS_WITH_AS(expr, with, __VA_ARGS__)
#define REQUIRE_NOTHROW(...) DOCTEST_REQUIRE_NOTHROW(__VA_ARGS__)

#define WARN_MESSAGE(cond, ...) DOCTEST_WARN_MESSAGE(cond, __VA_ARGS__)
#define WARN_FALSE_MESSAGE(cond, ...) DOCTEST_WARN_FALSE_MESSAGE(cond, __VA_ARGS__)
#define WARN_THROWS_MESSAGE(expr, ...) DOCTEST_WARN_THROWS_MESSAGE(expr, __VA_ARGS__)
#define WARN_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_WARN_THROWS_AS_MESSAGE(expr, ex, __VA_ARGS__)
#define WARN_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_WARN_THROWS_WITH_MESSAGE(expr, with, __VA_ARGS__)
#define WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_WARN_THROWS_WITH_AS_MESSAGE(expr, with, ex, __VA_ARGS__)
#define WARN_NOTHROW_MESSAGE(expr, ...) DOCTEST_WARN_NOTHROW_MESSAGE(expr, __VA_ARGS__)
#define CHECK_MESSAGE(cond, ...) DOCTEST_CHECK_MESSAGE(cond, __VA_ARGS__)
#define CHECK_FALSE_MESSAGE(cond, ...) DOCTEST_CHECK_FALSE_MESSAGE(cond, __VA_ARGS__)
#define CHECK_THROWS_MESSAGE(expr, ...) DOCTEST_CHECK_THROWS_MESSAGE(expr, __VA_ARGS__)
#define CHECK_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_CHECK_THROWS_AS_MESSAGE(expr, ex, __VA_ARGS__)
#define CHECK_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_CHECK_THROWS_WITH_MESSAGE(expr, with, __VA_ARGS__)
#define CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_CHECK_THROWS_WITH_AS_MESSAGE(expr, with, ex, __VA_ARGS__)
#define CHECK_NOTHROW_MESSAGE(expr, ...) DOCTEST_CHECK_NOTHROW_MESSAGE(expr, __VA_ARGS__)
#define REQUIRE_MESSAGE(cond, ...) DOCTEST_REQUIRE_MESSAGE(cond, __VA_ARGS__)
#define REQUIRE_FALSE_MESSAGE(cond, ...) DOCTEST_REQUIRE_FALSE_MESSAGE(cond, __VA_ARGS__)
#define REQUIRE_THROWS_MESSAGE(expr, ...) DOCTEST_REQUIRE_THROWS_MESSAGE(expr, __VA_ARGS__)
#define REQUIRE_THROWS_AS_MESSAGE(expr, ex, ...) DOCTEST_REQUIRE_THROWS_AS_MESSAGE(expr, ex, __VA_ARGS__)
#define REQUIRE_THROWS_WITH_MESSAGE(expr, with, ...) DOCTEST_REQUIRE_THROWS_WITH_MESSAGE(expr, with, __VA_ARGS__)
#define REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, ...) DOCTEST_REQUIRE_THROWS_WITH_AS_MESSAGE(expr, with, ex, __VA_ARGS__)
#define REQUIRE_NOTHROW_MESSAGE(expr, ...) DOCTEST_REQUIRE_NOTHROW_MESSAGE(expr, __VA_ARGS__)

#define SCENARIO(name) DOCTEST_SCENARIO(name)
#define SCENARIO_CLASS(name) DOCTEST_SCENARIO_CLASS(name)
#define SCENARIO_TEMPLATE(name, T, ...) DOCTEST_SCENARIO_TEMPLATE(name, T, __VA_ARGS__)
#define SCENARIO_TEMPLATE_DEFINE(name, T, id) DOCTEST_SCENARIO_TEMPLATE_DEFINE(name, T, id)
#define GIVEN(name) DOCTEST_GIVEN(name)
#define WHEN(name) DOCTEST_WHEN(name)
#define AND_WHEN(name) DOCTEST_AND_WHEN(name)
#define THEN(name) DOCTEST_THEN(name)
#define AND_THEN(name) DOCTEST_AND_THEN(name)

#define WARN_EQ(...) DOCTEST_WARN_EQ(__VA_ARGS__)
#define CHECK_EQ(...) DOCTEST_CHECK_EQ(__VA_ARGS__)
#define REQUIRE_EQ(...) DOCTEST_REQUIRE_EQ(__VA_ARGS__)
#define WARN_NE(...) DOCTEST_WARN_NE(__VA_ARGS__)
#define CHECK_NE(...) DOCTEST_CHECK_NE(__VA_ARGS__)
#define REQUIRE_NE(...) DOCTEST_REQUIRE_NE(__VA_ARGS__)
#define WARN_GT(...) DOCTEST_WARN_GT(__VA_ARGS__)
#define CHECK_GT(...) DOCTEST_CHECK_GT(__VA_ARGS__)
#define REQUIRE_GT(...) DOCTEST_REQUIRE_GT(__VA_ARGS__)
#define WARN_LT(...) DOCTEST_WARN_LT(__VA_ARGS__)
#define CHECK_LT(...) DOCTEST_CHECK_LT(__VA_ARGS__)
#define REQUIRE_LT(...) DOCTEST_REQUIRE_LT(__VA_ARGS__)
#define WARN_GE(...) DOCTEST_WARN_GE(__VA_ARGS__)
#define CHECK_GE(...) DOCTEST_CHECK_GE(__VA_ARGS__)
#define REQUIRE_GE(...) DOCTEST_REQUIRE_GE(__VA_ARGS__)
#define WARN_LE(...) DOCTEST_WARN_LE(__VA_ARGS__)
#define CHECK_LE(...) DOCTEST_CHECK_LE(__VA_ARGS__)
#define REQUIRE_LE(...) DOCTEST_REQUIRE_LE(__VA_ARGS__)
#define WARN_UNARY(...) DOCTEST_WARN_UNARY(__VA_ARGS__)
#define CHECK_UNARY(...) DOCTEST_CHECK_UNARY(__VA_ARGS__)
#define REQUIRE_UNARY(...) DOCTEST_REQUIRE_UNARY(__VA_ARGS__)
#define WARN_UNARY_FALSE(...) DOCTEST_WARN_UNARY_FALSE(__VA_ARGS__)
#define CHECK_UNARY_FALSE(...) DOCTEST_CHECK_UNARY_FALSE(__VA_ARGS__)
#define REQUIRE_UNARY_FALSE(...) DOCTEST_REQUIRE_UNARY_FALSE(__VA_ARGS__)

// KEPT FOR BACKWARDS COMPATIBILITY
#define FAST_WARN_EQ(...) DOCTEST_FAST_WARN_EQ(__VA_ARGS__)
#define FAST_CHECK_EQ(...) DOCTEST_FAST_CHECK_EQ(__VA_ARGS__)
#define FAST_REQUIRE_EQ(...) DOCTEST_FAST_REQUIRE_EQ(__VA_ARGS__)
#define FAST_WARN_NE(...) DOCTEST_FAST_WARN_NE(__VA_ARGS__)
#define FAST_CHECK_NE(...) DOCTEST_FAST_CHECK_NE(__VA_ARGS__)
#define FAST_REQUIRE_NE(...) DOCTEST_FAST_REQUIRE_NE(__VA_ARGS__)
#define FAST_WARN_GT(...) DOCTEST_FAST_WARN_GT(__VA_ARGS__)
#define FAST_CHECK_GT(...) DOCTEST_FAST_CHECK_GT(__VA_ARGS__)
#define FAST_REQUIRE_GT(...) DOCTEST_FAST_REQUIRE_GT(__VA_ARGS__)
#define FAST_WARN_LT(...) DOCTEST_FAST_WARN_LT(__VA_ARGS__)
#define FAST_CHECK_LT(...) DOCTEST_FAST_CHECK_LT(__VA_ARGS__)
#define FAST_REQUIRE_LT(...) DOCTEST_FAST_REQUIRE_LT(__VA_ARGS__)
#define FAST_WARN_GE(...) DOCTEST_FAST_WARN_GE(__VA_ARGS__)
#define FAST_CHECK_GE(...) DOCTEST_FAST_CHECK_GE(__VA_ARGS__)
#define FAST_REQUIRE_GE(...) DOCTEST_FAST_REQUIRE_GE(__VA_ARGS__)
#define FAST_WARN_LE(...) DOCTEST_FAST_WARN_LE(__VA_ARGS__)
#define FAST_CHECK_LE(...) DOCTEST_FAST_CHECK_LE(__VA_ARGS__)
#define FAST_REQUIRE_LE(...) DOCTEST_FAST_REQUIRE_LE(__VA_ARGS__)

#define FAST_WARN_UNARY(...) DOCTEST_FAST_WARN_UNARY(__VA_ARGS__)
#define FAST_CHECK_UNARY(...) DOCTEST_FAST_CHECK_UNARY(__VA_ARGS__)
#define FAST_REQUIRE_UNARY(...) DOCTEST_FAST_REQUIRE_UNARY(__VA_ARGS__)
#define FAST_WARN_UNARY_FALSE(...) DOCTEST_FAST_WARN_UNARY_FALSE(__VA_ARGS__)
#define FAST_CHECK_UNARY_FALSE(...) DOCTEST_FAST_CHECK_UNARY_FALSE(__VA_ARGS__)
#define FAST_REQUIRE_UNARY_FALSE(...) DOCTEST_FAST_REQUIRE_UNARY_FALSE(__VA_ARGS__)

#define TEST_CASE_TEMPLATE_INSTANTIATE(id, ...) DOCTEST_TEST_CASE_TEMPLATE_INSTANTIATE(id, __VA_ARGS__)

#endif // DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES

#if !defined(DOCTEST_CONFIG_DISABLE)

// this is here to clear the 'current test suite' for the current translation unit - at the top
DOCTEST_TEST_SUITE_END();

// add stringification for primitive/fundamental types
namespace doctest { namespace detail {
    DOCTEST_TYPE_TO_STRING_IMPL(bool)
    DOCTEST_TYPE_TO_STRING_IMPL(float)
    DOCTEST_TYPE_TO_STRING_IMPL(double)
    DOCTEST_TYPE_TO_STRING_IMPL(long double)
    DOCTEST_TYPE_TO_STRING_IMPL(char)
    DOCTEST_TYPE_TO_STRING_IMPL(signed char)
    DOCTEST_TYPE_TO_STRING_IMPL(unsigned char)
#if !DOCTEST_MSVC || defined(_NATIVE_WCHAR_T_DEFINED)
    DOCTEST_TYPE_TO_STRING_IMPL(wchar_t)
#endif // not MSVC or wchar_t support enabled
    DOCTEST_TYPE_TO_STRING_IMPL(short int)
    DOCTEST_TYPE_TO_STRING_IMPL(unsigned short int)
    DOCTEST_TYPE_TO_STRING_IMPL(int)
    DOCTEST_TYPE_TO_STRING_IMPL(unsigned int)
    DOCTEST_TYPE_TO_STRING_IMPL(long int)
    DOCTEST_TYPE_TO_STRING_IMPL(unsigned long int)
    DOCTEST_TYPE_TO_STRING_IMPL(long long int)
    DOCTEST_TYPE_TO_STRING_IMPL(unsigned long long int)
}} // namespace doctest::detail

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_MSVC_SUPPRESS_WARNING_POP
DOCTEST_GCC_SUPPRESS_WARNING_POP

#endif // DOCTEST_LIBRARY_INCLUDED
