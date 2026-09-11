// ===========================================================================
// sheet_model.h  --  The Nordshrift sheet model (NS-SST-0001 §II–§XIII).
// ===========================================================================
#ifndef NORDSHRIFT_SHEET_MODEL_H
#define NORDSHRIFT_SHEET_MODEL_H

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace nordshrift {

enum class Layout   { MirrorSource, Flat, PackageMapped, Custom };
enum class Verbosity{ Silent, Quiet, Normal, Verbose, Debug };
enum class Severity { Error, Warning, Notice };
enum class EffectPolicy { Strict, Lenient, Audit, Off };
enum class GuardMode { CompileOnly, RuntimeOnly, CompileAndRuntime, Off };
enum class FailureAction { Throw, Assert, LogAndContinue, Halt };
enum class NullWrap { Maybe, NullableAnnotation, Trust };
enum class CheckedEx { Wrap, Propagate, RethrowUnchecked };
enum class DeriveStyle { Record, ImmutableClass, Builder, Lombok };

enum class TargetLang { Java, Sleela, C };

// Network objects are first-class SST declarations.  These names correspond
// to the Nordshrift network model and are deliberately closed so an SST sheet
// cannot silently request an unsupported runtime object.
enum class NetworkObject {
    Endpoint,
    NIC,
    Link,
    Packet,
    Queue,
    Switch,
    Router,
    Fabric,
    Listener,
    Connector,
    Gateway,
    LoadBalancer,
    Service,
    TLS,
    DNS
};

enum class NetworkTransport { TCP, UDP, TLS, QUIC };
enum class NetworkAddressFamily { IPv4, IPv6, Dual };

struct NetworkSpec {
    std::vector<NetworkObject> objects;
    std::vector<NetworkTransport> transports;
    NetworkAddressFamily addressFamily = NetworkAddressFamily::Dual;
    bool tls = false;
    bool present = false;
    int line = 0;
};

struct Pragmas {
    std::string nordshrift;
    std::string sleela;
    std::string encoding;
    bool hasNordshrift = false;
    int nordshriftLine = 0;
};

struct SheetMeta {
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::string extends;
    std::vector<std::string> tags;
    int line = 0;
    bool present = false;
};

struct Import {
    std::string path;
    std::string alias;
    std::vector<std::string> only;
    std::vector<std::string> except;
    bool hasOnly = false;
    bool hasExcept = false;
    int line = 0;
};

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

struct Target {
    std::string root;
    Layout layout = Layout::MirrorSource;
    int javaVersion = 0;
    std::string packageRoot;
    bool overwrite = true;
    bool sourceMap = true;
    bool optimize = false;
    bool clean = false;
    std::map<std::string, std::string> layoutMap;
    TargetLang language = TargetLang::Java;
    bool present = false;
    bool hasRoot = false;
    bool hasLayout = false;
    bool hasJavaVersion = false;
    int line = 0;
    int javaVersionLine = 0;
};

struct Pipeline {
    std::vector<std::string> phases;
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

struct RuleConfig {
    std::string name;
    Severity severity = Severity::Error;
    bool hasSeverity = false;
    std::map<std::string, std::string> config;
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

struct Effects {
    EffectPolicy policy = EffectPolicy::Strict;
    std::vector<std::string> declare;
    std::map<std::string, std::vector<std::string>> aliases;
    std::string defaultEffect = "Pure";
    bool present = false;
    int line = 0;
};

struct Derive {
    bool lens = false, projection = false, equality = false, hashcode = false;
    bool toStringD = false, builder = false, copy = false;
    bool hasBuilder = false;
    DeriveStyle targetStyle = DeriveStyle::Record;
    bool hasTargetStyle = false;
    bool present = false;
    int line = 0;
};

struct Guards {
    GuardMode mode = GuardMode::CompileAndRuntime;
    FailureAction onFailure = FailureAction::Throw;
    std::string messageFormat;
    bool present = false;
    int line = 0;
};

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

struct Sheet;
struct Profile {
    std::string name;
    std::string inherits;
    bool hasInherits = false;
    std::shared_ptr<Sheet> overlay;
    int line = 0;
};

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
    NetworkSpec network;
    std::vector<RuleConfig> ruleBlocks;
    std::vector<Profile>    profiles;
    std::string file;
};

} // namespace nordshrift

#endif // NORDSHRIFT_SHEET_MODEL_H
