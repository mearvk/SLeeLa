#include "sleela_runtime.hpp"
namespace sleela::runtime {
WorkQueue::WorkQueue(std::size_t workers):worker_count_(workers?workers:1){} WorkQueue::~WorkQueue(){stop();}
Status WorkQueue::start(){std::lock_guard<std::mutex> l(mutex_);if(accepting_)return Status::Ok;stopping_=false;accepting_=true;for(std::size_t i=0;i<worker_count_;++i)workers_.emplace_back(&WorkQueue::worker,this);return Status::Ok;}
Status WorkQueue::stop(){{std::lock_guard<std::mutex> l(mutex_);if(!accepting_&&workers_.empty())return Status::Ok;accepting_=false;stopping_=true;}cv_.notify_all();for(auto& t:workers_)if(t.joinable())t.join();workers_.clear();std::lock_guard<std::mutex> l(mutex_);while(!queue_.empty())queue_.pop();return Status::Ok;}
bool WorkQueue::running()const noexcept{std::lock_guard<std::mutex>l(mutex_);return accepting_;}
bool WorkQueue::submit(std::function<void()> task,CancellationToken* token){if(!task)return false;{std::lock_guard<std::mutex>l(mutex_);if(!accepting_||stopping_)return false;queue_.push(Job{std::move(task),token});}cv_.notify_one();return true;}
std::size_t WorkQueue::pending()const noexcept{std::lock_guard<std::mutex>l(mutex_);return queue_.size();}
void WorkQueue::worker(){for(;;){Job j;{std::unique_lock<std::mutex>l(mutex_);cv_.wait(l,[this]{return stopping_||!queue_.empty();});if(queue_.empty()&&stopping_)return;if(queue_.empty())continue;j=std::move(queue_.front());queue_.pop();}if(j.token&&j.token->cancelled())continue;try{j.fn();}catch(...){}}}
EventLoop::~EventLoop(){stop();}
Status EventLoop::start(){std::lock_guard<std::mutex>l(mutex_);if(running_)return Status::Ok;stopping_=false;running_=true;thread_=std::thread(&EventLoop::run,this);return Status::Ok;}
Status EventLoop::stop(){{std::lock_guard<std::mutex>l(mutex_);if(!running_)return Status::Ok;stopping_=true;running_=false;}cv_.notify_all();if(thread_.joinable())thread_.join();std::lock_guard<std::mutex>l(mutex_);while(!queue_.empty())queue_.pop();return Status::Ok;}
bool EventLoop::running()const noexcept{std::lock_guard<std::mutex>l(mutex_);return running_;}
bool EventLoop::post(std::function<void()>task){if(!task)return false;{std::lock_guard<std::mutex>l(mutex_);if(!running_||stopping_)return false;queue_.push(std::move(task));}cv_.notify_one();return true;}
void EventLoop::run(){for(;;){std::function<void()>task;{std::unique_lock<std::mutex>l(mutex_);cv_.wait(l,[this]{return stopping_||!queue_.empty();});if(queue_.empty()&&stopping_)return;if(queue_.empty())continue;task=std::move(queue_.front());queue_.pop();}try{task();}catch(...){}}}
std::size_t EventLoop::pending()const noexcept{std::lock_guard<std::mutex>l(mutex_);return queue_.size();}
}