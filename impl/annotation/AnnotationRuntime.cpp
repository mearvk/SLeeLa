#include "AnnotationRuntime.hpp"
namespace sleela::annotation {
bool AnnotationRuntime::install(const DocumentAnnotations& annotations, std::string& error) noexcept {
    annotations_ = annotations;
    AnnotationForwarder forwarder;
    forwarding_ = forwarder.forward(annotations_);
    if (forwarding_.status == ForwardingStatus::Rejected) {
        error = "annotation forwarding rejected";
        return false;
    }
    if (forwarding_.status == ForwardingStatus::Multiple) {
        error = "multiple @next annotations require an explicit multi-destination policy";
        return false;
    }
    error.clear();
    return true;
}
} // namespace sleela::annotation
