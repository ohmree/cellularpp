#ifndef _CELLULAR_HPP_
#define _CELLULAR_HPP_

#include "experimental/mdspan"
#include "pprint.hpp"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>


namespace cellular {
    namespace stdex = std::experimental;
    template<typename T>
    class Automaton {
        using Grid =
            stdex::mdspan<T, stdex::dynamic_extent, stdex::dynamic_extent>;

        public:
        // Automaton of size x*y with all cells initialized to the default state
        // (first in T enum)
        Automaton(const size_t w, const size_t h);
        Automaton() = delete;
        void step();
        void print();
        T& operator()(const size_t x, const size_t y);
        const T& operator()(const size_t x, const size_t y) const;
        size_t width() const;
        size_t height() const;
        void set_grid_from_file(const std::filesystem::path& filename,
                                const char delimiter = '\n');

        void set_grid_from_string(std::string&& str,
                                  const char delimiter = '\n');

        protected:
        const std::unordered_map<const char*, T>&
        vn_neighborhood_at(const size_t x, const size_t y);
        const std::unordered_map<const char*, T>&
        moore_neighborhood_at(const size_t x, const size_t y);
        const std::unordered_map<const char*, T>&
        extended_vn_neighborhood_at(const size_t x, const size_t y);
        unsigned int neighbors();
        virtual T next_state(const T& current_cell, const size_t x,
                             const size_t y)      = 0;
        virtual char state_to_char(T state) const = 0;
        virtual T char_to_state(char c) const     = 0;
        virtual ~Automaton()                      = default;
        pprint::PrettyPrinter m_pp;

        private:
        size_t m_width;
        size_t m_height;
        std::vector<T> m_grid_vec;
        std::vector<T> m_next_grid_vec;
        Grid m_grid;
        Grid m_next_grid;
        std::unordered_map<const char*, T> m_neighborhood;
    };

    template<typename T>
    inline Automaton<T>::Automaton(const size_t width, const size_t height) :
    m_width(width - 1), m_height(height - 1), m_grid_vec(width * height, T()),
    m_next_grid_vec(m_grid_vec),
    m_grid(m_grid_vec.data(), width - 1, height - 1),
    m_next_grid(m_next_grid_vec.data(), width - 1, height - 1) {
        //   *
        //  ***
        // ** **
        //  ***
        //   *
        m_neighborhood.reserve(12);
        m_pp.indent(0);
    }

    //   *
    //  * *
    //   *
    template<typename T>
    inline const std::unordered_map<const char*, T>&
    Automaton<T>::vn_neighborhood_at(const size_t x, const size_t y) {
        m_neighborhood.clear();
        bool x_over_zero   = x > 0;
        bool x_under_limit = x < m_width;
        bool y_over_zero   = y > 0;
        bool y_under_limit = y < m_height;

        if (x_over_zero) { m_neighborhood.emplace("w", m_grid(x - 1, y)); }
        if (x_under_limit) { m_neighborhood.emplace("e", m_grid(x + 1, y)); }
        if (y_over_zero) { m_neighborhood.emplace("n", m_grid(x, y - 1)); }
        if (y_under_limit) { m_neighborhood.emplace("s", m_grid(x, y + 1)); }

        return m_neighborhood;
    }

    //  ***
    //  * *
    //  ***
    template<typename T>
    inline const std::unordered_map<const char*, T>&
    Automaton<T>::moore_neighborhood_at(const size_t x, const size_t y) {
        m_neighborhood.clear();
        bool x_over_zero   = x > 0;
        bool x_under_limit = x < m_width;
        bool y_over_zero   = y > 0;
        bool y_under_limit = y < m_height;

        if (x_over_zero) { m_neighborhood.emplace("w", m_grid(x - 1, y)); }
        if (x_under_limit) { m_neighborhood.emplace("e", m_grid(x + 1, y)); }
        if (y_over_zero) { m_neighborhood.emplace("n", m_grid(x, y - 1)); }
        if (y_under_limit) { m_neighborhood.emplace("s", m_grid(x, y + 1)); }

        if (y_over_zero) {
            if (x_under_limit) {
                m_neighborhood.emplace("ne", m_grid(x + 1, y - 1));
            }
            if (x_over_zero) {
                m_neighborhood.emplace("nw", m_grid(x - 1, y - 1));
            }
        }
        if (y_under_limit) {
            if (x_under_limit) {
                m_neighborhood.emplace("se", m_grid(x + 1, y + 1));
            }
            if (x_over_zero) {
                m_neighborhood.emplace("sw", m_grid(x - 1, y + 1));
            }
        }

        return m_neighborhood;
    }

