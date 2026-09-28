
#include "Environment.hpp"
#include <cstdlib>
namespace sleela::fundamental {
bool Environment::has(const std::string&k){return std::getenv(k.c_str())!=nullptr;}
std::string Environment::get(const std::string&k,const std::string&f){auto*v=std::getenv(k.c_str());return v?v:f;}
bool Environment::set(const std::string&k,const std::string&v){
#if defined(_WIN32)
return _putenv_s(k.c_str(),v.c_str())==0;
#else
return ::setenv(k.c_str(),v.c_str(),1)==0;
#endif
}
bool Environment::unset(const std::string&k){
#if defined(_WIN32)
return _putenv_s(k.c_str(),"")==0;
#else
return ::unsetenv(k.c_str())==0;
#endif
}
}
