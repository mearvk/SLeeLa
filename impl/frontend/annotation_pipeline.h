#pragma once
#include "../annotation/Annotation.hpp"
#include <string>
#include <vector>
namespace sleela::frontend {
struct AnnotationDiagnostic {
    enum class Severity { Warning, Error };
    Severity severity;
    std::string message;
};
bool isKnownLanguageAnnotation(const std::string& name) noexcept;
bool annotationMayRepeat(const std::string& name) noexcept;
std::vector<AnnotationDiagnostic> validateLanguageAnnotations(const annotation::DocumentAnnotations& annotations);
} // namespace sleela::frontend
