#include "annotation_language_bridge.hpp"

namespace sleela::server {

bool AnnotationLanguageBridge::ingest(
    const std::string& holdingDocument,
    const annotation::DocumentAnnotations& annotations,
    AnnotationContext& context,
    std::string& error) const noexcept {
    annotation::AnnotationRuntime runtime;
    if (!runtime.install(annotations, error)) {
        context = AnnotationContext{};
        return false;
    }
    context.holdingDocument = holdingDocument;
    context.nexterColony = runtime.nexterColony();
    context.forwardingAnnotation.clear();
    if (runtime.hasForwarding()) {
        const auto* next = annotations.first("next");
        if (next) context.forwardingAnnotation = next->value;
    }
    context.valid = true;
    error.clear();
    return true;
}

} // namespace sleela::server
