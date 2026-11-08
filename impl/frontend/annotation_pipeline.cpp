#include "annotation_pipeline.h"
#include "../annotation/ForwardingAnnotation.hpp"
#include <set>
namespace sleela::frontend {
bool isKnownLanguageAnnotation(const std::string& name) noexcept {
    static const std::set<std::string> names = {"scope","area","next","responsibility","provider","source","requires","capability","group","counter","stage","release","runbook","supporter","modium","registry","native"};
    return names.count(name) != 0;
}
bool annotationMayRepeat(const std::string& name) noexcept {
    return name == "next" || name == "requires" || name == "capability" || name == "counter" || name == "supporter";
}
std::vector<AnnotationDiagnostic> validateLanguageAnnotations(const annotation::DocumentAnnotations& annotations) {
    std::vector<AnnotationDiagnostic> out;
    std::set<std::string> seen;
    for (const auto& a : annotations.all()) {
        if (a.name.empty()) {
            out.push_back({AnnotationDiagnostic::Severity::Error, "annotation name must not be empty"});
            continue;
        }
        // Marker annotations (no parentheses, e.g. @native) legitimately carry
        // no value; only a value-form annotation (written with parentheses) is
        // required to be non-empty.
        if (!a.marker && a.value.empty()) {
            out.push_back({AnnotationDiagnostic::Severity::Error, "annotation '@" + a.name + "(...)' must not have an empty value"});
            continue;
        }
        if (!isKnownLanguageAnnotation(a.name)) {
            out.push_back({AnnotationDiagnostic::Severity::Warning, "unknown annotation '@" + a.name + "' is preserved as metadata"});
        } else if (!annotationMayRepeat(a.name) && !seen.insert(a.name).second) {
            out.push_back({AnnotationDiagnostic::Severity::Error, "annotation '@" + a.name + "' may occur only once"});
        }
        if (a.name == "next" && !annotation::ForwardingPolicy::valid_destination(a.value)) {
            out.push_back({AnnotationDiagnostic::Severity::Error, "invalid @next destination '" + a.value + "'"});
        }
    }
    return out;
}
} // namespace sleela::frontend
