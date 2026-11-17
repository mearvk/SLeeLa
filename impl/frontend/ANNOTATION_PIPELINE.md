# Sleela Annotation Language Pipeline

Annotations are now first-class language metadata.

## Canonical path

**Wrapper source → Lexer → Parser → AST/Program → Semantic Analysis → Compiler → Runtime → Server Edition**

The annotation payload is carried by Program::annotations and is never discarded merely
because the compiler has lowered executable statements.

## Surface syntax

A document annotation is `@name(value)`; the parentheses are required for a value, and a bare `@name`
is a marker with no value. Document annotations come after the optional/required #sleela version pragma
and before the first class. They must be followed by something other than the keyword `class` itself
(a modifier such as `public`, an import, or a struct): an annotation written directly before `class`
binds to that class instead of to the document.

    @scope(system)
    @area(network)
    @responsibility(transport)
    @provider(posix-socket)
    @source(impl/network/TcpTransport.cpp)
    @capability(network.connect)
    @next(system/network/multiplexing)

    public class Example {
        void main() { print("annotation-aware"); }
    }

The parser reads the text inside the parentheses as source tokens and joins them with single spaces,
so a value is a sequence of tokens, not raw text: `system/network` is stored as `system / network`.
Architecture paths, source paths, release identifiers, counters, and capability names fit this form
without a second expression grammar. A backslash is not a legal source character.

## Stage contracts

1. Lexer recognizes @ as a language token while retaining line/column data.
2. Parser consumes document annotation lines and stores them on the Program.
3. AST owns the annotation collection; annotations are part of the parsed program.
4. Semantic analysis validates known annotations, repetition policy, and safe @next
   destinations: a destination must be relative (no leading `/`, no drive or scheme `:`), must not contain a
   parent component (`..`, also when the tokens are spaced apart), a backslash, a `%`, or a control character. Unknown annotations are preserved with warnings.
5. Compiler refuses semantic-invalid annotation programs before bytecode emission.
6. Runtime installs the same annotation collection and resolves the forwarding decision
   through AnnotationForwarder. Runtime forwarding never bypasses capabilities or routing
   policy.
7. Server Edition receives the same metadata and can expose the Holding Document,
   Forwarding Annotation, Nexter Colony, trace, and HTTP-generation boundary.

## Safety

@next is architectural continuation metadata. Runtime forwarding is a separate,
explicit operation. A source annotation cannot directly execute a destination, grant a
capability, or bypass SourceRouter policy.

Multiple @next declarations remain legal language metadata but are rejected by the
runtime until a multi-destination forwarding policy is explicitly selected.

Max Rupplin — MEARVK LLC — 2026
