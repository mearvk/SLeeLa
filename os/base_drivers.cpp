/*
 * os/base_drivers.cpp
 * SLeeLa OS base-driver C++ surface — a typed view over the minimum-boot series.
 *
 * A thin C++ wrapper over base_drivers.h for host tools and the OS image's C++
 * composition layer: it presents the generated driver series as a vector of
 * records, exposes the bring-up and the source-policy / cross-OS-bridge
 * decisions as methods, and classifies the series (boot-required vs. early).
 * The real binding happens in base_drivers.c against the kernel hooks; this is
 * the inspectable, iterable surface.
 */
#include "base_drivers.h"
#include <string>
#include <vector>

namespace sleela_os {

class BaseDrivers {
public:
    struct Record {
        sleela_driver_class  cls;
        std::string          name;
        sleela_driver_phase  phase;
        sleela_driver_origin origin;
    };

    BaseDrivers() {
        const sleela_driver *d = sleela_base_drivers();
        size_t n = sleela_base_driver_count();
        for (size_t i = 0; i < n; ++i)
            records_.push_back(Record{d[i].cls, d[i].name ? d[i].name : "",
                                      d[i].phase, d[i].origin});
    }

    const std::vector<Record> &drivers() const { return records_; }
    std::string host() const { return sleela_os_host_family(); }

    // Bring the series up (walks class order; binds boot drivers first).
    int bringUp() const { return sleela_base_drivers_bringup(); }

    // How many drivers must bind for boot.
    size_t bootRequired() const {
        size_t c = 0;
        for (auto &r : records_) if (r.phase == SL_PHASE_BOOT) ++c;
        return c;
    }

    // Admit a driver by origin (+ foreign family / device class when foreign).
    sleela_driver_admit admit(sleela_driver_origin origin,
                              const std::string &foreignFamily,
                              const std::string &deviceClass) const {
        return sleela_driver_admit_check(origin, foreignFamily.c_str(),
                                         deviceClass.c_str());
    }

    // The bridge adapting a foreign driver to this host, or "none".
    std::string bridge(const std::string &foreignFamily,
                       const std::string &deviceClass) const {
        return sleela_driver_bridge(foreignFamily.c_str(), deviceClass.c_str());
    }

    void setPolicy(bool allowUnknown, bool allowForeign) const {
        sleela_driver_policy(allowUnknown ? 1 : 0, allowForeign ? 1 : 0);
    }

private:
    std::vector<Record> records_;
};

} // namespace sleela_os
