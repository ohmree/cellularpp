#ifndef CELLULAR_HPP_
#define CELLULAR_HPP_

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "experimental/mdspan"

namespace stdex = std::experimental;
namespace cellular {

template<typename StateType>
class Automaton {
	using Grid =
	    stdex::mdspan<StateType, stdex::dynamic_extent, stdex::dynamic_extent>;

public:
	// Automaton of size x*y with all cells initialized to the default state
	// (first in StateType enum)
	inline Automaton(const size_t w, const size_t h);
	inline Automaton(const std::filesystem::path& filename);
	inline Automaton() = delete;
	inline void             step();
	inline void             print();
	inline StateType&       operator()(const size_t x, const size_t y);
	inline const StateType& operator()(const size_t x, const size_t y) const;
	inline size_t           width() const;
	inline size_t           height() const;
	inline void set_grid_from_file(const std::filesystem::path& filename,
	                               const char delimiter = '\n');
	inline void set_grid_from_string(std::string&& str,
	                                 const char    delimiter = '\n');
	inline virtual StateType cycle_state(
	    const StateType current_cell) const = 0;

protected:
	inline const std::unordered_map<const char*, StateType>& vn_neighborhood_at(
	    const size_t x,
	    const size_t y);
	inline const std::unordered_map<const char*, StateType>&
	moore_neighborhood_at(const size_t x, const size_t y);
	inline const std::unordered_map<const char*, StateType>&
	                         extended_vn_neighborhood_at(const size_t x, const size_t y);
	inline unsigned int      neighbors();
	inline virtual StateType next_state(const StateType current_cell,
	                                    const size_t    x,
	                                    const size_t    y)           = 0;
	inline virtual char      state_to_char(StateType state) const = 0;
	inline virtual StateType char_to_state(char c) const          = 0;
	inline virtual ~Automaton()                                   = default;

private:
	/// Width starting from 0, so a 5x5 automaton would have a `m_width`
	/// of 4.
	size_t m_width;
	/// Height starting from 0, so a 5x5 automaton would have a `m_height`
	/// of 4.
	size_t m_height;
	/// The underlying vector where the current grid is stored.
	std::vector<StateType> m_grid_vec;
	/// The underlying vector where the next iteration of the grid is
	/// stored.
	std::vector<StateType> m_next_grid_vec;
	/// The `mdspan` that facilitates convenient 2D access to `m_grid_vec`.
	Grid m_grid;
	/// The `mdspan` that facilitates convenient 2D access to
	/// `m_next_grid_vec`.
	Grid m_next_grid;
	/// The map that stores the result of the `*_neighborhood_at` functions.
	std::unordered_map<const char*, StateType> m_neighborhood;
};

template<typename StateType>
inline Automaton<StateType>::Automaton(const size_t width,
                                       const size_t height) :
    m_width(width - 1),
    m_height(height - 1),
    m_grid_vec(width * height, StateType()),
    m_next_grid_vec(m_grid_vec),
    m_grid(m_grid_vec.data(), width, height),
    m_next_grid(m_next_grid_vec.data(), width, height) {
	//   *
	//  ***
	// ** **
	//  ***
	//   *
	m_neighborhood.reserve(12);
}

template<typename StateType>
inline Automaton<StateType>::Automaton(const std::filesystem::path& filename) {
	set_grid_from_file(filename);
	m_neighborhood.reserve(12);
}

//   *
//  * *
//   *
template<typename StateType>
inline const std::unordered_map<const char*, StateType>&
Automaton<StateType>::vn_neighborhood_at(const size_t x, const size_t y) {
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
template<typename StateType>
inline const std::unordered_map<const char*, StateType>&
Automaton<StateType>::moore_neighborhood_at(const size_t x, const size_t y) {
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
		if (x_over_zero) { m_neighborhood.emplace("nw", m_grid(x - 1, y - 1)); }
	}
	if (y_under_limit) {
		if (x_under_limit) {
			m_neighborhood.emplace("se", m_grid(x + 1, y + 1));
		}
		if (x_over_zero) { m_neighborhood.emplace("sw", m_grid(x - 1, y + 1)); }
	}

	return m_neighborhood;
}

//   *
//   *
// ** **
//   *
//   *
template<typename StateType>
inline const std::unordered_map<const char*, StateType>&
Automaton<StateType>::extended_vn_neighborhood_at(const size_t x,
                                                  const size_t y) {
	vn_neighborhood_at(x, y);
	bool x_over_one        = x > 1;
	bool x_one_under_limit = x < m_width - 1;
	bool y_over_one        = y > 1;
	bool y_one_under_limit = y < m_height - 1;

	if (x_over_one) { m_neighborhood.emplace("w2", m_grid(x - 1, y)); }
	if (x_one_under_limit) { m_neighborhood.emplace("e2", m_grid(x + 1, y)); }
	if (y_over_one) { m_neighborhood.emplace("n2", m_grid(x, y - 1)); }
	if (y_one_under_limit) { m_neighborhood.emplace("s2", m_grid(x, y + 1)); }

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
inline T& Automaton<T>::operator()(const size_t x, const size_t y) {
	return m_grid(x, y);
}

template<typename T>
inline const T& Automaton<T>::operator()(const size_t x, const size_t y) const {
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
inline void Automaton<T>::set_grid_from_file(
    const std::filesystem::path& filename,
    const char                   delimiter) {
	std::ifstream filein(filename);

	// Construct string with file contents
	// TODO: validate file contents, existence, etc.
	std::string grid_string{std::istreambuf_iterator<char>(filein),
	                        std::istreambuf_iterator<char>()};
	set_grid_from_string(std::move(grid_string), delimiter);
}

template<typename T>
inline void Automaton<T>::set_grid_from_string(std::string&& str,
                                               const char    delimiter) {
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

	std::vector<T> grid_vec;
	grid_vec.reserve(chars.size());
	for (char c : chars) {
		T state = char_to_state(c);
		grid_vec.push_back(state);
	}

	m_grid_vec      = std::move(grid_vec);
	m_next_grid_vec = m_grid_vec;
	m_grid          = Grid(m_grid_vec.data(), m_width + 1, m_height + 1);
	m_next_grid     = Grid(m_next_grid_vec.data(), m_width + 1, m_height + 1);
}

} // namespace cellular

#endif
