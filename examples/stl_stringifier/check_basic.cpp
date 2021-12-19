#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "header.h"

TEST_SUITE("stl stringification basic") {

using namespace std;

TEST_ARRAY_TYPE(valarray)
TEST_ARRAY_TYPE(vector)
TEST_ARRAY_TYPE(deque)
TEST_ARRAY_TYPE(list)
TEST_ARRAY_TYPE(set)
TEST_ARRAY_TYPE(multiset)

#define TEST_ARRAY_ADAPTER_IMPL(type, underlying, extra) \
TEST_CASE(#type " stringifications " #underlying) { \
    type<int, underlying<int>> t; \
    for (int i = 0; i < 3; i++) { \
        t.push(i); \
    } \
    FAIL_CHECK(t); \
    FAIL_CHECK(type<int, underlying<int>>()); \
 \
    DOCTEST_STL_DEBRACE extra \
}

#define TEST_ARRAY_ADAPTER(type, underlying) \
    TEST_ARRAY_ADAPTER_IMPL(type, underlying, (\
        underlying<int> ints{ 1, 2, 3 }; \
        FAIL_CHECK(type<int, underlying<int>>(ints)); \
))

TEST_ARRAY_ADAPTER(stack, deque)
TEST_ARRAY_ADAPTER(stack, vector)
TEST_ARRAY_ADAPTER(stack, list)
TEST_ARRAY_ADAPTER(queue, deque)
TEST_ARRAY_ADAPTER(queue, list)
TEST_ARRAY_ADAPTER_IMPL(priority_queue, deque, ())
DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wstrict-overflow") // genuinely no clue
TEST_ARRAY_ADAPTER_IMPL(priority_queue, vector, ())
DOCTEST_GCC_SUPPRESS_WARNING_POP

TEST_MAP(map, TEST_DEBRACE_FAIL_CHECK)
TEST_MAP(multimap, TEST_DEBRACE_FAIL_CHECK)

TEST_CASE("pair stringifications") {
    FAIL_CHECK(pair<int, const char*>());
    FAIL_CHECK(pair<int, const char*>{ 1, "one" });
}

#if defined(__clang__)
    #if __has_feature(cxx_rtti)
        #define TEST_HAS_RTTI
    #endif
#elif defined(__GNUC__) && defined(__GXX_RTTI) \
   || defined(_MSC_VER) && defined(_CPPRTTI)
    #define TEST_HAS_RTTI
#endif
TEST_CASE("typeinfo stringifications") {
#ifdef TEST_HAS_RTTI
    struct Test { };
    CHECK(doctest::toString(typeid(Test)) != "{?}");
#else
    CHECK(true);
#endif
}

}
