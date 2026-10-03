# JDK 28 SLeeLa Source Set

The Java support surface is treated as a native SLeeLa source set.

## Required mapping

Every supported Java class has:
1. its Java package name,
2. its Java class name,
3. a matching .sleela filename,
4. a SLeeLa declaration,
5. its Java binary name,
6. a path through the Java conformance boundary for Java behavior.

## Execution model

1. User writes SLeeLa.
2. Loader resolves lib/java/java/.../<Class>.sleela.
3. Compiler retains the SLeeLa declaration and operation.
4. Java operations are lowered to the conformance boundary.
5. The actual Java class, method, or constructor executes on the configured JDK.
6. The result or exception returns to SLeeLa.
7. Unsupported access fails closed.

## Guarantee

This gives us a source-level SLeeLa representation of the Java API without duplicating Java's implementation.

The remaining implementation requirement is compiler/VM lowering and value marshaling so an ordinary SLeeLa expression can directly invoke the represented Java operation without a separate Java-side test program.

## Design law

The .sleela file is the source. Java is the behavior provider. SLeeLa controls the request.
