#include "osls.hpp"
namespace osls {
osls_status SystemInfo::read(SystemInfo&out){osls_system_info v{};osls_status s=osls_get_system_info(&v);if(s!=OSLS_OK)return s;out.info_=v;out.hostname_=v.hostname;out.user_name_=v.user_name;out.working_directory_=v.working_directory;return OSLS_OK;}
}
