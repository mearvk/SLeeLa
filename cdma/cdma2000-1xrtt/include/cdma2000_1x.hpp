/* SPDX-License-Identifier: MIT */
#ifndef CDMA2000_1X_REFERENCE_H_HPP
#define CDMA2000_1X_REFERENCE_H_HPP
#include "cdma2000_1x.h"
#include <vector>
#include <stdexcept>
namespace sleela { namespace cdma { namespace cdma2000_1x {
inline std::vector<int> code(std::size_t sf, unsigned index) { std::vector<int> c(sf); if (cdma2000_1x_make_code(c.data(), sf, index)) throw std::invalid_argument("invalid code"); return c; }
inline std::vector<int> spread(int symbol, std::size_t sf, unsigned index) { std::vector<int> c(sf); if (cdma2000_1x_spread(symbol, sf, index, c.data())) throw std::invalid_argument("invalid spread parameters"); return c; }
inline int recover(const std::vector<int>& chips, unsigned index) { return cdma2000_1x_recover(chips.data(), chips.size(), index); }
}}}
#endif
