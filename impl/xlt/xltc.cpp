// ===========================================================================
// xltc.cpp  --  The XLT (.xlt) CPU-model compiler CLI.
//
// Usage:
//   xltc <file.xlt>        compile a model; print SUCCESS + build list, or the
//                          congruence report of why it could not be built.
//   xltc --check <file>    same, but always exit 0 (report-only).
//
// Exit code: 0 when the model built, 1 when it did not (or on I/O error), so
// the tool composes into build pipelines. The output contract matches the
// Sleela-side SLXltCompiler / SLXltBuildResult.
// ===========================================================================
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#include "xlt_lexer.h"
#include "xlt_parser.h"
#include "xlt_resolve.h"
#include "xlt_diagnostics.h"

int main(int argc, char** argv) {
    bool checkOnly = false;
    std::string path;
    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--check") checkOnly = true;
        else path = a;
    }
    if (path.empty()) {
        std::fprintf(stderr, "usage: xltc [--check] <file.xlt>\n");
        return 1;
    }

    std::ifstream in(path);
    if (!in) { std::fprintf(stderr, "xltc: cannot open %s\n", path.c_str()); return 1; }
    std::stringstream ss; ss << in.rdbuf();
    std::string src = ss.str();

    xlt::DiagnosticBag diags;
    auto toks = xlt::lex(src, path, diags);
    xlt::XltModel model = xlt::parseModel(toks, path, diags);
    xlt::ResolveResult res = xlt::resolve(model, diags);

    std::string name = model.name.empty() ? path : model.name;
    std::fputs(xlt::renderResult(name, res).c_str(), stdout);

    if (checkOnly) return 0;
    return res.built ? 0 : 1;
}
