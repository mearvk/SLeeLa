#pragma once
#include <functional>
#include <future>
#include <utility>
namespace sleela::fundamental {
class Task {
    std::function<void()> fn_;
    std::future<void> future_;
public:
    explicit Task(std::function<void()> f = {});
    void start();
    void wait();
    bool valid() const noexcept;
};
}