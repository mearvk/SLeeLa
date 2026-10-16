// ===========================================================================
// xlt_diagnostics.h  --  Structured diagnostics for the XLT (.xlt) front-end,
// modeled on impl/nordshrift/diagnostics.h. Every diagnostic carries a code
// XLT-{tier}-{XXXX}, the file and line, a plain-English message, and a rule
// reference. The front-end's output contract is: a parse with no errors yields
// a model to resolve (then SUCCESS + build list); a parse or resolve error
// yields a congruence report built from these diagnostics.
// ===========================================================================
#ifndef SLEELA_XLT_DIAGNOSTICS_H
#define SLEELA_XLT_DIAGNOSTICS_H

#include <string>
#include <vector>

namespace xlt {

enum class Tier { Error, Warning, Notice };

struct Diagnostic {
    std::string code;       // e.g. "XLT-E-0002"
    Tier        tier;
    std::string file;
    int         line;       // 1-based; 0 if not applicable
    std::string message;
    std::string ruleRef;    // e.g. "XLT-SRC-01"

    std::string tierWord() const {
        switch (tier) { case Tier::Error: return "error";
                        case Tier::Warning: return "warning";
                        default: return "notice"; }
    }
};

class DiagnosticBag {
public:
    void error(const std::string& code, const std::string& file, int line,
               const std::string& message, const std::string& ruleRef) {
        add(Diagnostic{ code, Tier::Error, file, line, message, ruleRef });
    }
    void warning(const std::string& code, const std::string& file, int line,
                 const std::string& message, const std::string& ruleRef) {
        add(Diagnostic{ code, Tier::Warning, file, line, message, ruleRef });
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
            out += "\n";
        }
        return out;
    }

private:
    std::vector<Diagnostic> diags_;
    int errorCount_ = 0;
    int warningCount_ = 0;
};

} // namespace xlt

#endif // SLEELA_XLT_DIAGNOSTICS_H
