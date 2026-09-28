# Regex Natural Form Build

Version: 1.1.0-dev

Natural Form lives under `regex/natural` and has C, C++, Java, and SLeeLa representations. Native tests are available through:

```bash
make -C regex natural
make -C regex java
make -C regex test
```

The disposable build directory is `regex/build`. No generated binaries belong in source control.

The language contract is defined by `regex/natural/SYMBOLS.md` and `regex/natural/GRAMMAR.md`. Backends may implement a subset, but must report unsupported constructs rather than silently accepting them.
