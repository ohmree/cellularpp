#include "wireworld.hpp"

#include <filesystem>
#include <stdexcept>

#include "cellular.hpp"

using namespace cellular;
using namespace wireworld;

// First state is the default one
Wireworld::Wireworld(const size_t width, const size_t height) :
    Automaton<State>(width, height) {}

Wireworld::Wireworld(const std::filesystem::path& filename) :
    Automaton<State>(filename) {}

char Wireworld::state_to_char(State state) const {
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

State Wireworld::char_to_state(char c) const {
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

State Wireworld::next_state(const State  current_cell,
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

State Wireworld::cycle_state(const State current_cell) const {
	static const std::unordered_map<State, State> m{
	    {State::Empty, State::Conductor},
	    {State::Conductor, State::ElectronHead},
	    {State::ElectronHead, State::ElectronTail},
	    {State::ElectronTail, State::Empty}};

	return m.at(current_cell);
}
