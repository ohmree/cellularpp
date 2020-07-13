#ifndef CELLULAR_WIREWORLD_HPP_
#define CELLULAR_WIREWORLD_HPP_

#include <SDL2/SDL.h>

#include "cellular.hpp"
#include "cellular_gui.hpp"

using namespace cellular;

namespace wireworld {
// First state is the default one
enum class State { Empty, ElectronHead, ElectronTail, Conductor };

class WireWorld : public GuiAutomaton<State> {
public:
	WireWorld(const size_t width, const size_t height);
	const SDL_Color state_to_color(State state) const override;
	char            state_to_char(State state) const override;
	State           char_to_state(char c) const override;

protected:
	State next_state(const State& current_cell,
	                 const size_t x,
	                 const size_t y) override;
};
} // namespace wireworld

#endif // CELLULAR_WIREWORLD_HPP_
