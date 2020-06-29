#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"
#include "cellular.cpp"

TEST_CASE("S::i returns m_i", "[S]") {
    REQUIRE(S(5).i() == 5);
    REQUIRE(S(6).i() == 6);
    REQUIRE(S(1).i() == 1);
}
