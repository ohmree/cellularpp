#ifndef CELLULAR_GAME_OF_LIFE_HPP_
#define CELLULAR_GAME_OF_LIFE_HPP_

#include <SDL2/SDL.h>

#include <filesystem>

#include "cellular.hpp"
#include "cellular_gui.hpp"

using namespace cellular;

namespace gol {
// First state is the default one
enum class State { Dead, Alive };

class GameOfLife : public Automaton<State> {
public:
	GameOfLife(const size_t width, const size_t height);
	GameOfLife(const std::filesystem::path& filename);
	char  state_to_char(State state) const override;
	State char_to_state(char c) const override;
	State cycle_state(const State current_cell) const override;

protected:
	State next_state(const State  current_cell,
	                 const size_t x,
	                 const size_t y) override;
};
} // namespace gol

#endif // CELLULAR_GAME_OF_LIFE_HPP_
