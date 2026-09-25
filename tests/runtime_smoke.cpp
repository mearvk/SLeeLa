#include "../api/native/runtime/sleela_runtime.hpp"
#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>
int main(){using namespace sleela::runtime;EventLoop loop;assert(loop.start()==Status::Ok);std::atomic<int>v{0};assert(loop.post([&]{v.fetch_add(1);}));std::this_thread::sleep_for(std::chrono::milliseconds(20));assert(loop.stop()==Status::Ok);assert(v.load()==1);CancellationToken token;token.cancel();WorkQueue q(2);assert(q.start()==Status::Ok);std::atomic<int>c{0};assert(q.submit([&]{c.fetch_add(1);},&token));std::this_thread::sleep_for(std::chrono::milliseconds(20));q.stop();assert(c.load()==0);return 0;}
