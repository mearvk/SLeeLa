#ifndef OS_LEVEL_SUPPORT_OSLS_HPP
#define OS_LEVEL_SUPPORT_OSLS_HPP
#include "osls.h"
#include <string>
#include <utility>
#include <vector>
namespace osls {
class SystemInfo { public: static osls_status read(SystemInfo&out); osls_platform platform()const noexcept{return info_.platform;} uint64_t process_id()const noexcept{return info_.process_id;} int64_t unix_time_seconds()const noexcept{return info_.unix_time_seconds;} uint64_t monotonic_time_nanoseconds()const noexcept{return info_.monotonic_time_nanoseconds;} const std::string&hostname()const noexcept{return hostname_;} const std::string&user_name()const noexcept{return user_name_;} const std::string&working_directory()const noexcept{return working_directory_;} const osls_system_info&native()const noexcept{return info_;} private: osls_system_info info_{}; std::string hostname_,user_name_,working_directory_; };
class CronExpression { public: explicit CronExpression(std::string e):expression_(std::move(e)){} osls_status validate()const noexcept{return osls_cron_validate(expression_.c_str());} osls_status next_after(int64_t t,int64_t&out)const noexcept{return osls_cron_next_after(expression_.c_str(),t,&out);} const std::string&text()const noexcept{return expression_;} private: std::string expression_; };
class Host { public:
static osls_status process(uint64_t p,osls_process_info&o){return osls_get_process_info(p,&o);}
static osls_status processes(std::vector<osls_process_info>&o){size_t n=0;osls_status s=osls_enumerate_processes(nullptr,0,&n);if(s!=OSLS_BUFFER_TOO_SMALL&&s!=OSLS_OK)return s;o.resize(n);return osls_enumerate_processes(o.data(),o.size(),&n);}
static osls_status environment(const std::string&n,std::string&v){char b[1024];osls_status s=osls_get_environment_value(n.c_str(),b,sizeof(b));if(s==OSLS_OK)v=b;return s;}
static osls_status environment(std::vector<osls_environment_entry>&o){size_t n=0;osls_status s=osls_enumerate_environment(nullptr,0,&n);if(s!=OSLS_BUFFER_TOO_SMALL&&s!=OSLS_OK)return s;o.resize(n);return osls_enumerate_environment(o.data(),o.size(),&n);}
static osls_status file(const std::string&p,osls_file_info&o){return osls_get_file_info(p.c_str(),&o);}
static osls_status directory(const std::string&p,std::vector<std::string>&o){size_t n=0,w=0;osls_status s=osls_list_directory(p.c_str(),nullptr,0,&n);if(s!=OSLS_BUFFER_TOO_SMALL&&s!=OSLS_OK)return s;std::vector<char>b(n?n:1);s=osls_list_directory(p.c_str(),b.data(),b.size(),&w);if(s!=OSLS_OK)return s;o.clear();for(size_t i=0;i<w;){std::string x(&b[i]);i+=x.size()+1;o.push_back(std::move(x));}return OSLS_OK;}
static osls_status cpu(osls_cpu_info&o){return osls_get_cpu_info(&o);} static osls_status timezone(osls_timezone_info&o){return osls_get_timezone_info(&o);}
static osls_status network(std::vector<osls_network_interface>&o){size_t n=0;osls_status s=osls_get_network_interfaces(nullptr,0,&n);if(s!=OSLS_BUFFER_TOO_SMALL&&s!=OSLS_OK)return s;o.resize(n);return osls_get_network_interfaces(o.data(),o.size(),&n);}
};
}
#endif
