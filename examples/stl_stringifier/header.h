#define DOCTEST_STL_STRINGIFY_FLIP
#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#include <doctest/extensions/stl_stringifier.h>

#define TEST_ARRAY_TYPE_IMPL(type, temp, size) \
TEST_CASE(#type " stringifications") { \
    FAIL_CHECK(type<int DOCTEST_STL_DEBRACE size>{ 1, 2, 3 }); \
    FAIL_CHECK(type DOCTEST_STL_DEBRACE temp{ }); \
}

#define TEST_ARRAY_TYPE(type) TEST_ARRAY_TYPE_IMPL(type, (<int>), ())

#define TEST_MAP(type) \
TEST_CASE(#type " stringifications") { \
    FAIL_CHECK(type<int, const char*>{ { 1, "one" }, { 2, "two" }, { 3, "three" }}); \
    FAIL_CHECK(type<int, const char*>()); \
}
