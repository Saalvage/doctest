#ifndef DOCTEST_STL_STRINGIFIER_H_INCLUDED
#define DOCTEST_STL_STRINGIFIER_H_INCLUDED

#ifndef DOCTEST_LIBRARY_INCLUDED
#include "../doctest.h"
#endif

#define DOCTEST_STL_NAMESPACE namespace doctest

#define DOCTEST_STL_NAMESPACES_BEGIN DOCTEST_STL_NAMESPACE { namespace detail {
#define DOCTEST_STL_NAMESPACES_END } }


#define DOCTEST_STL_DEBRACE(...) __VA_ARGS__

#define DOCTEST_STL_STRINGIFY_GEN_IMPL(temp, type, func, varName) \
template <DOCTEST_STL_DEBRACE temp> \
String func(const DOCTEST_STL_DEBRACE type&); \
template <DOCTEST_STL_DEBRACE temp> \
struct StringMaker<DOCTEST_STL_DEBRACE type> { \
    static String convert(const DOCTEST_STL_DEBRACE type& value) { return detail::func(value); } \
}; \
template <DOCTEST_STL_DEBRACE temp> \
String func(const DOCTEST_STL_DEBRACE type& varName)

#define DOCTEST_STL_STRINGIFY_GEN(temp, type, varName) DOCTEST_STL_STRINGIFY_GEN_IMPL(temp, type, DOCTEST_ANONYMOUS(_), varName)

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

#if defined(DOCTEST_STL_STRINGIFY_FLAG_ARRAY) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <array>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_ARRAY((typename T, size_t SIZE), (std::array<T, SIZE>))
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_FLAG_SPAN) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <span>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_ARRAY((typename T, size_t SIZE), (std::span<T, SIZE>))
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_FLAG_VALARRAY) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <valarray>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_ARRAY((typename T), (std::valarray<T>))
DOCTEST_STL_NAMESPACES_END
#endif

#if (defined(DOCTEST_STL_STRINGIFY_FLAG_INITIALIZER_LIST) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)) || !defined(DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES)
#include <initializer_list>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_ARRAY((typename T), (std::initializer_list<T>))
DOCTEST_STL_NAMESPACES_END
#endif

#define DOCTEST_STL_SIMPLE_CONTAINER(type) DOCTEST_STL_CONTAINER((typename T, typename ALLOC), (type<T, ALLOC>), "[", "]")

#if (defined(DOCTEST_STL_STRINGIFY_FLAG_VECTOR) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)) || !defined(DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES)
#include <vector>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_SIMPLE_CONTAINER(std::vector)
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_FLAG_DEQUE) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <deque>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_SIMPLE_CONTAINER(std::deque)
DOCTEST_STL_NAMESPACES_END
#endif

#if (defined(DOCTEST_STL_STRINGIFY_FLAG_LIST) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)) || !defined(DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES)
#include <list>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_SIMPLE_CONTAINER(std::list)
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_FLAG_FORWARD_LIST) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <forward_list>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_SIMPLE_CONTAINER(std::forward_list)
DOCTEST_STL_NAMESPACES_END
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
    (name<T, S>), adptr, toString(adptr.top()); adptr.pop());

#if defined(DOCTEST_STL_STRINGIFY_FLAG_STACK) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <stack>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_SIMPLE_ADAPTER(std::stack, top)
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_FLAG_QUEUE) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <queue>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_SIMPLE_ADAPTER(std::queue, front)
DOCTEST_STL_ADAPTER((typename T, typename S, typename COMP),
    (std::priority_queue<T, S, COMP>), adptr, toString(adptr.top()); adptr.pop());
DOCTEST_STL_NAMESPACES_END
#endif

#if (defined(DOCTEST_STL_STRINGIFY_SET) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)) || !defined(DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES)
#include <set>
#define DOCTEST_STL_SET(type) DOCTEST_STL_CONTAINER((typename T, typename COMP, typename ALLOC), (type<T, COMP, ALLOC>), "{", "}")
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_SET(std::set)
DOCTEST_STL_SET(std::multiset)
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_UNORDERED_SET) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <unordered_set>
#define DOCTEST_STL_USET(type) DOCTEST_STL_CONTAINER((typename T, typename HASH, typename EQ, typename ALLOC), \
    (type<T, HASH, EQ, ALLOC>), "{", "}")
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_USET(std::unordered_set)
DOCTEST_STL_USET(std::unordered_multiset)
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_MAP) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <map>
#define DOCTEST_STL_MAP(name) DOCTEST_STL_CONTAINER((typename T, typename S, typename COMP, typename ALLOC), (name<T, S, COMP, ALLOC>), "{", "}")
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_MAP(std::map)
DOCTEST_STL_MAP(std::multimap)
DOCTEST_STL_NAMESPACES_END
#endif

