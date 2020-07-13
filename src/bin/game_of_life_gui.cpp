// Grid drawing code generously lent to me by catsocks.
// I then heavily modified it to suit my needs.
// https://github.com/catsocks/sdl-grid/
// Thanks a bunch!
#include <SDL2/SDL.h>

#include <string>

#include "automata/game_of_life.hpp"
#include "cellular_gui.hpp"

constexpr const SDL_Color BLACK = {0, 0, 0, 255};
constexpr const SDL_Color WHITE = {255, 255, 255, 255};

using namespace gol;

int main(int argc, char** argv) {
	constexpr const int grid_size = 20;
	GameOfLife          gol(grid_size, grid_size);
	constexpr const int cell_size = 36;

	if (argc >= 2) {
		std::string file_path = argv[1];
		gol.set_grid_from_file(file_path);
	}

	// + 1 so that the last grid lines fit in the screen.
	int window_width  = (gol.width() * cell_size) + 1;
	int window_height = (gol.height() * cell_size) + 1;

	const SDL_Color BACKGROUND_COLOR = WHITE;
	SDL_Color       LINE_COLOR       = BLACK;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		             "SDL_Init error: %s",
		             SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window*   window;
	SDL_Renderer* renderer;
	if (SDL_CreateWindowAndRenderer(window_width,
	                                window_height,
	                                0,
	                                &window,
	                                &renderer)
	    < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		             "SDL_CreateWindowAndRenderer error: %s",
		             SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_SetWindowTitle(window, "SDL Grid");

	int   x;
	int   y;
	bool  is_closed = false;
	State current;
	while (!is_closed) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN: {
				x       = (event.motion.x / cell_size);
				y       = (event.motion.y / cell_size);
				current = gol(x, y);
				gol(x, y) =
				    current == State::Alive ? State::Dead : State::Alive;
				break;
			}

			case SDL_KEYDOWN: {
				gol.step();
				break;
			}
			case SDL_QUIT: {
				is_closed = true;
				break;
			}
			}
		}

		// Draw grid background.
		SDL_SetRenderDrawColor(renderer,
		                       BACKGROUND_COLOR.r,
		                       BACKGROUND_COLOR.g,
		                       BACKGROUND_COLOR.b,
		                       BACKGROUND_COLOR.a);
		SDL_RenderClear(renderer);

		// Draw grid lines.
		SDL_SetRenderDrawColor(renderer,
		                       LINE_COLOR.r,
		                       LINE_COLOR.g,
		                       LINE_COLOR.b,
		                       LINE_COLOR.a);

		for (int x = 0; x < 1 + grid_size * cell_size; x += cell_size) {
			SDL_RenderDrawLine(renderer, x, 0, x, window_height);
		}

		for (int y = 0; y < 1 + grid_size * cell_size; y += cell_size) {
			SDL_RenderDrawLine(renderer, 0, y, window_width, y);
		}

		// Draw cells.
		for (int i = 0; i <= grid_size * cell_size; i += cell_size) {
			for (int j = 0; j <= grid_size * cell_size; j += cell_size) {
				int      x                 = i / cell_size;
				int      y                 = j / cell_size;
				SDL_Rect current_cell_rect = {
				    .x = i + 1,
				    .y = j + 1,
				    .w = cell_size - 1,
				    .h = cell_size - 1,
				};
				SDL_Color color = gol.state_to_color(gol(x, y));
				SDL_SetRenderDrawColor(renderer,
				                       color.r,
				                       color.g,
				                       color.b,
				                       color.a);
				SDL_RenderFillRect(renderer, &current_cell_rect);
			}
		}

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
