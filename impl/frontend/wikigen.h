// wikigen.h -- the wikitext/wikicode emanation (syntax 2.4, third output).
// ===========================================================================
// A major SLeeLa work, when author-marked, can emanate a MediaWiki wikitext
// document as a consequence of compilation -- a third output alongside the
// runnable artifact and the originated source document (see originate.h). The
// page is factual and structured: an infobox, a wikitable of every class and
// member with its declaration-order id and SHA-256 brander, and a "Movement"
// section that lays the members out in declaration order (the tempo of the
// work) against a temporal axis (the source edit date and the compile stamp).
// The output scales with the size of the program; a larger work produces a
// larger corpus. It is MediaWiki markup for a self-hosted wiki -- not a
// submission to any public encyclopedia.
#ifndef SLEELA_WIKIGEN_H
#define SLEELA_WIKIGEN_H
#include <string>
namespace sleela {
struct Program;
// Write the wikitext document for `prog`.
//  srcPath -- the original source path (base name = doc name; mtime = edit date).
//  wikiDir -- directory to write into; empty means the source directory.
// Returns the written path, or "" on failure (message on std::cerr).
std::string writeWikitextDocument(const Program& prog,
                                  const std::string& srcPath,
                                  const std::string& wikiDir);
} // namespace sleela
#endif // SLEELA_WIKIGEN_H
