#include "osls.hpp"

namespace osls {

osls_status SystemInfo::read(SystemInfo &out) {
    osls_system_info value{};
    const osls_status status = osls_get_system_info(&value);
    if (status != OSLS_OK) return status;
    out.info_ = value;
    out.hostname_ = value.hostname;
    out.user_name_ = value.user_name;
    out.working_directory_ = value.working_directory;
    return OSLS_OK;
}

osls_status CronExpression::validate() const noexcept {
    return osls_cron_validate(expression_.c_str());
}

osls_status CronExpression::next_after(int64_t epoch_seconds, int64_t &out_epoch_seconds) const noexcept {
    return osls_cron_next_after(expression_.c_str(), epoch_seconds, &out_epoch_seconds);
}

} // namespace osls
