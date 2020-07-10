#include "cellular.hpp"
// #include "pprint.hpp"

#include <SDL2/SDL.h>

#include <array>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <utility>

using namespace cellular;

constexpr const SDL_Color YELLOW = {255, 255, 0, 255};
constexpr const SDL_Color BLACK  = {0, 0, 0, 255};
constexpr const SDL_Color WHITE  = {255, 255, 255, 255};

// First state is the default one
enum class State { Dead, Alive };

static pprint::PrettyPrinter pp;

class GameOfLife : public Automaton<State> {
   public:
	GameOfLife(const size_t width, const size_t height) :
	    Automaton<State>(width, height) {}
	const SDL_Color state_to_color(State state) const {
		return state == State::Alive ? YELLOW : WHITE;
	}

   protected:
	State next_state(const State &current_cell,
	                 const size_t x,
	                 const size_t y) override {
		const auto & neighborhood = moore_neighborhood_at(x, y);
		unsigned int live_neighbors =
		    std::count_if(std::begin(neighborhood),
		                  std::end(neighborhood),
		                  [](std::pair<const char *, State> const &p) {
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

	char state_to_char(State state) const override {
		return state == State::Alive ? '#' : '*';
	}

	State char_to_state(char c) const override {
		switch (c) {
		case '#':
			return State::Alive;
			break;
		case '*':
			return State::Dead;
			break;
		default:
			return State::Dead;
			break;
		}
	}
};

constexpr const int   SCREEN_WIDTH  = 640;
constexpr const int   SCREEN_HEIGHT = 640;
constexpr const char *SPINNER_PATH =
    "/home/animo/code/cellular++/tests/spinner.txt";

int main(int argc, char **argv) {
	// FIXME: this seems to add extra living cells.
	// TODO: find out why????
	// GameOfLife gol(3, 3);
	// gol(1, 0) = State::Alive;
	// gol(1, 1) = State::Alive;
	// gol(1, 2) = State::Alive;

	// FIXME: this seems to result in an empty grid.
	// TODO: set up proper unit tests to pinpoint the bug
	// gol.set_grid_from_file(SPINNER_PATH);

	// GameOfLife gol(5, 5);

	// gol(2, 1) = State::Alive;
	// gol(2, 2) = State::Alive;
	// gol(2, 3) = State::Alive;
	int        grid_size = 10;
	GameOfLife gol(grid_size, grid_size);
	gol(2, 1)     = State::Alive;
	gol(2, 2)     = State::Alive;
	gol(2, 3)     = State::Alive;
	int cell_size = 36;

	// + 1 so that the last grid lines fit in the screen.
	int window_width  = (grid_size * cell_size) + 1;
	int window_height = (grid_size * cell_size) + 1;

	SDL_Color grid_background   = WHITE;
	SDL_Color grid_line_color   = {0, 0, 0, 255};
	SDL_Color grid_cursor_color = YELLOW;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		             "SDL_Init error: %s",
		             SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window *  window;
	SDL_Renderer *renderer;
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

	bool is_closed = false;

	while (!is_closed) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				SDL_Log("x: %d, y: %d\n", event.motion.x, event.motion.y);
				gol(event.motion.x, event.motion.y) = State::Alive;
				break;
			case SDL_QUIT:
				is_closed = true;
				break;
			}
		}

		// Draw grid background.
		SDL_SetRenderDrawColor(renderer,
		                       grid_background.r,
		                       grid_background.g,
		                       grid_background.b,
		                       grid_background.a);
		SDL_RenderClear(renderer);

		// Draw grid lines.
		SDL_SetRenderDrawColor(renderer,
		                       grid_line_color.r,
		                       grid_line_color.g,
		                       grid_line_color.b,
		                       grid_line_color.a);

		for (int x = 0; x < 1 + grid_size * cell_size; x += cell_size) {
			SDL_RenderDrawLine(renderer, x, 0, x, window_height);
		}

		for (int y = 0; y < 1 + grid_size * cell_size; y += cell_size) {
			SDL_RenderDrawLine(renderer, 0, y, window_width, y);
		}

		// Draw cells.
		for (int i = 0; i < 1 + grid_size * cell_size; i += cell_size) {
			for (int j = 0; j < 1 + grid_size * cell_size; j += cell_size) {
				SDL_Rect current_cell_rect = {
				    .x = i + 1,
				    .y = j + 1,
				    .w = cell_size - 1,
				    .h = cell_size - 1,
				};
				SDL_Color color = gol.state_to_color(gol(i, j));
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
