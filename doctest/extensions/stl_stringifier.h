#ifndef DOCTEST_STL_STRINGIFIER_H_INCLUDED
#define DOCTEST_STL_STRINGIFIER_H_INCLUDED

#ifndef DOCTEST_LIBRARY_INCLUDED
#include "doctest/doctest.h"
#endif

#define DOCTEST_STL_DETAIL_NAMESPACE_NAME detail
#define DOCTEST_STL_NAMESPACES_BEGIN namespace doctest { namespace DOCTEST_STL_DETAIL_NAMESPACE_NAME {
#define DOCTEST_STL_NAMESPACES_END } }


#define DOCTEST_STL_DEBRACE(...) __VA_ARGS__

#define DOCTEST_STL_STRINGIFY_IMPL_IMPL(funcTemp, specTemp, type, varName, func) \
DOCTEST_STL_NAMESPACES_BEGIN \
DOCTEST_STL_DEBRACE funcTemp \
inline String func(const DOCTEST_STL_DEBRACE type&); \
DOCTEST_STL_NAMESPACES_END \
namespace doctest { /* gcc bug forces this to be a proper namespace */ \
template <DOCTEST_STL_DEBRACE specTemp> \
struct StringMaker<DOCTEST_STL_DEBRACE type> { \
    static String convert(const DOCTEST_STL_DEBRACE type& value) { \
        return DOCTEST_STL_DETAIL_NAMESPACE_NAME::func(value); \
    } \
}; \
} \
DOCTEST_STL_DEBRACE funcTemp \
inline doctest::String doctest::DOCTEST_STL_DETAIL_NAMESPACE_NAME::func(const DOCTEST_STL_DEBRACE type& varName)

#define DOCTEST_STL_STRINGIFY_IMPL(funcTemp, specTemp, type, varName) \
DOCTEST_STL_STRINGIFY_IMPL_IMPL(funcTemp, specTemp, type, varName, DOCTEST_ANONYMOUS(_))

#define DOCTEST_STL_STRINGIFY(type, varName) \
DOCTEST_STL_STRINGIFY_IMPL((), (), (type), varName)

#define DOCTEST_STL_STRINGIFY_GEN_IMPL(temp, type, varName) \
DOCTEST_STL_STRINGIFY_IMPL((template <DOCTEST_STL_DEBRACE temp>), temp, type, varName)

#define DOCTEST_STL_STRINGIFY_GEN(temp, type, varName) DOCTEST_STL_STRINGIFY_GEN_IMPL(temp, type, varName)

#define DOCTEST_STL_CONTAINER(temp, type, begin, end) DOCTEST_STL_STRINGIFY_GEN(temp, type, var) { \
    String s = begin; \
    bool first = true; \
    for (const auto& v : var) { \
        if (!first) { \
            s += ", "; \
        } \
        first = false; \
        s += toString(v); \
    } \
    return s + end; \
}

#define DOCTEST_STL_ARRAY(temp, type) DOCTEST_STL_CONTAINER(temp, type, "[", "]")

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wstrict-overflow") // genuinely no clue

#if DOCTEST_CPP >= 11
#if defined(DOCTEST_STL_STRINGIFY_FLAG_ARRAY) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <array>
DOCTEST_STL_ARRAY((typename T, std::size_t SIZE), (std::array<T, SIZE>))
#endif
#endif

#ifdef __cpp_lib_span
#if defined(DOCTEST_STL_STRINGIFY_FLAG_SPAN) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <span>
DOCTEST_STL_ARRAY((typename T, std::size_t SIZE), (std::span<T, SIZE>))
#endif
#endif

#if defined(DOCTEST_STL_STRINGIFY_FLAG_VALARRAY) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <valarray>
DOCTEST_STL_STRINGIFY_GEN((typename T), (std::valarray<T>), var) {
    // this only exists because MSVC's STL is broken
    if (var.size() == 0) { return "[]"; }
    String s = "[";
    bool first = true;
    for (const auto& v : var) {
        if (!first) {
            s += ", ";
        }
        first = false;
        s += toString(v);
    }
    return s + "]";
}
#endif

#if DOCTEST_CPP >= 11
#if (defined(DOCTEST_STL_STRINGIFY_FLAG_INITIALIZER_LIST) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)) || !defined(DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES)
#include <initializer_list>
DOCTEST_STL_ARRAY((typename T), (std::initializer_list<T>))
#endif
#endif

#define DOCTEST_STL_SIMPLE_CONTAINER(type) DOCTEST_STL_CONTAINER((typename T, typename ALLOC), (type<T, ALLOC>), "[", "]")

