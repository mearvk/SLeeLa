#ifndef SLEELA_OFDMA_HPP
#define SLEELA_OFDMA_HPP
#include "ofdma.h"
#include <stdexcept>
namespace sleela::ofdma {
class Numerology {
public:
    explicit Numerology(ofdma_numerology value) : value_(value) {
        if (ofdma_validate_numerology(&value_) != OFDMA_OK)
            throw std::invalid_argument("invalid OFDMA numerology");
    }
    ofdma_metrics metrics(uint32_t symbols) const {
        ofdma_metrics result{};
        if (ofdma_calculate_metrics(&value_, symbols, &result) != OFDMA_OK)
            throw std::invalid_argument("invalid OFDMA grid symbol count");
        return result;
    }
    const ofdma_numerology& native() const noexcept { return value_; }
private:
    ofdma_numerology value_;
};
}
#endif
