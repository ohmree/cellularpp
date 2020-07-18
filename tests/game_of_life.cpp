#include "game_of_life.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>

#include "doctest.h"

using namespace gol;

GameOfLife automaton("./spinner.txt");

TEST_CASE("generation 0") {
	// for (auto i = 0; i < automaton.width(); ++i) {
	// 	for (auto j = 0; j < automaton.height(); ++j) {
	// 		INFO(automaton.state_to_char(automaton(i, j)));
	// 	}
	// 	INFO('\n');
	// }
	CHECK(automaton(0, 0) == State::Dead);
	// CHECK(automaton(2, 1) == State::Alive);
}

// TEST_CASE("generation 1") {
// 	automaton.step();
// 	CHECK(automaton(0, 0) == State::Dead);
// 	CHECK(automaton(2, 1) == State::Dead);
// 	CHECK(automaton(1, 2) == State::Alive);
// }
