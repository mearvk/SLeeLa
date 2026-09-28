#include "Handler.hpp"
#include <utility>
namespace sleela::fundamental {
Handler::Handler(std::function<std::string(const std::string&)> f):f_(std::move(f)){}
std::string Handler::invoke(const std::string&i)const{return f_?f_(i):std::string{};}
bool Handler::valid()const noexcept{return bool(f_);}
}
