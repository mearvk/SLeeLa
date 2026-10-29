#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
namespace sleela::runtime {
enum class Status { Ok, Stopped, Cancelled, Invalid };
class CancellationToken { public: void cancel() noexcept { cancelled_.store(true,std::memory_order_release); } bool cancelled() const noexcept { return cancelled_.load(std::memory_order_acquire); } private: std::atomic<bool> cancelled_{false}; };
class WorkQueue {
public:
 explicit WorkQueue(std::size_t workers=1); ~WorkQueue(); WorkQueue(const WorkQueue&)=delete; WorkQueue& operator=(const WorkQueue&)=delete;
 Status start(); Status stop(); bool running() const noexcept; bool submit(std::function<void()> task,CancellationToken* token=nullptr); std::size_t pending() const noexcept;
private:
 struct Job { std::function<void()> fn; CancellationToken* token; }; void worker();
 mutable std::mutex mutex_; std::condition_variable cv_; std::queue<Job> queue_; std::vector<std::thread> workers_; std::size_t worker_count_; bool accepting_{false}; bool stopping_{false};
};
class EventLoop {
public:
 EventLoop()=default; ~EventLoop(); EventLoop(const EventLoop&)=delete; EventLoop& operator=(const EventLoop&)=delete;
 Status start(); Status stop(); bool running() const noexcept; bool post(std::function<void()> task); void run(); std::size_t pending() const noexcept;
private:
 mutable std::mutex mutex_; std::condition_variable cv_; std::queue<std::function<void()>> queue_; std::thread thread_; bool running_{false}; bool stopping_{false};
};
template<class T> struct FutureResult { std::future<T> future; };
}