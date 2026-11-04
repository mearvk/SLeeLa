#ifndef OSLS_RUNTIME_HPP
#define OSLS_RUNTIME_HPP

#include "osls_runtime.h"
#include <memory>

namespace osls {

class Runtime {
public:
    Runtime() noexcept;
    ~Runtime();
    Runtime(const Runtime &) = delete;
    Runtime &operator=(const Runtime &) = delete;
    Runtime(Runtime &&other) noexcept;
    Runtime &operator=(Runtime &&other) noexcept;

    osls_runtime_status status() const noexcept { return status_; }
    osls_runtime_status subscribe(osls_event_kind kind, osls_event_callback cb,
                                  void *user, osls_runtime_token &token) noexcept;
    osls_runtime_status unsubscribe(osls_runtime_token token) noexcept;
    osls_runtime_status emit(osls_event_kind kind, const void *payload = nullptr,
                             size_t payload_size = 0) noexcept;
    osls_runtime_status post(osls_task_callback cb, void *user = nullptr) noexcept;
    osls_runtime_status run_one(bool &ran) noexcept;
    size_t pending() const noexcept;
    osls_runtime *native() const noexcept { return runtime_; }

private:
    osls_runtime *runtime_;
    osls_runtime_status status_;
};

} // namespace osls
#endif
