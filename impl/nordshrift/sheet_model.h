// ===========================================================================
// sheet_model.h  --  The Nordshrift sheet model (NS-SST-0001 §II–§XIII).
//
// A parsed .sst file becomes a Sheet: pragmas + sheet metadata + optional
// imports + the configuration sections. Every field mirrors a directive from
// the spec. Fields carry a `has*` presence flag where the spec distinguishes
// "unset" (use default) from an explicit value.
//
// Triplet extension: the spec's target section is Java-only. Nordshrift drives
// a triplet, so Target adds a `language` selector (java|sleela|c) via the
// conformant-superset directive `target-language` (default java).
// ===========================================================================
#ifndef NORDSHRIFT_SHEET_MODEL_H
#define NORDSHRIFT_SHEET_MODEL_H

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace nordshrift {

// ---- enums (spec §14.2 value productions) ------------------------------
enum class Layout   { MirrorSource, Flat, PackageMapped, Custom };
enum class Verbosity{ Silent, Quiet, Normal, Verbose, Debug };
enum class Severity { Error, Warning, Notice };
enum class EffectPolicy { Strict, Lenient, Audit, Off };
enum class GuardMode { CompileOnly, RuntimeOnly, CompileAndRuntime, Off };
enum class FailureAction { Throw, Assert, LogAndContinue, Halt };
enum class NullWrap { Maybe, NullableAnnotation, Trust };
enum class CheckedEx { Wrap, Propagate, RethrowUnchecked };
enum class DeriveStyle { Record, ImmutableClass, Builder, Lombok };

// Triplet backend selector (Nordshrift superset of the spec's Java-only target).
enum class TargetLang { Java, Sleela, C };

// ---- pragmas (§1.6) -----------------------------------------------------
struct Pragmas {
    std::string nordshrift;   // required version operand, e.g. "1.0"
    std::string sleela;       // optional
    std::string encoding;     // optional
    bool hasNordshrift = false;
    int  nordshriftLine = 0;
};

// ---- sheet block (§III) -------------------------------------------------
struct SheetMeta {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::string extends;              // path
    std::vector<std::string> tags;
    int line = 0;
    bool present = false;
};

// ---- import (§IV) -------------------------------------------------------
struct Import {
    std::string path;
    std::string alias;
    std::vector<std::string> only;    // mutually exclusive with except
    std::vector<std::string> except;
    bool hasOnly = false;
    bool hasExcept = false;
    int line = 0;
};

// ---- source (§V) --------------------------------------------------------
struct Source {
    std::string root;
    std::vector<std::string> globs;
    std::vector<std::string> exclude;
    std::string encoding = "utf-8";
    bool watch = false;
    bool present = false;
    bool hasRoot = false;
    int line = 0;
    int rootLine = 0;
};

// ---- target (§VI) -------------------------------------------------------
struct Target {
    std::string root;
    Layout layout = Layout::MirrorSource;
    int javaVersion = 0;
    std::string packageRoot;
    bool overwrite = true;
    bool sourceMap = true;
    bool optimize = false;
    bool clean = false;
    std::map<std::string, std::string> layoutMap;   // custom layout

    TargetLang language = TargetLang::Java;          // triplet selector

    bool present = false;
    bool hasRoot = false;
    bool hasLayout = false;
    bool hasJavaVersion = false;
    int line = 0;
    int javaVersionLine = 0;
};

// ---- pipeline (§VII) ----------------------------------------------------
struct Pipeline {
    std::vector<std::string> phases;   // ordered phase tokens
    std::vector<std::string> skip;
    int parallelThreshold = 1;
    bool cache = true;
    std::string cacheDir = ".nordshrift-cache";
    Verbosity verbosity = Verbosity::Normal;
    bool failFast = false;
    bool present = false;
    int line = 0;
    int phasesLine = 0;
};

// ---- rules (§VIII) ------------------------------------------------------
struct RuleConfig {
    std::string name;
    Severity severity = Severity::Error;
    bool hasSeverity = false;
    std::map<std::string, std::string> config;   // key -> scalar (as text)
    int line = 0;
};
struct Rules {
    std::vector<std::string> activate;
    std::vector<std::string> deactivate;
    std::map<std::string, Severity> severity;
    std::map<std::string, std::map<std::string, std::string>> config;
    bool present = false;
    int line = 0;
};

// ---- effects (§IX) ------------------------------------------------------
struct Effects {
    EffectPolicy policy = EffectPolicy::Strict;
    std::vector<std::string> declare;
    std::map<std::string, std::vector<std::string>> aliases;
    std::string defaultEffect = "Pure";
    bool present = false;
    int line = 0;
};

// ---- derive (§X) --------------------------------------------------------
struct Derive {
    bool lens = false, projection = false, equality = false, hashcode = false;
    bool toStringD = false, builder = false, copy = false;
    bool hasBuilder = false;
    DeriveStyle targetStyle = DeriveStyle::Record;
    bool hasTargetStyle = false;
    bool present = false;
    int line = 0;
};

// ---- guards (§XI) -------------------------------------------------------
struct Guards {
    GuardMode mode = GuardMode::CompileAndRuntime;
    FailureAction onFailure = FailureAction::Throw;
    std::string messageFormat;
    bool present = false;
    int line = 0;
};

// ---- interop (§XII) -----------------------------------------------------
struct Interop {
    bool assumeImpure = false;
    NullWrap nullWrapping = NullWrap::Maybe;
    bool hasNullWrapping = false;
    CheckedEx checkedExceptions = CheckedEx::Wrap;
    std::map<std::string, std::string> typeMapping;
    std::vector<std::string> packageAllow;
    std::vector<std::string> packageDeny;
    bool present = false;
    int line = 0;
};

// ---- profile (§XIII): a named variant that overrides sections ----------
struct Sheet;   // fwd
struct Profile {
    std::string name;
    std::string inherits;      // single parent (PROF-02 forbids multiple)
    bool hasInherits = false;
    // A profile carries its own (partial) set of sections to override with.
    // Stored as a nested Sheet-like overlay via pointer to keep this header light.
    std::shared_ptr<Sheet> overlay;
    int line = 0;
};

// ---- the whole sheet ----------------------------------------------------
struct Sheet {
    Pragmas   pragmas;
    SheetMeta meta;
    std::vector<Import> imports;

    Source    source;
    Target    target;
    Pipeline  pipeline;
    Rules     rules;
    Effects   effects;
    Derive    derive;
    Guards    guards;
    Interop   interop;

    std::vector<RuleConfig> ruleBlocks;     // inline `rule Name:` blocks (§8.6)
    std::vector<Profile>    profiles;

    std::string file;   // originating .sst path
};

} // namespace nordshrift

#endif // NORDSHRIFT_SHEET_MODEL_H
