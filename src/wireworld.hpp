#ifndef CELLULAR_WIREWORLD_HPP_
#define CELLULAR_WIREWORLD_HPP_

#include <SDL2/SDL.h>

#include <filesystem>

#include "cellular.hpp"
#include "cellular_gui.hpp"

using namespace cellular;

namespace wireworld {
// First state is the default one
enum class State { Empty, ElectronHead, ElectronTail, Conductor };

class Wireworld : public Automaton<State> {
public:
	Wireworld(const size_t width, const size_t height);
	Wireworld(const std::filesystem::path& filename);
	char  state_to_char(State state) const override;
	State char_to_state(char c) const override;
	State cycle_state(const State current_cell) const override;

protected:
	State next_state(const State  current_cell,
	                 const size_t x,
	                 const size_t y) override;
};
} // namespace wireworld

#endif // CELLULAR_WIREWORLD_HPP_
