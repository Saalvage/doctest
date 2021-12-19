#include "header.h"

TEST_SUITE("stl stringification C++11") {

using namespace std;

TEST_ARRAY_TYPE_IMPL(array, (<int, 0>), (, 3))
TEST_ARRAY_TYPE(initializer_list)
TEST_ARRAY_TYPE(forward_list)
TEST_ARRAY_TYPE(unordered_set)
TEST_ARRAY_TYPE(unordered_multiset)

TEST_MAP(unordered_map)
TEST_MAP(unordered_multimap)

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

}
