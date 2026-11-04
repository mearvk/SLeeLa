#ifndef OS_LEVEL_SUPPORT_OSLS_HPP
#define OS_LEVEL_SUPPORT_OSLS_HPP

#include "osls.h"
#include <string>

namespace osls {

class SystemInfo {
public:
    static osls_status read(SystemInfo &out);
    osls_platform platform() const noexcept { return info_.platform; }
    uint64_t process_id() const noexcept { return info_.process_id; }
    int64_t unix_time_seconds() const noexcept { return info_.unix_time_seconds; }
    uint64_t monotonic_time_nanoseconds() const noexcept { return info_.monotonic_time_nanoseconds; }
    const std::string &hostname() const noexcept { return hostname_; }
    const std::string &user_name() const noexcept { return user_name_; }
    const std::string &working_directory() const noexcept { return working_directory_; }

private:
    osls_system_info info_{};
    std::string hostname_;
    std::string user_name_;
    std::string working_directory_;
};

class CronExpression {
public:
    explicit CronExpression(std::string expression) : expression_(std::move(expression)) {}
    osls_status validate() const noexcept;
    osls_status next_after(int64_t epoch_seconds, int64_t &out_epoch_seconds) const noexcept;
    const std::string &text() const noexcept { return expression_; }

private:
    std::string expression_;
};

} // namespace osls

#endif
