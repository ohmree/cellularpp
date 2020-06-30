#include "cellular.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace cellular;

// First state is the default one
enum class State { Dead, Alive };

class GameOfLife : public Automaton<State> {
    public:
    GameOfLife(const size_t width, const size_t height) :
    Automaton<State>(width, height) {}

    protected:
    virtual State next_state(const State& current_cell, const size_t x,
                             const size_t y) override {
        auto neighborhood = moore_neighborhood_at(x, y);
        unsigned int live_neighbors =
            std::count_if(std::begin(neighborhood),
                          std::end(neighborhood),
                          [](std::pair<const char*, State> const& p) {
                              return p.second == State::Alive;
                          });
        if (current_cell == State::Alive) {
            return (live_neighbors == 2 || live_neighbors == 3) ? State::Alive
                                                                : State::Dead;
        } else if (current_cell == State::Dead) {
            return live_neighbors == 3 ? State::Alive : State::Dead;
        } else {
            return State::Dead;
        }
    }

    virtual char format_state(State state) const override {
        switch (state) {
        case State::Dead:
            return '*';
            break;
        case State::Alive:
            return '#';
            break;
        default:
            return '*';
            break;
        }
    }

    inline State& cell_at(const size_t x, const size_t y) {
        return operator()(x, y);
    }

    inline const State& cell_at(const size_t x, const size_t y) const {
        return operator()(x, y);
    }
};

int main() {
    GameOfLife gol(5, 5);
    gol(0, 1) = State::Alive;
    gol(1, 1) = State::Alive;
    // FIXME: This adds a living tile on (0,5).
    // I think it might have a garbage value (I've seen a 129 when trying to
    // print the grid). I wonder what causes it?
    // gol(1, 0) = State::Alive;
    gol(2, 2) = State::Alive;
    std::cout << '\t' << gol.width() << 'x' << gol.height() << "\n\n";

    gol.print();

    // FIXME: this segfaults, I'm probably doing something wrong with boundaries
    // gol.step();
    // gol.print();

    return 0;
}
