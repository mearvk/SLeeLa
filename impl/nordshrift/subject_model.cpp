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

bool evidenceStatusFromName(const std::string& s, EvidenceStatus& out) {
    if (s == "observed")  { out = EvidenceStatus::Observed;  return true; }
    if (s == "specified") { out = EvidenceStatus::Specified; return true; }
    if (s == "derived")   { out = EvidenceStatus::Derived;   return true; }
    if (s == "modeled")   { out = EvidenceStatus::Modeled;   return true; }
    if (s == "inferred")  { out = EvidenceStatus::Inferred;  return true; }
    if (s == "assumed")   { out = EvidenceStatus::Assumed;   return true; }
    return false;
}

bool workStatusFromName(const std::string& s, WorkStatus& out) {
    if (s == "planned")    { out = WorkStatus::Planned;    return true; }
    if (s == "ready")      { out = WorkStatus::Ready;      return true; }
    if (s == "active")     { out = WorkStatus::Active;     return true; }
    if (s == "blocked")    { out = WorkStatus::Blocked;    return true; }
    if (s == "validating") { out = WorkStatus::Validating; return true; }
    if (s == "complete")   { out = WorkStatus::Complete;   return true; }
    if (s == "deferred")   { out = WorkStatus::Deferred;   return true; }
    return false;
}

const char* evidenceStatusName(EvidenceStatus status) {
    switch (status) {
        case EvidenceStatus::Observed:  return "observed";
        case EvidenceStatus::Specified: return "specified";
        case EvidenceStatus::Derived:   return "derived";
        case EvidenceStatus::Modeled:   return "modeled";
        case EvidenceStatus::Inferred:  return "inferred";
        case EvidenceStatus::Assumed:   return "assumed";
    }
    return "specified";
}

const char* workStatusName(WorkStatus status) {
    switch (status) {
        case WorkStatus::Planned:    return "planned";
        case WorkStatus::Ready:      return "ready";
        case WorkStatus::Active:     return "active";
        case WorkStatus::Blocked:    return "blocked";
        case WorkStatus::Validating: return "validating";
        case WorkStatus::Complete:   return "complete";
        case WorkStatus::Deferred:   return "deferred";
    }
    return "planned";
}

} // namespace nordshrift::semantic
