#include "automata/wireworld.hpp"

#include <SDL2/SDL.h>

#include <stdexcept>

#include "cellular.hpp"
#include "cellular_gui.hpp"

constexpr const SDL_Color YELLOW = {255, 255, 0, 255};
constexpr const SDL_Color WHITE  = {255, 255, 255, 255};
constexpr const SDL_Color BLACK  = {0, 0, 0, 255};
constexpr const SDL_Color RED    = {255, 0, 0, 255};
constexpr const SDL_Color BLUE   = {0, 0, 255, 255};

using namespace cellular;
using namespace wireworld;

// First state is the default one
WireWorld::WireWorld(const size_t width, const size_t height) :
    GuiAutomaton<State>(width, height) {}

const SDL_Color WireWorld::state_to_color(State state) const {
	SDL_Color ret = BLACK;
	switch (state) {
	case State::Empty: {
		ret = WHITE;
		break;
	}
	case State::ElectronHead: {
		ret = BLUE;
		break;
	}
	case State::ElectronTail: {
		ret = RED;
		break;
	}
	case State::Conductor:
		ret = YELLOW;
		break;
	}

	return ret;
}

char WireWorld::state_to_char(State state) const {
	char ret = ' ';
	switch (state) {
	case State::Empty: {
		ret = ' ';
		break;
	}
	case State::ElectronHead: {
		ret = '*';
		break;
	}
	case State::ElectronTail: {
		ret = 'o';
		break;
	}
	case State::Conductor:
		ret = '#';
		break;
	}

	return ret;
}

State WireWorld::char_to_state(char c) const {
	switch (c) {
	case ' ': {
		return State::Empty;
		break;
	}

	case '*': {
		return State::ElectronHead;
		break;
	}

	case 'o': {
		return State::ElectronTail;
		break;
	}

	case '#': {
		return State::Conductor;
		break;
	}

	default: {
		throw std::invalid_argument(std::string{"Invalid state value: "} + c);
		break;
	}
	}
}

State WireWorld::next_state(const State& current_cell,
                            const size_t x,
                            const size_t y) {
	State ret = State::Empty;
	switch (current_cell) {
	case State::Empty: {
		break;
	}
	case State::ElectronHead: {
		ret = State::ElectronTail;
		break;
	}
	case State::ElectronTail: {
		ret = State::Conductor;
		break;
	}
	case State::Conductor: {
		const auto&  neighborhood = moore_neighborhood_at(x, y);
		unsigned int nearby_heads =
		    std::count_if(std::begin(neighborhood),
		                  std::end(neighborhood),
		                  [](std::pair<const char*, State> const& p) {
			                  return p.second == State::ElectronHead;
		                  });
		if (nearby_heads == 1 || nearby_heads == 2) {
			ret = State::ElectronHead;
		} else {
			ret = State::Conductor;
		}
		break;
	}
	}

	return ret;
}
