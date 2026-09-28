#include "annotation_pipeline.h"
#include "../annotation/ForwardingAnnotation.hpp"
#include <set>
namespace sleela::frontend {
bool isKnownLanguageAnnotation(const std::string& name) noexcept {
    static const std::set<std::string> names = {"scope","area","next","responsibility","provider","source","requires","capability","group","counter","stage","release","runbook"};
    return names.count(name) != 0;
}
bool annotationMayRepeat(const std::string& name) noexcept {
    return name == "next" || name == "requires" || name == "capability" || name == "counter";
}
std::vector<AnnotationDiagnostic> validateLanguageAnnotations(const annotation::DocumentAnnotations& annotations) {
    std::vector<AnnotationDiagnostic> out;
    std::set<std::string> seen;
    for (const auto& a : annotations.all()) {
        if (a.name.empty() || a.value.empty()) {
            out.push_back({AnnotationDiagnostic::Severity::Error, "annotation name and value must not be empty"});
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
