#ifndef NORDSHRIFT_SUBJECT_MODEL_H
#define NORDSHRIFT_SUBJECT_MODEL_H

#include <string>
#include <vector>

namespace nordshrift::semantic {

enum class EvidenceStatus {
    Observed,
    Specified,
    Derived,
    Modeled,
    Inferred,
    Assumed
};

enum class WorkStatus {
    Planned,
    Ready,
    Active,
    Blocked,
    Validating,
    Complete,
    Deferred
};

struct Quantity {
    std::string identity;
    std::string value;
    std::string unit;
    std::string dimension;
    std::string domain;
    EvidenceStatus status = EvidenceStatus::Specified;
};

struct Assumption {
    std::string identity;
    std::string statement;
    std::string scope;
    std::string source;
    EvidenceStatus status = EvidenceStatus::Specified;
};

struct Relation {
    std::string identity;
    std::string formula;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
};

struct Transformation {
    std::string identity;
    std::string source;
    std::string operation;
    std::string parameters;
    std::string destination;
    bool approximate = false;
};

struct ComparativeNorm {
    std::string prior_subject;
    std::string current_subject;
    std::string reference_subject;
    std::string norm;
};

struct Evidence {
    EvidenceStatus status = EvidenceStatus::Specified;
    std::string source;
    std::string note;
};

struct Explanation {
    std::string subject;
    std::vector<std::string> steps;
};

struct Todo {
    std::string identity;
    std::string subject;
    int priority = 0;
    std::vector<std::string> dependencies;
    std::string preconditions;
    std::string action;
    std::string expected_result;
    std::string validation;
    WorkStatus status = WorkStatus::Planned;
};

struct Subject {
    std::string identity;
    std::string domain;
    std::vector<std::string> dependencies;
    std::vector<Quantity> quantities;
    std::vector<Assumption> assumptions;
    std::vector<Relation> relations;
    std::vector<Transformation> transformations;
    std::vector<ComparativeNorm> comparisons;
    std::vector<Evidence> evidence;
    std::vector<Explanation> explanations;
    std::vector<Todo> workplan;
};

bool isValidEvidenceStatus(EvidenceStatus status);
bool isValidWorkStatus(WorkStatus status);
bool hasDeclaredDependency(const Subject& subject, const std::string& dependency);
bool isFoundationalDomain(const std::string& domain);

} // namespace nordshrift::semantic

#endif
