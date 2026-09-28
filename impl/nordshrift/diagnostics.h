// ===========================================================================
// diagnostics.h  --  Nordshrift structured diagnostics (NS-SST-0001 Part XV).
//
// Every diagnostic carries a structured code NSS-{tier}-{XXXX}, the .sst file
// path and line of the offending directive, a plain-English message, a pointer
// to the governing rule, and (for errors) whether it is recoverable within the
// current phase.
// ===========================================================================
#ifndef NORDSHRIFT_DIAGNOSTICS_H
#define NORDSHRIFT_DIAGNOSTICS_H

#include <string>
#include <vector>

namespace nordshrift {

enum class Tier { Error, Warning, Notice };

struct Diagnostic {
    std::string code;       // e.g. "NSS-E-0030"
    Tier        tier;
    std::string file;       // .sst path (or Sleela source path for phase 1-7)
    int         line;       // 1-based; 0 if not applicable
    std::string message;    // concrete, identifies the offending token/value
    std::string ruleRef;    // e.g. "SRC-01" or "§5.3"
    bool        recoverable;// errors only; meaningless for warning/notice

    std::string tierWord() const {
        switch (tier) { case Tier::Error: return "error";
                        case Tier::Warning: return "warning";
                        default: return "notice"; }
    }
};

class DiagnosticBag {
public:
    void error(const std::string& code, const std::string& file, int line,
               const std::string& message, const std::string& ruleRef,
               bool recoverable = false) {
        add(Diagnostic{ code, Tier::Error, file, line, message, ruleRef, recoverable });
    }
    void warning(const std::string& code, const std::string& file, int line,
                 const std::string& message, const std::string& ruleRef) {
        add(Diagnostic{ code, Tier::Warning, file, line, message, ruleRef, false });
    }
    void notice(const std::string& code, const std::string& file, int line,
                const std::string& message, const std::string& ruleRef) {
        add(Diagnostic{ code, Tier::Notice, file, line, message, ruleRef, false });
    }

    void add(const Diagnostic& d) {
        if (d.tier == Tier::Error) errorCount_++;
        else if (d.tier == Tier::Warning) warningCount_++;
        diags_.push_back(d);
    }

    bool hasErrors() const { return errorCount_ > 0; }
    int  errorCount() const { return errorCount_; }
    int  warningCount() const { return warningCount_; }
    const std::vector<Diagnostic>& all() const { return diags_; }

    // Render every diagnostic in the spec's format (Part XV §15.3).
    std::string render() const {
        std::string out;
        for (const auto& d : diags_) {
            out += d.code + " (" + d.tierWord() + "): ";
            if (!d.file.empty()) {
                out += d.file;
                if (d.line > 0) out += ":" + std::to_string(d.line);
                out += ": ";
            }
            out += d.message;
            if (!d.ruleRef.empty()) out += "  [see " + d.ruleRef + "]";
            if (d.tier == Tier::Error)
                out += d.recoverable ? "  (recoverable)" : "  (fatal)";
            out += "\n";
        }
        return out;
    }

private:
    std::vector<Diagnostic> diags_;
    int errorCount_ = 0;
    int warningCount_ = 0;
};

} // namespace nordshrift

#endif // NORDSHRIFT_DIAGNOSTICS_H
