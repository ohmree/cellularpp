#ifndef CELLULAR_GUI_HPP_
#define CELLULAR_GUI_HPP_

#include <SDL2/SDL.h>

#include "cellular.hpp"

namespace cellular {
template<typename T>
class GuiAutomaton : public Automaton<T> {
public:
	inline GuiAutomaton(size_t w, size_t h);
	GuiAutomaton() = delete;

protected:
	inline virtual const SDL_Color state_to_color(T state) const = 0;
};

template<typename T>
inline GuiAutomaton<T>::GuiAutomaton(size_t w, size_t h) : Automaton<T>(w, h) {}

} // namespace cellular

#endif // CELLULAR_GUI_HPP
