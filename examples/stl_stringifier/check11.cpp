#include "header.h"

TEST_SUITE("stl stringification C++11") {

using namespace std;

TEST_CASE("array stringifications") {
    FAIL_CHECK(array<int, 3>{ 1, 2, 3 });
    FAIL_CHECK(array<int, 0>{ });
}

TEST_CASE("initializer_list stringifications") {
    FAIL_CHECK(initializer_list<int>{ 1, 2, 3 });
    FAIL_CHECK((initializer_list<int>{ }));
}

TEST_ARRAY_TYPE(forward_list)

#define TEST_UNORDERED_COMP(val) CHECK(doctest::toString(DOCTEST_STL_DEBRACE val) != "{?}")
#define TEST_UNORDERED_SET(type) TEST_ARRAY_TYPE_IMPL(unordered_set, TEST_UNORDERED_COMP)

TEST_UNORDERED_SET(unordered_set)
TEST_UNORDERED_SET(unordered_multiset)

TEST_MAP(unordered_map, TEST_UNORDERED_COMP)
TEST_MAP(unordered_multimap, TEST_UNORDERED_COMP)

TEST_CASE("tuple stringifications") {
    FAIL_CHECK(tuple<>());
    using TestTuple = tuple<int, const char*, bool, bool, bool>;
    FAIL_CHECK(TestTuple());
    FAIL_CHECK(TestTuple{ 1, "one", false, true, true });
}

TEST_CASE("ratio stringifications") {
    FAIL_CHECK(ratio<1, 1000000000000000000>());
    FAIL_CHECK(ratio<0, 1>());
}

using namespace chrono;
TEST_CASE_TEMPLATE("chrono time_point stringification",
    T, system_clock, high_resolution_clock, steady_clock) {
#if !defined(__GNUC__) || __GNUC__ >= 5
    CHECK(doctest::toString(time_point<T>()) != "{?}");
    CHECK(doctest::toString(T::now()) != "{?}");
#else
    CHECK(true);
    CHECK(true);
#endif
}

}
