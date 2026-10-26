/* SPDX-License-Identifier: MIT */
#ifndef EVDO_REFERENCE_H_HPP
#define EVDO_REFERENCE_H_HPP
#include "evdo.h"
#include <vector>
#include <stdexcept>
namespace sleela { namespace cdma { namespace evdo {
inline std::vector<int> code(std::size_t sf, unsigned index) { std::vector<int> c(sf); if (evdo_make_code(c.data(), sf, index)) throw std::invalid_argument("invalid code"); return c; }
inline std::vector<int> spread(int symbol, std::size_t sf, unsigned index) { std::vector<int> c(sf); if (evdo_spread(symbol, sf, index, c.data())) throw std::invalid_argument("invalid spread parameters"); return c; }
inline int recover(const std::vector<int>& chips, unsigned index) { return evdo_recover(chips.data(), chips.size(), index); }
}}}
#endif
