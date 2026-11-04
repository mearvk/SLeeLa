#ifndef GRT_CLAIM_HPP
#define GRT_CLAIM_HPP

#include "grt_claim.h"

namespace grt {

class Claim {
public:
    Claim() noexcept;
    explicit Claim(const grt_claim &value) noexcept;

    grt_claim_status validate(int64_t now) const noexcept;
    const grt_claim &native() const noexcept { return value_; }
    grt_claim &native() noexcept { return value_; }
    const char *statusName(int64_t now) const noexcept;

private:
    grt_claim value_;
};

} // namespace grt
#endif
