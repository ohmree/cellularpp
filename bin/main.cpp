#include "cellular.hpp"
#include "pprint.hpp"

#include <SDL2/SDL.h>
#include <algorithm>
#include <chrono>
#include <thread>

constexpr const SDL_Color WHITE   = {.r = 255, .g = 255, .b = 255, .a = 255};
constexpr const SDL_Color BLACK   = {.r = 0, .g = 0, .b = 0, .a = 0};
constexpr const int SCREEN_WIDTH  = 640;
constexpr const int SCREEN_HEIGHT = 480;

using namespace cellular;

// First state is the default one
enum class State { Dead, Alive };

static pprint::PrettyPrinter pp;

class GameOfLife : public Automaton<State> {
    public:
    GameOfLife(const size_t width, const size_t height) :
    Automaton<State>(width, height) {}

    protected:
    State next_state(const State& current_cell, const size_t x,
                     const size_t y) override {
        const auto& neighborhood = moore_neighborhood_at(x, y);
        unsigned int live_neighbors =
            std::count_if(std::begin(neighborhood),
                          std::end(neighborhood),
                          [](std::pair<const char*, State> const& p) {
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

    // sf::Color state_to_color(State state) {
    //     return state == State::Alive ? sf::Color::Black : sf::Color::White;
    // }
    SDL_Color state_to_color(State state) {
        return state == State::Alive ? BLACK : WHITE;
    }
};

constexpr const char* SPINNER_PATH =
    "/home/animo/code/cellular++/tests/spinner.txt";

int main() {
    // FIXME: this seems to add extra living cells.
    // TODO: find out why????
    // GameOfLife gol(3, 3);
    // gol(1, 0) = State::Alive;
    // gol(1, 1) = State::Alive;
    // gol(1, 2) = State::Alive;

    // FIXME: this seems to result in an empty grid.
    // TODO: set up proper unit tests to pinpoint the bug
    // gol.set_grid_from_file(SPINNER_PATH);

    GameOfLife gol(5, 5);

    gol(2, 1) = State::Alive;
    gol(2, 2) = State::Alive;
    gol(2, 3) = State::Alive;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Game of Life",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError()
                  << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
