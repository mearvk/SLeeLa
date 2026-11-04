#include "sleela_os_support.hpp"

namespace sleela { namespace os {
Runtime::Runtime() noexcept : runtime_(nullptr), status_(slosh_runtime_create(&runtime_)) {}
Runtime::~Runtime() { slosh_runtime_destroy(runtime_); }
Runtime::Runtime(Runtime &&other) noexcept : runtime_(other.runtime_), status_(other.status_) {
    other.runtime_ = nullptr;
    other.status_ = SLOSH_INVALID_ARGUMENT;
}
Runtime &Runtime::operator=(Runtime &&other) noexcept {
    if (this != &other) {
        slosh_runtime_destroy(runtime_);
        runtime_ = other.runtime_;
        status_ = other.status_;
        other.runtime_ = nullptr;
        other.status_ = SLOSH_INVALID_ARGUMENT;
    }
    return *this;
}
slosh_status Runtime::subscribe(slosh_event_kind k, slosh_event_fn f, void *u, slosh_token &t) noexcept {
    return runtime_ ? slosh_subscribe(runtime_, k, f, u, &t) : status_;
}
slosh_status Runtime::unsubscribe(slosh_token t) noexcept {
    return runtime_ ? slosh_unsubscribe(runtime_, t) : status_;
}
slosh_status Runtime::emit(slosh_event_kind k, const void *p, size_t n) noexcept {
    return runtime_ ? slosh_emit(runtime_, k, p, n) : status_;
}
slosh_status Runtime::post(slosh_task_fn f, void *u) noexcept {
    return runtime_ ? slosh_post(runtime_, f, u) : status_;
}
slosh_status Runtime::runOne(bool &ran) noexcept {
    int result = 0;
    slosh_status s = runtime_ ? slosh_run_one(runtime_, &result) : status_;
    ran = result != 0;
    return s;
}
size_t Runtime::pending() const noexcept { return slosh_pending(runtime_); }
}} // namespace sleela::os
