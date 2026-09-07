// ===========================================================================
// object_compat.cpp  --  Object compatibility list + relevance conversion.
//
// The conversion is driven first by object-specific knowledge (well-known
// objects that map to a concrete target construct), then by the object's
// conduct role (a role-level default). Anything a target has no construct for
// is a Model relevance -- realized as a pattern/shape rather than a keyword.
// ===========================================================================
#include "object_compat.h"

#include <array>
#include <map>

namespace nordshrift {

const char* relevanceName(Relevance r) {
    switch (r) {
        case Relevance::Direct: return "direct";
        case Relevance::Model:  return "model";
        default:                return "none";
    }
}

namespace {

std::string langName(TargetLang l) {
    return l == TargetLang::Java ? "java" : l == TargetLang::Sleela ? "sleela" : "c";
}

// Return a direct mapping string for well-known objects, or "" if none. Only
// objects with a genuine concrete construct in a target appear here; everything
// else falls through to the role-level default below.
std::string directMapping(const std::string& obj, TargetLang t) {
    // A compact table: obj -> {java, sleela, c}
    static const std::map<std::string, std::array<const char*, 3>> tbl = {
        // core structure
        {"Class",       {"class",              "class",            "struct + functions"}},
        {"Interface",   {"interface",          "class (contract)", "struct of function pointers"}},
        {"Method",      {"method",             "method",           "function"}},
        {"Function",    {"static method",      "method",           "function"}},
        {"Constructor", {"constructor",        "method (init)",    "init function"}},
        {"Field",       {"field",              "field (global)",   "struct member / global"}},
        // control / calls
        {"Call",        {"method call",        "call",             "function call"}},
        {"SystemCall",  {"native call",        "print/builtin",    "syscall()"}},
        {"Callback",    {"functional iface",   "method ref",       "function pointer"}},
        // concurrency (maps onto the core threading model)
        {"Thread",      {"java.lang.Thread",   "spawn(...)",       "pthread_t"}},
        {"Process",     {"ProcessBuilder",     "(model)",          "fork/exec"}},
        {"Lock",        {"synchronized",       "lock(n)/unlock(n)","pthread_mutex_t"}},
        // completitives
        {"Future",      {"CompletableFuture",  "spawn + join()",   "pthread + join"}},
        {"Promise",     {"CompletableFuture",  "send/recv",        "condvar tuple"}},
        {"Join",        {"Thread.join",        "join()",           "pthread_join"}},
        // pipings / conversations
        {"Channel",     {"BlockingQueue",      "send/recv (slot)", "mailbox tuple"}},
        {"Pipe",        {"PipedStream",        "send/recv",        "pipe()"}},
        {"Message",     {"record",             "String payload",   "struct"}},
        // limits
        {"Timeout",     {"Duration",           "(model)",          "timespec"}},
        {"Bound",       {"range check",        "if-guard",         "if-guard"}},
    };
    auto it = tbl.find(obj);
    if (it == tbl.end()) return "";
    return it->second[(size_t)t];
}

// Role-level default: for roles that generally correspond to a concrete
// construct we return Direct with a generic mapping; otherwise Model.
ObjectRelevance byRole(const catalog::Object& o, TargetLang t) {
    ObjectRelevance r;
    r.object = o.name; r.role = o.role; r.note = o.note;

    // roles that are directly constructible in a general-purpose target
    const std::string& role = o.role;
    std::string lang = langName(t);

    if (role == "method" || role == "call" || role == "inheritance" || role == "start") {
        r.relevance = Relevance::Direct;
        r.mapping = lang + " construct for role '" + role + "'";
        return r;
    }
    if (role == "limit" || role == "congruence") {
        // constraints/limits become guards/checks -- direct but as a check
        r.relevance = Relevance::Direct;
        r.mapping = lang + " guard/check";
        return r;
    }
    // idiom / highlight / attention / gain / conversation / piping /
    // transitive / completitive / smart-move / root => modeled shapes
    r.relevance = Relevance::Model;
    r.mapping = "modeled as a '" + role + "' shape (" + lang + " pattern)";
    return r;
}

} // namespace

bool isCompatible(const catalog::Catalog& cat, const std::string& object) {
    return cat.find(object) != nullptr;
}

ObjectRelevance relevanceOf(const catalog::Catalog& cat,
                            const std::string& object, TargetLang target) {
    const catalog::Object* o = cat.find(object);
    if (!o) {
        return ObjectRelevance{ object, "", Relevance::None,
                                "not on the compatibility list", "" };
    }
    // object-specific direct mapping takes precedence
    std::string direct = directMapping(o->name, target);
    if (!direct.empty()) {
        return ObjectRelevance{ o->name, o->role, Relevance::Direct, direct, o->note };
    }
    return byRole(*o, target);
}

std::vector<ObjectRelevance> relevanceList(const catalog::Catalog& cat,
                                           TargetLang target) {
    std::vector<ObjectRelevance> out;
    out.reserve(cat.objects.size());
    for (const auto& o : cat.objects)
        out.push_back(relevanceOf(cat, o.name, target));
    return out;
}

} // namespace nordshrift
