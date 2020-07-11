#include <SDL2/SDL.h>

#include <array>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <thread>
#include <utility>

#include "cellular.hpp"

namespace gol {
// First state is the default one
enum class State { Dead, Alive };

class GameOfLife : public cellular::Automaton<State> {
public:
	GameOfLife(const size_t width, const size_t height);
	const SDL_Color state_to_color(State state) const;
	char            state_to_char(State state) const override;
	State           char_to_state(char c) const override;

protected:
	State next_state(const State& current_cell,
	                 const size_t x,
	                 const size_t y) override;
};

constexpr const char* SPINNER_PATH =
    "/home/ohmree/code/cellular++/tests/spinner.txt";
} // namespace gol