#if (defined(DOCTEST_STL_STRINGIFY_FLAG_VECTOR) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)) || !defined(DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES)
#include <vector>
DOCTEST_STL_SIMPLE_CONTAINER(std::vector)
#endif

#if defined(DOCTEST_STL_STRINGIFY_FLAG_DEQUE) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <deque>
DOCTEST_STL_SIMPLE_CONTAINER(std::deque)
#endif

#if (defined(DOCTEST_STL_STRINGIFY_FLAG_LIST) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)) || !defined(DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES)
#include <list>
DOCTEST_STL_SIMPLE_CONTAINER(std::list)
#endif

#if DOCTEST_CPP >= 11
#if defined(DOCTEST_STL_STRINGIFY_FLAG_FORWARD_LIST) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <forward_list>
DOCTEST_STL_SIMPLE_CONTAINER(std::forward_list)
#endif
#endif

#define DOCTEST_STL_ADAPTER(temp, type, var, do) DOCTEST_STL_STRINGIFY_GEN(temp, type, internal) { \
    DOCTEST_STL_DEBRACE type var = internal; \
    String s = "["; \
    bool first = true; \
    while (!var.empty()) { \
        if (!first) { \
            s += ", "; \
        } \
        first = false; \
        s += do; \
    } \
    return s + "]"; \
}

#define DOCTEST_STL_SIMPLE_ADAPTER(name, top) DOCTEST_STL_ADAPTER((typename T, typename S), \
    (name<T, S>), adptr, toString(adptr.top()); adptr.pop())

#if defined(DOCTEST_STL_STRINGIFY_FLAG_STACK) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <stack>
DOCTEST_STL_SIMPLE_ADAPTER(std::stack, top)
#endif

#if defined(DOCTEST_STL_STRINGIFY_FLAG_QUEUE) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <queue>
DOCTEST_STL_SIMPLE_ADAPTER(std::queue, front)
DOCTEST_STL_ADAPTER((typename T, typename S, typename COMP),
    (std::priority_queue<T, S, COMP>), adptr, toString(adptr.top()); adptr.pop())
#endif

#if (defined(DOCTEST_STL_STRINGIFY_SET) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)) || !defined(DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES)
#include <set>
#define DOCTEST_STL_SET(type) DOCTEST_STL_CONTAINER((typename T, typename COMP, typename ALLOC), (type<T, COMP, ALLOC>), "{", "}")
DOCTEST_STL_SET(std::set)
DOCTEST_STL_SET(std::multiset)
#endif

#if DOCTEST_CPP >= 11
#if defined(DOCTEST_STL_STRINGIFY_UNORDERED_SET) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <unordered_set>
#define DOCTEST_STL_USET(type) DOCTEST_STL_CONTAINER((typename T, typename HASH, typename EQ, typename ALLOC), \
    (type<T, HASH, EQ, ALLOC>), "{", "}")
DOCTEST_STL_USET(std::unordered_set)
DOCTEST_STL_USET(std::unordered_multiset)
#endif
#endif

#if defined(DOCTEST_STL_STRINGIFY_MAP) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <map>
#define DOCTEST_STL_MAP(name) DOCTEST_STL_CONTAINER((typename T, typename S, typename COMP, typename ALLOC), (name<T, S, COMP, ALLOC>), "{", "}")
DOCTEST_STL_MAP(std::map)
DOCTEST_STL_MAP(std::multimap)
#endif

#if DOCTEST_CPP >= 11
#if (defined(DOCTEST_STL_STRINGIFY_UNORDERED_MAP) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)) || !defined(DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES)
#include <unordered_map>
#define DOCTEST_STL_UMAP(name) DOCTEST_STL_CONTAINER((typename T, typename S, typename HASH, typename EQ, typename ALLOC), \
    (name<T, S, HASH, EQ, ALLOC>), "{", "}")
DOCTEST_STL_UMAP(std::unordered_map)
DOCTEST_STL_UMAP(std::unordered_multimap)
#endif
#endif

#if defined(DOCTEST_STL_STRINGIFY_UTILITY) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <utility>
#if DOCTEST_CPP >= 14
DOCTEST_STL_NAMESPACES_BEGIN
template <typename T>
inline static void _appendInt(String&) { }

template <typename T, T I, T... INTS>
inline static void _appendInt(String& s) {
    s += toString(I);
    if (sizeof...(INTS) != 0) {
        s += ", ";
        _appendInt<T, INTS...>(s);
    }
}
DOCTEST_STL_NAMESPACES_END

