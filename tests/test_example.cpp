#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"
#include "cellular.cpp"
// TODO: Actually implement cellular automata tests.
// Perhaps a test that makes sure an automaton's grid is the same as any given
// grid.

// FIXME: This doesn't seem to be recognized and ran as a test by Meson.
// Who's at fault here?
TEST_CASE("Testing Catch2", "[catch2]") {
    REQUIRE(true == true);
    REQUIRE(false != true);
}
