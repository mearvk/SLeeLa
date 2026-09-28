#pragma once
#include "Annotation.hpp"
#include "AnnotationForwarder.hpp"
#include <string>
namespace sleela::annotation {
class AnnotationRuntime {
public:
    bool install(const DocumentAnnotations& annotations, std::string& error) noexcept;
    const DocumentAnnotations& annotations() const noexcept { return annotations_; }
    const ForwardingResult& forwarding() const noexcept { return forwarding_; }
    bool hasForwarding() const noexcept { return forwarding_.status == ForwardingStatus::Forwarded; }
    const std::string& nexterColony() const noexcept { return forwarding_.next; }
private:
    DocumentAnnotations annotations_;
    ForwardingResult forwarding_{};
};
} // namespace sleela::annotation
