#include "header.h"

TEST_SUITE("stl stringification C++17") {

using namespace std;

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

}
