#include "ForwardingAnnotation.hpp"
namespace sleela::annotation {
bool ForwardingPolicy::valid_destination(const std::string& v) noexcept {
    return !v.empty() && v.front()!='/' && v.find("..")==std::string::npos && v.find('\\')==std::string::npos;
}
bool ForwardingPolicy::valid(const ForwardingAnnotation& f) noexcept{return valid_destination(f.next);}
}