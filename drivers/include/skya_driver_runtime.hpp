#ifndef SKYA_DRIVER_RUNTIME_HPP
#define SKYA_DRIVER_RUNTIME_HPP
#include "skya_driver_runtime.h"
#include <stdexcept>
namespace skya { class DriverRuntime { skya_driver_runtime*p_; public: explicit DriverRuntime(size_t n=SKYA_DRIVER_QUEUE_CAPACITY):p_(nullptr){if(skya_driver_runtime_init(&p_,n)!=0)throw std::bad_alloc();} ~DriverRuntime(){skya_driver_runtime_destroy(p_);} DriverRuntime(const DriverRuntime&)=delete; DriverRuntime&operator=(const DriverRuntime&)=delete; void start(){if(skya_driver_runtime_start(p_))throw std::runtime_error("start failed");} void stop(){skya_driver_runtime_stop(p_);} uint64_t send(skya_message_type t,const void*d,size_t n){uint64_t s=0;if(skya_driver_runtime_send(p_,t,d,n,&s))throw std::runtime_error("send failed");return s;} bool receive(skya_driver_message&m,int ms=-1){return skya_driver_runtime_receive(p_,&m,ms)==0;} size_t depth()const{return skya_driver_runtime_depth(p_);} }; }
#endif
