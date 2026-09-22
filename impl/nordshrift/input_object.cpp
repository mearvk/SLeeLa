#include "input_object.h"

namespace nordshrift {
namespace {
size_t bounded(const std::string& s, size_t max) {
    return s.size() <= max ? s.size() : max + 1;
}
}

bool inputObjectCategoryFromName(const std::string& s, InputObjectCategory& out) {
    if (s == "core")       { out = InputObjectCategory::Core; return true; }
    if (s == "io")         { out = InputObjectCategory::IO; return true; }
    if (s == "system")    { out = InputObjectCategory::System; return true; }
    if (s == "network")   { out = InputObjectCategory::Network; return true; }
    if (s == "application"){ out = InputObjectCategory::Application; return true; }
    if (s == "data")      { out = InputObjectCategory::Data; return true; }
    if (s == "science")   { out = InputObjectCategory::Science; return true; }
    if (s == "security")  { out = InputObjectCategory::Security; return true; }
    if (s == "deployment"){ out = InputObjectCategory::Deployment; return true; }
    return false;
}

const char* inputObjectCategoryName(InputObjectCategory c) {
    switch (c) {
        case InputObjectCategory::Core: return "core";
        case InputObjectCategory::IO: return "io";
        case InputObjectCategory::System: return "system";
        case InputObjectCategory::Network: return "network";
        case InputObjectCategory::Application: return "application";
        case InputObjectCategory::Data: return "data";
        case InputObjectCategory::Science: return "science";
        case InputObjectCategory::Security: return "security";
        case InputObjectCategory::Deployment: return "deployment";
    }
    return "core";
}

bool inputObjectValidate(const InputObject& o, std::string& error) {
    if (o.identity.empty()) { error = "input object requires an identity"; return false; }
    if (bounded(o.identity, 256) > 256) { error = "input object identity exceeds 256 characters"; return false; }
    if (bounded(o.type, 128) > 128) { error = "input object type exceeds 128 characters"; return false; }
    if (bounded(o.source, 4096) > 4096 || bounded(o.target, 4096) > 4096) {
        error = "input object source/target exceeds 4096 characters"; return false;
    }
    if (o.inputs.size() > 64 || o.outputs.size() > 64 || o.properties.size() > 64) {
        error = "input object contains too many members"; return false;
    }
    return true;
}

} // namespace nordshrift
