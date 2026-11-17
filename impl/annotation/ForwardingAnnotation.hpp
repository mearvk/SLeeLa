#pragma once
#include "Annotation.hpp"
namespace sleela::annotation {
struct ForwardingAnnotation { std::string next; };
class ForwardingPolicy {
public:
    static bool valid(const ForwardingAnnotation& f) noexcept;
    static bool valid_destination(const std::string& next) noexcept;
    // The destination as written: the parser joins an annotation's tokens with single spaces
    // ("system / net"), so a space that does not sit between two letters or digits is dropped
    // ("system/net"). Call it on a destination that valid_destination accepted.
    static std::string normalize_destination(const std::string& next);
};
}