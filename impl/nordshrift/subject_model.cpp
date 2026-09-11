#include "subject_model.h"

namespace nordshrift::semantic {

bool isValidEvidenceStatus(EvidenceStatus status) {
    switch (status) {
    case EvidenceStatus::Observed:
    case EvidenceStatus::Specified:
    case EvidenceStatus::Derived:
    case EvidenceStatus::Modeled:
    case EvidenceStatus::Inferred:
    case EvidenceStatus::Assumed:
        return true;
    }
    return false;
}

bool isValidWorkStatus(WorkStatus status) {
    switch (status) {
    case WorkStatus::Planned:
    case WorkStatus::Ready:
    case WorkStatus::Active:
    case WorkStatus::Blocked:
    case WorkStatus::Validating:
    case WorkStatus::Complete:
    case WorkStatus::Deferred:
        return true;
    }
    return false;
}

bool hasDeclaredDependency(const Subject& subject, const std::string& dependency) {
    for (const auto& item : subject.dependencies) {
        if (item == dependency) return true;
    }
    return false;
}

bool isFoundationalDomain(const std::string& domain) {
    return domain == "math";
}

} // namespace nordshrift::semantic
