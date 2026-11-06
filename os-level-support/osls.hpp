#ifndef OS_LEVEL_SUPPORT_OSLS_HPP
#define OS_LEVEL_SUPPORT_OSLS_HPP

#include "osls.h"
#include <string>
#include <utility>
#include <vector>

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
    const osls_system_info &native() const noexcept { return info_; }
private:
    osls_system_info info_{};
    std::string hostname_, user_name_, working_directory_;
};

class CronExpression {
public:
    explicit CronExpression(std::string expression) : expression_(std::move(expression)) {}
    osls_status validate() const noexcept { return osls_cron_validate(expression_.c_str()); }
    osls_status next_after(int64_t epoch_seconds, int64_t &out) const noexcept { return osls_cron_next_after(expression_.c_str(), epoch_seconds, &out); }
    const std::string &text() const noexcept { return expression_; }
private:
    std::string expression_;
};

/* Thin typed C++ facade. All operations retain the C ABI's explicit status and
 * buffer semantics; no method launches processes or modifies host settings. */
class Host {
public:
    static osls_status process(uint64_t pid, osls_process_info &out) { return osls_get_process_info(pid, &out); }
    static osls_status processes(std::vector<osls_process_info> &out) {
        size_t count = 0;
        osls_status s = osls_enumerate_processes(nullptr, 0, &count);
        if (s != OSLS_BUFFER_TOO_SMALL && s != OSLS_OK) return s;
        out.resize(count);
        return osls_enumerate_processes(out.data(), out.size(), &count);
    }
    static osls_status environment(const std::string &name, std::string &value) {
        char buffer[1024]; osls_status s = osls_get_environment_value(name.c_str(), buffer, sizeof(buffer));
        if (s == OSLS_OK) value = buffer;
        return s;
    }
    static osls_status environment(std::vector<osls_environment_entry> &out) {
        size_t count = 0;
        osls_status s = osls_enumerate_environment(nullptr, 0, &count);
        if (s != OSLS_BUFFER_TOO_SMALL && s != OSLS_OK) return s;
        out.resize(count);
        return osls_enumerate_environment(out.data(), out.size(), &count);
    }
    static osls_status file(const std::string &path, osls_file_info &out) { return osls_get_file_info(path.c_str(), &out); }
    static osls_status directory(const std::string &path, std::vector<std::string> &out) {
        size_t needed = 0, written = 0;
        osls_status s = osls_list_directory(path.c_str(), nullptr, 0, &needed);
        if (s != OSLS_BUFFER_TOO_SMALL && s != OSLS_OK) return s;
        std::vector<char> packed(needed ? needed : 1);
        s = osls_list_directory(path.c_str(), packed.data(), packed.size(), &written);
        if (s != OSLS_OK) return s;
        out.clear();
        for (size_t i = 0; i < written;) { std::string item(&packed[i]); i += item.size() + 1; out.push_back(std::move(item)); }
        return OSLS_OK;
    }
    static osls_status cpu(osls_cpu_info &out) { return osls_get_cpu_info(&out); }
    static osls_status timezone(osls_timezone_info &out) { return osls_get_timezone_info(&out); }
    static osls_status network(std::vector<osls_network_interface> &out) {
        size_t count = 0;
        osls_status s = osls_get_network_interfaces(nullptr, 0, &count);
        if (s != OSLS_BUFFER_TOO_SMALL && s != OSLS_OK) return s;
        out.resize(count);
        return osls_get_network_interfaces(out.data(), out.size(), &count);
    }
};

} // namespace osls
#endif