DOCTEST_STL_STRINGIFY_GEN((typename T, T... INTS), (std::integer_sequence<T, INTS...>), ) {
    String nums;
    if (sizeof...(INTS) != 0) {
        DOCTEST_STL_DETAIL_NAMESPACE_NAME::_appendInt<T, INTS...>(nums);
    }
    return "[" + nums + "]";
}
#endif

DOCTEST_STL_STRINGIFY_GEN((typename T, typename S), (std::pair<T, S>), var) {
    return "(" + toString(var.first) + ", " + toString(var.second) + ")";
}
#endif

#if DOCTEST_CPP >= 11
#if defined(DOCTEST_STL_STRINGIFY_TUPLE) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <tuple>
DOCTEST_STL_NAMESPACES_BEGIN
template <std::size_t I = 0, typename... TYPES>
static typename std::enable_if<I == sizeof...(TYPES)>::type
inline _appendTupleIndexed(String&, const std::tuple<TYPES...>&) { }

template <std::size_t I = 0, typename... TYPES>
static typename std::enable_if<I < sizeof...(TYPES)>::type
inline _appendTupleIndexed(String& s, const std::tuple<TYPES...>& tuple) {
    s += toString(std::get<I>(tuple));
    if (I + 1 < sizeof...(TYPES)) {
        s += ", ";
    }
    _appendTupleIndexed<I + 1, TYPES...>(s, tuple);
}
DOCTEST_STL_NAMESPACES_END

DOCTEST_STL_STRINGIFY_GEN((typename... TYPES), (std::tuple<TYPES...>), var) {
    String data; DOCTEST_STL_DETAIL_NAMESPACE_NAME::_appendTupleIndexed(data, var);
    return "(" + data + ")";
}
#endif

#if defined(DOCTEST_STL_STRINGIFY_RATIO) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <ratio>
DOCTEST_STL_STRINGIFY_GEN((intmax_t NUM, intmax_t DEN), (std::ratio<NUM, DEN>), ) {
    return toString(NUM) + "/" + toString(DEN);
}
#endif
#endif

#if DOCTEST_CPP >= 17
#if defined(DOCTEST_STL_STRINGIFY_VARIANT) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <variant>
DOCTEST_STL_STRINGIFY_GEN((typename... T), (std::variant<T...>), value) {
    return std::visit([](auto&& val) { return toString(val); }, value);
}
DOCTEST_STL_STRINGIFY(std::monostate, ) {
    return "monostate";
}
#endif

#if defined(DOCTEST_STL_STRINGIFY_OPTIONAL) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <optional>
DOCTEST_STL_STRINGIFY_GEN((typename T), (std::optional<T>), var) {
    return var.has_value() ? toString(var.value()) : toString(std::nullopt);
}
DOCTEST_STL_STRINGIFY(std::nullopt_t, ) {
    return "nullopt";
}
#endif
#endif

#if defined(DOCTEST_STL_STRINGIFY_TYPE_INFO) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
#include <typeinfo>
DOCTEST_STL_STRINGIFY(std::type_info, value) {
    return String(value.name());
}
#endif

#if DOCTEST_CPP >= 11 && (DOCTEST_GCC == 0 || DOCTEST_GCC >= DOCTEST_COMPILER(5, 0, 0)) // gcc only supports put_time >= v5
#if defined(DOCTEST_STL_STRINGIFY_CHRONO) ^ defined(DOCTEST_STL_STRINGIFY_FLIP)
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4866)
#include <chrono>
DOCTEST_MSVC_SUPPRESS_WARNING_POP
#include <ostream>
#include <iomanip> // we don't *need* this, but I won't be writing my own time stringifier
DOCTEST_STL_STRINGIFY_GEN((typename CLOCK, typename DUR), (std::chrono::time_point<CLOCK, DUR>), value) {
    namespace stc = std::chrono;
    stc::system_clock::time_point sctp = stc::system_clock::now() + stc::duration_cast<stc::system_clock::duration>(value - CLOCK::now());
    time_t t = stc::system_clock::to_time_t(sctp);
    stc::system_clock::rep millis = stc::duration_cast<stc::milliseconds>(sctp.time_since_epoch()).count();
    tm tm;
#ifdef _MSC_VER
    if (localtime_s(&tm, &t) != 0) {
        return "FAILURE TO GET TIME";
    }
#else
    tm = *localtime(&t);
#endif
    std::ostream& ss = detail::tlssPush();
    ss << std::put_time(&tm, "%F %T") << '.' << std::setfill('0') << std::setw(3) << (millis % 1000) << " (local time)";
    return detail::tlssPop();
}
#endif
#endif

DOCTEST_GCC_SUPPRESS_WARNING_POP
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#endif // DOCTEST_STL_STRINGIFIER_H_INCLUDED
