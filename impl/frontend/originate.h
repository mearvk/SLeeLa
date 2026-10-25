// originate.h -- the "originated" source-document emitter (syntax 2.4).
// ===========================================================================
// When a document declares the `author`/`authorial` marker, the standard
// compile process emits, alongside the runnable artifact, a second document:
// a regenerated `.sleela` source in which every field and method is located and
// originated and carries a `∴` brander (the house analogue of Java's `@`). The
// brander records the member's declaration-order id, its SHA-256 hash, the
// document name, the member name, the parameter signature, and the document's
// edit date. This is the auditable twin of the artifact -- clean and united.
//
// The hash (the brander value) is SHA-256 over the preimage:
//     <docName>|<memberName>|<paramTypeList>|<editDate>
// where paramTypeList is the ordered, comma-separated list of the member's
// parameter types restricted to `int`/`String` (see SLEELA.syntax section 18).
#ifndef SLEELA_ORIGINATE_H
#define SLEELA_ORIGINATE_H
#include <string>
namespace sleela {
struct Program;
// Write the originated source document for `prog`.
//  srcPath     -- the original source file path (its base name is the docName
//                 and its mtime provides the edit date).
//  originateDir -- directory to write into; empty means the source directory.
// Returns the written path, or "" on failure (with a message on std::cerr).
std::string writeOriginatedDocument(const Program& prog,
                                    const std::string& srcPath,
                                    const std::string& originateDir);
} // namespace sleela
#endif // SLEELA_ORIGINATE_H
