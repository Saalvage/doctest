#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_STL_STRINGIFY_FLIP
#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#include <doctest/extensions/stl_stringifier.h>

TEST_SUITE("stl stringification") {

using namespace std;

#define TEST_ARRAY_TYPE_IMPL(type, temp) \
TEST_CASE(#type " stringifications") { \
    FAIL_CHECK(type{ 1, 2, 3 }); \
    FAIL_CHECK(type DOCTEST_STL_DEBRACE temp{ }); \
}

#define TEST_ARRAY_TYPE(type) TEST_ARRAY_TYPE_IMPL(type, (<int>))

TEST_ARRAY_TYPE_IMPL(array, (<int, 0>))
TEST_ARRAY_TYPE(valarray)
TEST_ARRAY_TYPE(initializer_list)
TEST_ARRAY_TYPE(vector)
TEST_ARRAY_TYPE(deque)
TEST_ARRAY_TYPE(list)
TEST_ARRAY_TYPE(forward_list)
TEST_ARRAY_TYPE(set)
TEST_ARRAY_TYPE(multiset)
TEST_ARRAY_TYPE(unordered_set)
TEST_ARRAY_TYPE(unordered_multiset)

#if DOCTEST_CPP >= 20
TEST_CASE("span stringifications") {
    int ints[] = { 1, 2, 3 };
    FAIL_CHECK(span(ints));
    FAIL_CHECK(span<int>());
}
#endif

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
        underlying ints{ 1, 2, 3 }; \
        FAIL_CHECK(type(ints)); \
))

TEST_ARRAY_ADAPTER(stack, deque)
TEST_ARRAY_ADAPTER(stack, vector)
TEST_ARRAY_ADAPTER(stack, list)
TEST_ARRAY_ADAPTER(queue, deque)
TEST_ARRAY_ADAPTER(queue, list)
TEST_ARRAY_ADAPTER_IMPL(priority_queue, deque, ())
TEST_ARRAY_ADAPTER_IMPL(priority_queue, vector, ())

#define TEST_MAP(type) \
TEST_CASE(#type " stringifications") { \
    FAIL_CHECK(type<int, const char*>{ { 1, "one" }, { 2, "two" }, { 3, "three" }}); \
    FAIL_CHECK(type<int, const char*>()); \
}

TEST_MAP(map)
TEST_MAP(multimap)
TEST_MAP(unordered_map)
TEST_MAP(unordered_multimap)

TEST_CASE("integer sequence stringifications") {
    FAIL_CHECK(integer_sequence<int, 1, 2, 3, 4>());
    FAIL_CHECK(integer_sequence<int>());
    FAIL_CHECK(make_index_sequence<42>());
}

TEST_CASE("pair stringifications") {
    FAIL_CHECK(pair<int, const char*>());
    FAIL_CHECK(pair{ 1, "one" });
}

TEST_CASE("tuple stringifications") {
    FAIL_CHECK(pair<int, const char*>());
    FAIL_CHECK(pair{ 1, "one" });
}

TEST_CASE("ratio stringifications") {
    FAIL_CHECK(ratio<1, 1000000000000000000>());
    FAIL_CHECK(ratio<0, 1>());
}

TEST_CASE("variant stringifications") {
    using MyVariant = variant<bool, int, const char*, std::vector<int>>;
    FAIL_CHECK(MyVariant{ "hello" });
    FAIL_CHECK(MyVariant{ });
    FAIL_CHECK(variant<monostate>());
    FAIL_CHECK(monostate());
}

TEST_CASE("optional stringifications") {
    FAIL_CHECK(optional<int>(5));
    FAIL_CHECK(optional<int>());
    FAIL_CHECK(nullopt);
}

TEST_CASE("typeinfo stringifications") {
    CHECK(doctest::toString(typeid(monostate)) != "{?}");
}

TEST_CASE("chrono time_point stringification") {
    using namespace chrono;
    FAIL_CHECK(time_point<system_clock>());
}

}
