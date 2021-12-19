#include "header.h"

TEST_SUITE("stl stringification C++14") {

using namespace std;

TEST_CASE("integer sequence stringifications") {
    FAIL_CHECK(integer_sequence<int, 2, 3, 5, 7>());
    FAIL_CHECK(integer_sequence<int>());
    FAIL_CHECK(make_index_sequence<3>());
}

}
