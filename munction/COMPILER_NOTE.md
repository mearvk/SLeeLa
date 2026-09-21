# Munction™ — Compiler & Version Note (syntax 1.3)

**Attention:** Max Rupplin — MEARVK LLC — 2026
See [`../MUNCTION.md`](../MUNCTION.md) for the normative language spec.

Munction™ is a **syntax 1.3** feature. A Wrapper™ (`.sleela` source) that uses a
Munction™ sentence declares the version pragma on its first non-blank,
non-comment line:

```text
#sleela 1.3
```

## No new grammar tokens

Like the Sleela network layer (NETWORK.md §3, §10), Munction™ requires **no new
lexer or parser keywords**. `Munction.start(...)` is an ordinary qualified call,
and the rest of the chain (`.connect(...).send(...).consume().latch()
.closeWithReceipt()`) is ordinary fluent member-call syntax the parser already
accepts (SLEELA.syntax §5). Recognition and gating happen at the compiler's
built-in dispatch point, exactly where the six network built-ins are recognized
(`impl/frontend/compiler.cpp`, `tryEmitBuiltin`).

## What the compiler gate enforces

When the compiler sees a chain rooted at the contextual name `Munction`:

1. **Version gate.** The *meaning* of `Munction` is version-gated. Under an
   explicitly declared `#sleela 1.2` (or lower) program, a `Munction` chain is
   rejected — the same discipline the network built-ins use (NETWORK.md §10).
   This requires raising the front end's supported syntax range to include 1.3:

   ```cpp
   // impl/frontend/version.h
   inline SyntaxVersion maxSupportedSyntax() { return SyntaxVersion{1, 3}; }
   ```

2. **Sanity bound.** The sentence must contain **4 to 16 calls inclusive**
   (opener + middles + closer). Outside the bound is rejected, not truncated or
   extended (MUNCTION.md §1.2).

3. **Legal sequence.** Exactly one opener (`start`) first; exactly one closer
   (`closeWithReceipt` / `close` / `abort`) last; every data verb (`send`,
   `consume`, `propagate`) follows a `connect`; `latch` at most once and before
   the closer (MUNCTION.md §3.1).

4. **Boundary, not repair.** A chain that violates the shape stops at the
   boundary with a semantic error; the compiler never silently repairs it.

## Registration table, not grammar growth

The channel set (pipe / file / tcp / http / sdps / crypto) is a **registration
table** (`MunctionChannels` on the Java side). Reaching a new *system method* is
a channel registration, not a grammar change — the Munction™ sentence stays
fixed while the runtime vocabulary grows through compiler/core coordination.

## Version awareness summary

| Item | Value |
|---|---|
| Syntax version introducing Munction™ | **1.3** |
| Supported range after this feature | `1.0 .. 1.3` |
| New lexer/parser tokens | none |
| Recognition point | compiler built-in dispatch (`tryEmitBuiltin`) |
| Call-count bound | 4 .. 16 inclusive |
| Rejection under `#sleela 1.2` | required (meaning is version-gated) |

## Reference layout

```text
munction/
  COMPILER_NOTE.md              this note
  examples/
    reach_xyx.sleela            the canonical sentence (sdps://)
    reach_all_methods.sleela    one grammar over every system method
    EXPECTED_OUTPUT.txt         reference program output
  java/com/mearvk/sleela/munction/
    Munction.java               the fluent engine (bound + sequence enforced)
    MunctionChannel.java         the receivability + coherent-send contract
    MunctionChannels.java        scheme -> channel registry
    Interims.java / Reception.java / SendCoherence.java / ChannelReceipt.java
    MunctionReceipt.java         the witnessed receipt
    MunctionDemo.java            Java launcher for the same sentences
    channels/                    pipe · file · tcp · http · sdps · crypto
```
