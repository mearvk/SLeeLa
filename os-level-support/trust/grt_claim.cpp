#include "grt_claim.hpp"
#include <cstring>

namespace grt {

Claim::Claim() noexcept : value_{} {
    value_.version = 1;
}

Claim::Claim(const grt_claim &value) noexcept : value_(value) {}

grt_claim_status Claim::validate(int64_t now) const noexcept {
    return grt_claim_validate(&value_, now);
}

const char *Claim::statusName(int64_t now) const noexcept {
    return grt_claim_status_name(validate(now));
}

} // namespace grt
