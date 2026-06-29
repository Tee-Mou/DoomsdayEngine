#include <catch2/catch_test_macros.hpp>

#include "../../inc/Utils.h"

TEST_CASE( "Count Trailing Zeroes", "[utils]") {
    REQUIRE(Utils::ctz(0) == 0);
    REQUIRE(Utils::ctz(1) == 0);
    REQUIRE(Utils::ctz(2) == 1);
    REQUIRE(Utils::ctz(128) == 7);
    REQUIRE(Utils::ctz(144) == 4);
    REQUIRE(Utils::ctz(4294967296) == 32);
}

TEST_CASE("Count Set Bits", "[utils]") {
    REQUIRE(Utils::csb(0) == 0);
    REQUIRE(Utils::csb(2) == 1);
    REQUIRE(Utils::csb(10) == 2);
    REQUIRE(Utils::csb(127) == 7);
    REQUIRE(Utils::csb(147) == 4);
    REQUIRE(Utils::csb(4294967296ULL) == 1);
}

TEST_CASE("Verify Seeded Generation", "[utils]") {
    REQUIRE(Utils::gen64(100) == Utils::gen64(100));
    REQUIRE(Utils::gen64(100) == 13845132144816671316ULL);
}
