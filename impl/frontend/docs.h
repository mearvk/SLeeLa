// docs.h -- the `sleela docs` subcommand: terminal-reachable SleelaDoc.
// ===========================================================================
// SleelaDoc is the SLeeLa documentation brand. This module makes it reachable
// straight from the command line:
//
//     sleela docs                 list every documentable topic (package/class)
//     sleela docs crypto          the crypto package: its classes and purpose
//     sleela docs stringbuilder   a single class, resolved case-insensitively
//     sleela docs strbuilder      a near match -> ranked "did you mean" results
//
// It resolves a topic against the live /lib index (the same discovery the
// compiler uses) and renders a rich, studyable plain-text page. When a
// pre-generated text corpus is present under api/sleela/docs (produced by
// tools/generate-sleeladoc-text.py) it is preferred; otherwise the page is
// parsed live from the .sleela source so the command always works.
#ifndef SLEELA_DOCS_H
#define SLEELA_DOCS_H
namespace sleela {
// Entry point for the `docs` subcommand. argv[0] is "docs"; argv[1..] are the
// topic words (joined). Returns a process exit code.
int docsCmd(int argc, char** argv);
}
#endif
