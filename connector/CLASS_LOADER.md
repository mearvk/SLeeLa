# SleelaClassLoader — intelligent loader of raw compiled classes

`SleelaClassLoader`
([`connector/java/.../SleelaClassLoader.java`](java/com/mearvk/sleela/connector/SleelaClassLoader.java))
is a `Class.forName()`-style facility for the SLeeLa → Java bridge — but
**hardened**. It resolves raw compiled classes (from the classpath or from
operator-supplied `.class` directories / `.jar` files) under an explicit
allow-list, with caching and fail-closed behavior.

## Why not just `Class.forName(userString)`?

A bare `Class.forName()` on a user-supplied name is a real hazard: it can
resolve and **static-initialize any class** on the classpath, which is a
code-execution / deserialization risk. `SleelaClassLoader` keeps the
convenience but removes the danger, matching the allow-list posture already used
by the java28 memory server.

## What "intelligent" means

| Property | Behavior |
|---|---|
| **Allow-listed** | A class resolves only if its exact FQCN is allowed, or it is under an allowed package prefix. |
| **Explicit roots** | `addRoot(dirOrJar)` layers raw `.class` directories and `.jar` files via a `URLClassLoader` over the bridge's own loader. |
| **Non-initializing by default** | `resolve(name)` does **not** run static initializers; `resolve(name, true)` opts in. |
| **Cached** | Successful resolutions are memoized (same `Class` instance returned). |
| **Fail closed** | A disallowed / missing / unlinkable class throws `ClassResolutionException`; junk names throw `IllegalArgumentException`. Never an arbitrary load. |

Thread-safe; `AutoCloseable` (closing releases any `URLClassLoader` opened for
roots).

## Usage

```java
try (SleelaClassLoader loader = new SleelaClassLoader()) {
    loader.addRoot(Path.of("build/plugins"));   // dir of raw .class files
    loader.addRoot(Path.of("libs/widgets.jar")); // or a jar
    loader.allowPackage("com.acme");             // or allowClass("com.acme.Widget")

    if (loader.isAllowed("com.acme.Widget")) {
        Class<?> type = loader.resolve("com.acme.Widget");        // no static init
        // ... reflect / instantiate as needed ...
        Class<?> live = loader.resolve("com.acme.Widget", true);  // run static init
    }
}
```

## Errors

- `ClassResolutionException` — not allow-listed, not found on any root, or failed
  to link (bad/incompatible bytecode in a raw `.class`).
- `IllegalArgumentException` — the name is blank or not a valid Java binary name
  (guards against injection-style input).
