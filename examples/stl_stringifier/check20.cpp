#include "header.h"

TEST_SUITE("stl stringification C++20") {

using namespace std;

TEST_CASE("span stringifications") {
    int ints[] = { 1, 2, 3 };
    FAIL_CHECK(span(ints));
    FAIL_CHECK(span<int>());
}

}
