#pragma once
#include <atomic>
#include <memory>
namespace sleela::fundamental { class CancellationToken { std::shared_ptr<std::atomic_bool> state_; public: CancellationToken():state_(std::make_shared<std::atomic_bool>(false)){} void cancel()noexcept{state_->store(true,std::memory_order_release);} bool cancelled()const noexcept{return state_->load(std::memory_order_acquire);} }; }