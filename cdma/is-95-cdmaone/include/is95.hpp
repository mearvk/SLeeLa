/* SPDX-License-Identifier: MIT */
#ifndef IS95_REFERENCE_H_HPP
#define IS95_REFERENCE_H_HPP
#include "is95.h"
#include <vector>
#include <stdexcept>
namespace sleela { namespace cdma { namespace is95 {
inline std::vector<int> code(std::size_t sf, unsigned index) { std::vector<int> c(sf); if (is95_make_code(c.data(), sf, index)) throw std::invalid_argument("invalid code"); return c; }
inline std::vector<int> spread(int symbol, std::size_t sf, unsigned index) { std::vector<int> c(sf); if (is95_spread(symbol, sf, index, c.data())) throw std::invalid_argument("invalid spread parameters"); return c; }
inline int recover(const std::vector<int>& chips, unsigned index) { return is95_recover(chips.data(), chips.size(), index); }
}}}
#endif
