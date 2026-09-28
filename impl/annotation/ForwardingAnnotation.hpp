#pragma once
#include "Annotation.hpp"
namespace sleela::annotation {
struct ForwardingAnnotation { std::string next; };
class ForwardingPolicy { public: static bool valid(const ForwardingAnnotation& f){ return !f.next.empty() && f.next.front()!='/' && f.next.find("..") == std::string::npos; } };
}