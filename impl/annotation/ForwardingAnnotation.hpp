#pragma once
#include "Annotation.hpp"
namespace sleela::annotation {
struct ForwardingAnnotation { std::string next; };
class ForwardingPolicy {
public:
    static bool valid(const ForwardingAnnotation& f) noexcept;
    static bool valid_destination(const std::string& next) noexcept;
};
}