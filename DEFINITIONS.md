# DEFINITIONS

## Static Protected Memory Model

This document defines the SLeeLa terms **static**, **protected**, **System Degree 2**, and **managed-memory boundary** as they apply to the SLeeLa source language and its C/C++ execution core.

### 1. Static

static identifies a class member as class-level rather than instance-level. The SLeeLa compiler lowers class fields into VM global storage and class methods into core functions.

A static member therefore has one language-level identity for the compiled program. It is not a statement that the member is a native memory address.

### 2. Protected

protected identifies a class member as an internal class interface.

For the current flattened SLeeLa class model:

- protected fields are accessible only from their declaring class;
- protected methods are callable only from their declaring class;
- protected members are not public source interfaces;
- a protected member MUST also be static.

The compiler rejects:

    protected int state;

and accepts the required form:

    static protected int state;

### 3. System Degree 2

**System Degree 2** is a SLeeLa safety term for the static protected reference boundary.

Its two relations are:

    class member
        |
        v
    VM-managed storage / handle

The two degrees describe the semantic relationship between the source-level class member and the runtime-managed storage representation. They do not mean two native pointer increments, two arbitrary pointer dereferences, or a license for pointer arithmetic.

The intended source form is:

    static protected int systemDegree = 2;

### 4. Pointer and Handle Distinction

SLeeLa source does not receive raw C/C++ pointers from the execution core.

Where runtime resources require identity, the core uses bounded VM-owned handles. Struct instances, sockets, files, threads, Synchro probes, Munction reaches, and Best-of selectors are examples of resources represented through managed tables.

A handle is an identifier controlled by the VM. It is not a native address.

Therefore:

    System Degree 2 != pointer arithmetic

and:

    protected source != raw pointer access

### 5. Managed-Memory Boundary

The **managed-memory boundary** is the runtime condition required before protected source can execute.

The C/C++ core reports a managed-memory safety state through:

    slvm_memory_safe_mode()

The compiler checks this state whenever a program contains protected source.

The protected-source gate therefore has two mandatory conditions:

1. the member must be static protected;
2. the active VM must report the managed-memory safety state.

If either condition is absent, compilation fails.

### 6. Memory Lifetime

SLeeLa treats runtime object identity as a lifetime-managed property.

A managed value must not be used as though it were a live object after its storage or handle has been released. The core validates handle ranges and active entries before operating on managed resources.

### 7. Static Protected Safety Rule

The source rule for this feature is:

    protected member
        => MUST be static
        => MUST execute through the managed-memory boundary

The compiler consequently rejects:

    protected int x;

while accepting:

    static protected int x = 2;

A protected method follows the same rule:

    static protected int readState() {
        return state;
    }

### 8. Access Rule

The declaring class is the protected access boundary.

A separate class cannot directly use a protected field or protected method through the flattened source namespace. The compiler records the declaring class and rejects protected access from another class.

### 9. Safety Intent

This model is intentionally conservative.

The static requirement establishes class-level identity. The protected requirement establishes a restricted source interface. The managed-memory requirement establishes that the source program cannot bypass the runtime's bounded storage/handle accounting merely by declaring a protected member.

The concepts therefore work together:

    static
       +
    protected
       +
    managed memory
       =
    protected System Degree-2 source

### 10. C/C++ Relationship

The terms are inspired by ordinary C++ language concepts but do not change the meaning of the C++ keywords themselves.

In C++, static class members are not associated with individual class instances, while protected controls member accessibility. SLeeLa adopts those concepts at its source-language boundary and adds its own managed-memory enforcement.

The C/C++ execution core remains responsible for native memory, handles, threads, sockets, files, and other operating-system resources. Sleela source remains above that boundary.

### 11. Implementation References

- impl/frontend/lexer.h and lexer.cpp — protected token;
- impl/frontend/ast.h — static/protected member metadata;
- impl/frontend/parser.h and parser.cpp — member modifier parsing;
- impl/frontend/compiler.cpp — static/protected and access enforcement;
- impl/core/sleela_core.h — managed-memory API;
- impl/core/sleela_core.c — managed-memory safety state;
- impl/examples/static-protected.sleela — source example.

## Definition Status

These definitions establish the current SLeeLa implementation contract for static protected source. They should remain synchronized with the lexer, parser, AST, compiler, and C/C++ execution core whenever the language memory or access model changes.
