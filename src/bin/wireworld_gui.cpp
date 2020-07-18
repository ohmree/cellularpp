// Grid drawing code generously lent to me by catsocks.
// I then heavily modified it to suit my needs.
// https://github.com/catsocks/sdl-grid/
// Thanks a bunch!
#include <SDL2/SDL.h>

#include <string>

#include "cellular_gui.hpp"
#include "draw_grid.hpp"
#include "wireworld.hpp"

constexpr const SDL_Color BLACK  = {0, 0, 0, 255};
constexpr const SDL_Color WHITE  = {255, 255, 255, 255};
constexpr const SDL_Color RED    = {255, 0, 0, 255};
constexpr const SDL_Color BLUE   = {0, 0, 255, 255};
constexpr const SDL_Color YELLOW = {255, 255, 0, 255};

using namespace wireworld;
using namespace cellular_gui;

SDL_Color state_to_color(State state) {
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

int main(int argc, char** argv) {
	Wireworld wireworld(20, 20);

	if (argc >= 2) {
		std::string file_path = argv[1];
		wireworld.set_grid_from_file(file_path);
	}

	return draw_grid(wireworld, ColorFunctor<State>(state_to_color));
}
