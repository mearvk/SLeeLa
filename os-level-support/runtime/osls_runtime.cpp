#include "osls_runtime.hpp"

namespace osls {
Runtime::Runtime() noexcept : runtime_(nullptr), status_(osls_runtime_create(&runtime_)) {}
Runtime::~Runtime() { osls_runtime_destroy(runtime_); }
Runtime::Runtime(Runtime &&other) noexcept : runtime_(other.runtime_), status_(other.status_) {
    other.runtime_ = nullptr;
    other.status_ = OSLS_RUNTIME_INVALID_ARGUMENT;
}
Runtime &Runtime::operator=(Runtime &&other) noexcept {
    if (this != &other) {
        osls_runtime_destroy(runtime_);
        runtime_ = other.runtime_;
        status_ = other.status_;
        other.runtime_ = nullptr;
        other.status_ = OSLS_RUNTIME_INVALID_ARGUMENT;
    }
    return *this;
}
osls_runtime_status Runtime::subscribe(osls_event_kind k, osls_event_callback cb,
    void *u, osls_runtime_token &t) noexcept {
    return runtime_ ? osls_runtime_subscribe(runtime_, k, cb, u, &t) : status_;
}
osls_runtime_status Runtime::unsubscribe(osls_runtime_token t) noexcept {
    return runtime_ ? osls_runtime_unsubscribe(runtime_, t) : status_;
}
osls_runtime_status Runtime::emit(osls_event_kind k, const void *p, size_t n) noexcept {
    return runtime_ ? osls_runtime_emit(runtime_, k, p, n) : status_;
}
osls_runtime_status Runtime::post(osls_task_callback cb, void *u) noexcept {
    return runtime_ ? osls_runtime_post(runtime_, cb, u) : status_;
}
osls_runtime_status Runtime::run_one(bool &ran) noexcept {
    int did_run = 0;
    osls_runtime_status s = runtime_ ? osls_runtime_run_one(runtime_, &did_run) : status_;
    ran = did_run != 0;
    return s;
}
size_t Runtime::pending() const noexcept { return osls_runtime_pending(runtime_); }
} // namespace osls
