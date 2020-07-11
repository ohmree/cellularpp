#include "automata/game_of_life.hpp"

#include <SDL2/SDL.h>

#include <array>
#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <thread>
#include <utility>

#include "cellular.hpp"

constexpr const SDL_Color YELLOW = {255, 255, 0, 255};
constexpr const SDL_Color BLACK  = {0, 0, 0, 255};
constexpr const SDL_Color WHITE  = {255, 255, 255, 255};

using namespace cellular;
using namespace gol;

// First state is the default one
GameOfLife::GameOfLife(const size_t width, const size_t height) :
    Automaton<State>(width, height) {}

const SDL_Color GameOfLife::state_to_color(State state) const {
	return state == State::Alive ? YELLOW : WHITE;
}

char GameOfLife::state_to_char(State state) const {
	return state == State::Alive ? '#' : '*';
}

// TODO: maybe add an invalid state?
// Alternatively, we can raise an exception.
State GameOfLife::char_to_state(char c) const {
	switch (c) {
	case '#':
		return State::Alive;
		break;
	case '*':
		return State::Dead;
		break;
	default:
		throw std::invalid_argument("Invalid state value: ");
		break;
	}
}

State GameOfLife::next_state(const State& current_cell,
                             const size_t x,
                             const size_t y) {
	const auto&  neighborhood = moore_neighborhood_at(x, y);
	unsigned int live_neighbors =
	    std::count_if(std::begin(neighborhood),
	                  std::end(neighborhood),
	                  [](std::pair<const char*, State> const& p) {
		                  return p.second == State::Alive;
	                  });
	State next_generation = current_cell;
	switch (current_cell) {
	case State::Alive:
		if (!(live_neighbors == 2 || live_neighbors == 3))
			next_generation = State::Dead;
		break;
	case State::Dead:
		if (live_neighbors == 3) next_generation = State::Alive;
		break;
	}

	return next_generation;
}
