// Grid drawing code generously lent to me by catsocks.
// I then heavily modified it to suit my needs.
// https://github.com/catsocks/sdl-grid/
// Thanks a bunch!
#include <SDL2/SDL.h>

#include <iostream>

#include "draw_grid.hpp"
#include "game_of_life.hpp"

constexpr const SDL_Color YELLOW = {255, 255, 0, 255};
constexpr const SDL_Color WHITE  = {255, 255, 255, 255};

using namespace gol;
using namespace cellular_gui;

SDL_Color state_to_color(State state) {
	return state == State::Alive ? YELLOW : WHITE;
}

int main(int argc, char** argv) {
	GameOfLife gol(20, 20);

	if (argc >= 2) {
		std::string file_path = argv[1];
		gol.set_grid_from_file(file_path);
	}

	return draw_grid(gol, ColorFunctor<State>(state_to_color));
}