#if (defined(DOCTEST_STL_STRINGIFY_UNORDERED_MAP) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)) || !defined(DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES)
#include <unordered_map>
#define DOCTEST_STL_UMAP(name) DOCTEST_STL_CONTAINER((typename T, typename S, typename HASH, typename EQ, typename ALLOC), \
    (name<T, S, HASH, EQ, ALLOC>), "{", "}")
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_UMAP(std::unordered_map)
DOCTEST_STL_UMAP(std::unordered_multimap)
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_UTILITY) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <utility>
DOCTEST_STL_NAMESPACES_BEGIN
template <typename T>
static void appendInt(String& s) { }

template <typename T, T I, T... INTS>
static void appendInt(String& s) {
    s += toString(I);
    if (sizeof...(INTS) != 0) {
        s += ", ";
        appendInt<T, INTS...>(s);
    }
}

DOCTEST_STL_STRINGIFY_GEN((typename T, T... INTS), (std::integer_sequence<T, INTS...>), var) {
    String nums;
    if (sizeof...(INTS) != 0) {
        appendInt<T, INTS...>(nums);
    }
    return "[" + nums + "]";
}

DOCTEST_STL_STRINGIFY_GEN((typename T, typename S), (std::pair<T, S>), var) {
    return "(" + toString(var.first) + ", " + toString(var.second) + ")";
}
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_TUPLE) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <tuple>
DOCTEST_STL_NAMESPACES_BEGIN
template <size_t I = 0, typename... TYPES>
static std::enable_if<I == sizeof...(TYPES)>::type
appendTupleIndexed(String& s, const std::tuple<TYPES...>& tuple) { }

template <size_t I = 0, typename... TYPES>
static std::enable_if < I < sizeof...(TYPES)>::type
    appendTupleIndexed(String& s, const std::tuple<TYPES...>& tuple) {
    s += toString(std::get<I>(tuple));
    if (I + 1 < sizeof...(TYPES)) {
        s += ", ";
    }
    appendTupleIndexed<I + 1, TYPES...>(s, tuple);
}

DOCTEST_STL_STRINGIFY_GEN((typename... TYPES), (std::tuple<TYPES...>), var) {
    String data; appendTupleIndexed(data, var);
    return "(" + data + ")";
}
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_RATIO) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <ratio>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_STRINGIFY_GEN((std::intmax_t NUM, std::intmax_t DEN), (std::ratio<NUM, DEN>), var) {
    return toString(NUM) + "/" + toString(DEN);
}
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_UNORDERED_VARIANT) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <variant>
DOCTEST_STL_NAMESPACES_BEGIN
DOCTEST_STL_STRINGIFY_GEN((typename... T), (std::variant<T...>), value) {
    return "(" + std::visit([](auto&& val) { return toString(val); }, value) + ")";
}
DOCTEST_STL_NAMESPACES_END
#endif

#if defined(DOCTEST_STL_STRINGIFY_UNORDERED_TYPE_INFO) ^ defined(DOCTEST_STL_STRIGIFY_FLIP)
#include <typeinfo>
template <> struct doctest::StringMaker<std::type_info> {
    static String convert(const std::type_info& value) {
        return String(value.name());
    }
};
#endif


#undef DOCTEST_STL_NAMESPACE

#undef DOCTEST_STL_NAMESPACES_BEGIN
#undef DOCTEST_STL_NAMESPACES_END

#undef DOCTEST_STL_DEBRACE

#undef DOCTEST_STL_STRINGIFY_GEN_IMPL
#undef DOCTEST_STL_STRINGIFY_GEN

#undef DOCTEST_STL_CONTAINER

#undef DOCTEST_STL_ARRAY
#undef DOCTEST_STL_SIMPLE_CONTAINER

#undef DOCTEST_STL_ADAPTER
#undef DOCTEST_STL_SIMPLE_ADAPTER

#undef DOCTEST_STL_SET
#undef DOCTEST_STL_USET

#undef DOCTEST_STL_MAP
#undef DOCTEST_STL_UMAP

#endif // DOCTEST_STL_STRINGIFIER_H_INCLUDED
