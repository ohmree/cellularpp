#ifndef _CELLULAR_HPP_
#define _CELLULAR_HPP_

#include <cstddef>
#include <cstdio>
#include <unordered_map>
#include <vector>

namespace cellular {
    template<typename T>
    class Automaton {
        public:
        Automaton(const size_t x, const size_t y);
        Automaton() = delete;
        void step();
        void print();
        T& operator()(const size_t x, const size_t y);
        const T& operator()(const size_t x, const size_t y) const;
        size_t width() const;
        size_t height() const;


        protected:
        T& cell_at(const size_t x, const size_t y);
        const T& cell_at(const size_t x, const size_t y) const;
        const std::unordered_map<const char*, T>&
        vn_neighborhood_at(const size_t x, const size_t y);
        const std::unordered_map<const char*, T>&
        moore_neighborhood_at(const size_t x, const size_t y);
        const std::unordered_map<const char*, T>&
        extended_vn_neighborhood_at(const size_t x, const size_t y);
        unsigned int neighbors();
        virtual T next_state(const T& current_cell, const size_t x,
                             const size_t y)     = 0;
        virtual char format_state(T state) const = 0;
        virtual ~Automaton()                     = default;

        private:
        size_t m_width, m_height;
        std::vector<T> m_grid;
        std::vector<T> m_next_grid;
        std::unordered_map<const char*, T> m_neighborhood;
    };

    template<typename T>
    inline Automaton<T>::Automaton(const size_t width, const size_t height) :
    m_width(width - 1), m_height(height - 1), m_grid(width * height, T()),
    m_next_grid(m_grid) {
        //   *
        //  ***
        // ** **
        //  ***
        //   *
        m_neighborhood.reserve(12);
    }

    template<typename T>
    inline T& Automaton<T>::cell_at(const size_t x, const size_t y) {
        return m_grid[x * m_width + y];
    }

    template<typename T>
    inline const T& Automaton<T>::cell_at(const size_t x,
                                          const size_t y) const {
        return m_grid[x * m_width + y];
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

        if (x_over_zero) { m_neighborhood.emplace("w", cell_at(x - 1, y)); }
        if (x_under_limit) { m_neighborhood.emplace("e", cell_at(x + 1, y)); }
        if (y_over_zero) { m_neighborhood.emplace("n", cell_at(x, y - 1)); }
        if (y_under_limit) { m_neighborhood.emplace("s", cell_at(x, y + 1)); }

        return m_neighborhood;
    }

    //  ***
    //  * *
    //  ***
    template<typename T>
    inline const std::unordered_map<const char*, T>&
    Automaton<T>::moore_neighborhood_at(const size_t x, const size_t y) {
        // Partially set the neighborhood map
        vn_neighborhood_at(x, y);
        bool has_north_neighbor = m_neighborhood.contains("n");
        bool has_south_neighbor = m_neighborhood.contains("s");
        bool has_east_neighbor  = m_neighborhood.contains("e");
        bool has_west_neighbor  = m_neighborhood.contains("w");

        if (has_north_neighbor) {
            if (has_east_neighbor) {
                m_neighborhood.emplace("ne", cell_at(x, y));
            }
            if (has_west_neighbor) {
                m_neighborhood.emplace("nw", cell_at(x, y));
            }
        }
        if (has_south_neighbor) {
            if (has_east_neighbor) {
                m_neighborhood.emplace("se", cell_at(x, y));
            }
            if (has_west_neighbor) {
                m_neighborhood.emplace("sw", cell_at(x, y));
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

        if (x_over_one) { m_neighborhood.emplace("w2", cell_at(x - 1, y)); }
        if (x_one_under_limit) {
            m_neighborhood.emplace("e2", cell_at(x + 1, y));
        }
        if (y_over_one) { m_neighborhood.emplace("n2", cell_at(x, y - 1)); }
        if (y_one_under_limit) {
            m_neighborhood.emplace("s2", cell_at(x, y + 1));
        }

        return m_neighborhood;
    }

    template<typename T>
    inline unsigned int Automaton<T>::neighbors() {
        return m_neighborhood.size();
    }

    template<typename T>
    inline void Automaton<T>::step() {
        // for (T&& cell : m_grid) {
        //     size_t i       = &cell - &m_grid[0];
        //     m_next_grid[i] = next_state(cell);
        // }
        for (size_t i = 0; i <= m_width; ++i) {
            for (size_t j = 0; i <= m_height; ++j) {
                T cell                       = cell_at(i, j);
                m_next_grid[i * m_width + j] = next_state(cell, i, j);
            }
        }
        m_grid = m_next_grid;
    }
    template<typename T>
    inline void Automaton<T>::print() {
        for (size_t y = 0; y <= m_width; ++y) {
            std::putchar('\t');
            for (size_t x = 0; x <= m_height; ++x) {
                T current_cell = cell_at(x, y);
                std::printf("%c", format_state(current_cell));
            }
            std::putchar('\n');
        }
    }

    template<typename T>
    inline T& Automaton<T>::operator()(const size_t x, const size_t y) {
        return m_grid[x * m_width + y];
    }

    template<typename T>
    inline const T& Automaton<T>::operator()(const size_t x,
                                             const size_t y) const {
        return m_grid[x * m_width + y];
    }

    template<typename T>
    inline size_t Automaton<T>::width() const {
        return m_width + 1;
    }
    template<typename T>
    inline size_t Automaton<T>::height() const {
        return m_height + 1;
    }
} // namespace cellular

#endif
