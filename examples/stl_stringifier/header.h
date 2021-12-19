#ifndef DOCTEST_STL_STRINGIFY_TESTS_UTILITY_HEADER_INCLUDED
#define DOCTEST_STL_STRINGIFY_TESTS_UTILITY_HEADER_INCLUDED

#define DOCTEST_STL_STRINGIFY_FLIP
#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#include <doctest/extensions/stl_stringifier.h>

#define TEST_ARRAY_TYPE_IMPL(type, temp, size, comp) \
TEST_CASE(#type " stringifications") { \
    comp((type<int DOCTEST_STL_DEBRACE size>{ 1, 2, 3 })); \
    comp((type DOCTEST_STL_DEBRACE temp{ })); \
}

#define TEST_DEBRACE_FAIL_CHECK(val) FAIL_CHECK(DOCTEST_STL_DEBRACE val)

#define TEST_ARRAY_TYPE(type) TEST_ARRAY_TYPE_IMPL(type, (<int>), (), TEST_DEBRACE_FAIL_CHECK)

#define TEST_MAP(type, comp) \
TEST_CASE(#type " stringifications") { \
    comp((type<int, const char*>{ { 1, "one" }, { 2, "two" }, { 3, "three" }})); \
    FAIL_CHECK(type<int, const char*>()); /* Empty is always {} */ \
}

#endif // DOCTEST_STL_STRINGIFY_TESTS_UTILITY_HEADER_INCLUDED
