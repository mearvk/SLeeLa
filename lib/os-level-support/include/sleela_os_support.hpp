#ifndef SLEELA_OS_SUPPORT_HPP
#define SLEELA_OS_SUPPORT_HPP

#include "sleela_os_support.h"

namespace sleela { namespace os {

class Runtime {
public:
    Runtime() noexcept;
    ~Runtime();
    Runtime(const Runtime &) = delete;
    Runtime &operator=(const Runtime &) = delete;
    Runtime(Runtime &&other) noexcept;
    Runtime &operator=(Runtime &&other) noexcept;

    slosh_status status() const noexcept { return status_; }
    slosh_status subscribe(slosh_event_kind kind, slosh_event_fn callback,
                           void *user_data, slosh_token &token) noexcept;
    slosh_status unsubscribe(slosh_token token) noexcept;
    slosh_status emit(slosh_event_kind kind, const void *payload = nullptr,
                      size_t payload_size = 0) noexcept;
    slosh_status post(slosh_task_fn callback, void *user_data = nullptr) noexcept;
    slosh_status runOne(bool &ran) noexcept;
    size_t pending() const noexcept;

private:
    slosh_runtime *runtime_;
    slosh_status status_;
};

}} // namespace sleela::os
#endif
