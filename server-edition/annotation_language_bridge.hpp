#pragma once
#include "../impl/annotation/AnnotationRuntime.hpp"
#include <string>

namespace sleela::server {

struct AnnotationContext {
    std::string holdingDocument;
    std::string forwardingAnnotation;
    std::string nexterColony;
    bool valid = false;
};

class AnnotationLanguageBridge {
public:
    bool ingest(const std::string& holdingDocument,
                const annotation::DocumentAnnotations& annotations,
                AnnotationContext& context,
                std::string& error) const noexcept;
};

} // namespace sleela::server
