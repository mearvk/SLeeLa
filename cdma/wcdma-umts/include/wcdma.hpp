/* SPDX-License-Identifier: MIT */
#ifndef WCDMA_REFERENCE_H_HPP
#define WCDMA_REFERENCE_H_HPP
#include "wcdma.h"
#include <vector>
#include <stdexcept>
namespace sleela { namespace cdma { namespace wcdma {
inline std::vector<int> code(std::size_t sf, unsigned index) { std::vector<int> c(sf); if (wcdma_make_code(c.data(), sf, index)) throw std::invalid_argument("invalid code"); return c; }
inline std::vector<int> spread(int symbol, std::size_t sf, unsigned index) { std::vector<int> c(sf); if (wcdma_spread(symbol, sf, index, c.data())) throw std::invalid_argument("invalid spread parameters"); return c; }
inline int recover(const std::vector<int>& chips, unsigned index) { return wcdma_recover(chips.data(), chips.size(), index); }
}}}
#endif
