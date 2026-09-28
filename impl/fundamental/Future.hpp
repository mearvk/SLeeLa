#pragma once
#include <future>
namespace sleela::fundamental { template<class T> class Future { std::future<T> f_; public: Future()=default; explicit Future(std::future<T>f):f_(std::move(f)){} bool valid()const noexcept{return f_.valid();} T get(){return f_.get();} }; }