    //   *
    //   *
    // ** **
    //   *
    //   *
    template<typename T>
    inline const std::unordered_map<const char*, T>&
    Automaton<T>::extended_vn_neighborhood_at(const size_t x, const size_t y) {
        vn_neighborhood_at(x, y);
        bool x_over_one        = x > 1;
        bool x_one_under_limit = x < m_width - 1;
        bool y_over_one        = y > 1;
        bool y_one_under_limit = y < m_height - 1;

        if (x_over_one) { m_neighborhood.emplace("w2", m_grid(x - 1, y)); }
        if (x_one_under_limit) {
            m_neighborhood.emplace("e2", m_grid(x + 1, y));
        }
        if (y_over_one) { m_neighborhood.emplace("n2", m_grid(x, y - 1)); }
        if (y_one_under_limit) {
            m_neighborhood.emplace("s2", m_grid(x, y + 1));
        }

        return m_neighborhood;
    }

    template<typename T>
    inline unsigned int Automaton<T>::neighbors() {
        return m_neighborhood.size();
    }

    template<typename T>
    inline void Automaton<T>::step() {
        for (size_t i = 0; i <= m_width; ++i) {
            for (size_t j = 0; j <= m_height; ++j) {
                T& current_cell   = m_grid(j, i);
                m_next_grid(j, i) = next_state(current_cell, j, i);
            }
        }
        m_grid_vec = m_next_grid_vec;
    }

    template<typename T>
    inline void Automaton<T>::print() {
        m_pp.print_inline('\n');
        for (size_t y = 0; y <= m_width; ++y) {
            m_pp.indent(2);
            for (size_t x = 0; x <= m_height; ++x) {
                T& current_cell = m_grid(x, y);
                m_pp.print_inline(state_to_char(current_cell));
                m_pp.indent(0);
            }
            m_pp.print_inline('\n');
        }

        std::cout << '\n';
    }

    template<typename T>
    inline T& Automaton<T>::operator()(const size_t x, const size_t y) {
        return m_grid(x, y);
    }

    template<typename T>
    inline const T& Automaton<T>::operator()(const size_t x,
                                             const size_t y) const {
        return m_grid(x, y);
    }

    template<typename T>
    inline size_t Automaton<T>::width() const {
        return m_width + 1;
    }
    template<typename T>
    inline size_t Automaton<T>::height() const {
        return m_height + 1;
    }

    template<typename T>
    inline void
    Automaton<T>::set_grid_from_file(const std::filesystem::path& filename,
                                     const char delimiter) {
        std::ifstream filein(filename);

        // Construct string with file contents
        // TODO: validate file contents, existence, etc.
        std::string grid_string{std::istreambuf_iterator<char>(filein),
                                std::istreambuf_iterator<char>()};

        set_grid_from_string(std::move(grid_string), delimiter);
    }


    template<typename T>
    inline void Automaton<T>::set_grid_from_string(std::string&& str,
                                                   const char delimiter) {
        // TODO: validate string non-emptiness, conformance to shape, etc.
        std::vector<char> chars(str.begin(), str.end());

        // This is an iterator
        auto first_occurance = std::find(chars.begin(), chars.end(), delimiter);
        // Calculate width from first occurance of delimiter...
        m_width = first_occurance - chars.begin() - 1;
        // ...and height from number of occurances of delimiter (e.g.
        // newline, so amount of lines).
        m_height = std::count(chars.begin(), chars.end(), delimiter) - 1;

        // Remove delimiters
        chars.erase(std::remove(chars.begin(), chars.end(), delimiter),
                    chars.end());

        std::vector<T> grid_vec(chars.capacity());
        for (char c : chars) {
            T state = char_to_state(c);
            grid_vec.push_back(state);
        }
        m_grid_vec      = std::move(grid_vec);
        m_next_grid_vec = m_grid_vec;
        m_grid          = Grid(m_grid_vec.data(), m_width, m_height);
        m_next_grid     = Grid(m_next_grid_vec.data(), m_width, m_height);
    }
} // namespace cellular

#endif
