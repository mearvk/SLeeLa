#pragma once
#include "Annotation.hpp"
#include "ForwardingAnnotation.hpp"
namespace sleela::annotation {
enum class ForwardingStatus { None, Forwarded, Rejected, Multiple };
struct ForwardingResult { bool forwarded=false; bool rejected=false; std::string next; ForwardingStatus status=ForwardingStatus::None; std::size_t candidates=0; };
class AnnotationForwarder { public: ForwardingResult forward(const DocumentAnnotations& d) const noexcept; };
}