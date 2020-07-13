#ifndef CELLULAR_GAME_OF_LIFE_HPP_
#define CELLULAR_GAME_OF_LIFE_HPP_

#include <SDL2/SDL.h>

#include "cellular.hpp"
#include "cellular_gui.hpp"

using namespace cellular;

namespace gol {
// First state is the default one
enum class State { Dead, Alive };

class GameOfLife : public GuiAutomaton<State> {
public:
	GameOfLife(const size_t width, const size_t height);
	const SDL_Color state_to_color(State state) const override;
	char            state_to_char(State state) const override;
	State           char_to_state(char c) const override;

protected:
	State next_state(const State& current_cell,
	                 const size_t x,
	                 const size_t y) override;
};
} // namespace gol

#endif // CELLULAR_GAME_OF_LIFE_HPP_
