# Sleela Annotation Language Pipeline

Annotations are now first-class language metadata.

## Canonical path

**Wrapper source → Lexer → Parser → AST/Program → Semantic Analysis → Compiler → Runtime → Server Edition**

The annotation payload is carried by Program::annotations and is never discarded merely
because the compiler has lowered executable statements.

## Surface syntax

Document annotations are placed before the first import, struct, or class:

    @scope system
    @area network
    @responsibility transport
    @provider posix-socket
    @source impl/network/TcpTransport.cpp
    @capability network.connect
    @next system/network/multiplexing

    class Example {
        void main() { print("annotation-aware"); }
    }

The value occupies the remainder of the annotation line. This permits architecture
paths, source paths, release identifiers, counters, and capability names without
inventing a second expression grammar.

## Stage contracts

1. Lexer recognizes @ as a language token while retaining line/column data.
2. Parser consumes document annotation lines and stores them on the Program.
3. AST owns the annotation collection; annotations are part of the parsed program.
4. Semantic analysis validates known annotations, repetition policy, and safe @next
   destinations. Unknown annotations are preserved with warnings.
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
