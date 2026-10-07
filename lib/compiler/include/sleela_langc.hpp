#ifndef SLEELA_LANGC_HPP
#define SLEELA_LANGC_HPP
/*
 * lib/compiler/include/sleela_langc.hpp
 * SLeeLa Compiler — multi-language front-end C++ orchestration facade.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * A thin, non-owning C++ wrapper over the stable C ABI in sleela_langc.h. It
 * orchestrates language modules and compile plans; it does not acquire VM,
 * capability, security, resolver, memory, or certificate authority, and it does
 * not execute, link, or load any input program. All questions bottom out in the
 * C registry.
 */

#include "sleela_langc.h"
#include <string>
#include <vector>

namespace sleela {
namespace compiler {

/* Phase codes mirrored for ergonomic C++ use. */
enum class Phase : uint32_t {
    Source   = SLEELA_LANG_PHASE_SOURCE,
    Lex      = SLEELA_LANG_PHASE_LEX,
    Parse    = SLEELA_LANG_PHASE_PARSE,
    Resolve  = SLEELA_LANG_PHASE_RESOLVE,
    Semantic = SLEELA_LANG_PHASE_SEMANTIC,
    IR       = SLEELA_LANG_PHASE_IR,
    Lower    = SLEELA_LANG_PHASE_LOWER,
    Codegen  = SLEELA_LANG_PHASE_CODEGEN
};

enum class Target : uint32_t {
    SLVM   = SLEELA_LANG_TARGET_SLVM,
    SLJVM  = SLEELA_LANG_TARGET_SLJVM,
    SLIR   = SLEELA_LANG_TARGET_SLIR,
    Native = SLEELA_LANG_TARGET_NATIVE
};

/* Build a phase mask from a list of phases — the ergonomic way a C++ front end
 * declares which pipeline stages it provides. */
inline uint32_t phaseMask(std::initializer_list<Phase> phases) {
    uint32_t mask = 0u;
    for (Phase p : phases) mask |= (1u << (static_cast<uint32_t>(p) - 1u));
    return mask;
}

/*
 * LanguageCompiler is the C++ base a developer extends to add a new front end.
 * A subclass fills in the module descriptor via describe(); the framework
 * registers it and plans compiles against it. The design mirrors the SLeeLa
 * `SLLanguageCompiler` contract so the two layers stay congruent.
 */
class LanguageCompiler {
public:
    virtual ~LanguageCompiler() = default;

    /* Identity + provided phases of this front end. */
    virtual sleela_lang_module_t describe() const = 0;

    /* Register this front end in the shared C registry. */
    int registerModule() const {
        sleela_lang_module_t m = describe();
        return sleela_lang_register(&m);
    }

    std::string name() const { return describe().language_name ? describe().language_name : ""; }

    bool provides(Phase p) const {
        sleela_lang_module_t m = describe();
        return sleela_lang_provides(&m, static_cast<uint32_t>(p)) != 0;
    }
};

/* A compile plan result wrapped for C++ callers. */
struct Plan {
    sleela_lang_plan_t value{};
    int status{SLEELA_LANG_INVALID};

    bool vmReady() const { return value.vm_ready != 0u; }
    bool ok() const { return status == SLEELA_LANG_OK; }
    uint32_t firstMissingPhase() const { return value.first_missing_phase; }
    std::string targetName() const { return sleela_lang_target_name(value.target); }
    std::string statusName() const { return sleela_lang_status_name(status); }
};

/*
 * Registry is the orchestration front door: register many front ends, look them
 * up, and plan compiles. It owns nothing beyond the shared C registry state.
 */
class Registry {
public:
    static void reset() { sleela_lang_registry_reset(); }
    static size_t count() { return sleela_lang_registry_count(); }

    static int add(const LanguageCompiler &frontEnd) { return frontEnd.registerModule(); }
    static int add(const sleela_lang_module_t &module) { return sleela_lang_register(&module); }

    static bool has(const std::string &languageName) {
        return sleela_lang_find(languageName.c_str()) != nullptr;
    }

    static const sleela_lang_module_t *find(const std::string &languageName) {
        return sleela_lang_find(languageName.c_str());
    }

    static const sleela_lang_module_t *findByExtension(const std::string &ext) {
        return sleela_lang_find_by_extension(ext.c_str());
    }

    /* Plan a compile of `inputPath` with the named module toward `target`. */
    static Plan plan(const std::string &languageName,
                     const std::string &inputPath,
                     Target target,
                     bool strict = false) {
        sleela_lang_request_t req{};
        req.module_name = languageName.c_str();
        req.input_path = inputPath.c_str();
        req.requested_target = static_cast<uint32_t>(target);
        req.strict = strict ? 1u : 0u;
        Plan out;
        out.status = sleela_lang_plan(&req, &out.value);
        return out;
    }
};

} // namespace compiler
} // namespace sleela
#endif
