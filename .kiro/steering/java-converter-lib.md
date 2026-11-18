---
inclusion: always
---

# lib/java/converter: standing decisions (repository owner, updated 2026-11-18)

Repo-scoped, always-on. `lib/java/converter` is the SLeeLa-language Java to SLeeLa
converter (README: `lib/java/converter/README.md`). It is separate from
`sleelactl java-convert` (`.kiro/steering/java-to-sleela-conversion.md`).

1. **Per-member stubs.** A member that cannot be converted becomes a stub that
   throws `UnsupportedOperationException` naming the reason, with its Java body
   kept as a comment; the rest of the type is still written. Do not switch this
   converter to whole-file refusal.
2. **Output syntax stays 3.1 (also written 3.01, SLEELA.syntax 44.1).** Converted
   files declare `#sleela 3.1` and use only forms up to section 44. Do not adopt
   syntax 3.1 forms (sections 45, 46: typed char text, read-only `a.length`,
   `new T[a][b]`) unless the owner asks for it.
3. **Shared semantic laws come from the model.** Type/rank/promotion, operator,
   cast, widening, string-conversion, identifier-replacement and built-in
   qualification laws are read from `impl/subjects/conversion/conversion.model`
   (`JConvModel`). Where both converters apply a law, they use the same row.
   Change a law in the model, not in either converter.
4. **Verify by output.** After a converter change: convert
   `lib/java/converter/samples` with `JCONV_BUNDLE=1`, run it, and diff against
   `samples/ConvDemo.java.expected.txt` and `samples/LambdaDemo.java.expected.txt`
   (all lines identical; the samples run from `demo/` and `lambda/`); convert the JDK
   sample and confirm **0 syntax errors** in the output. Record numbers in the
   README section 6 with the date. Long suites follow `validation-cadence.md`.
5. **Lambdas are lowered, not given syntax.** `JConvLambda` writes the
   `(id, env)` form of `lib/java/jdk-28/LOWERING.md` section 2 and relies on
   calls on `Any` (SLEELA.syntax 43.7). Keep 0 syntax errors: generated tokens
   go through `syn` (not `rewriteIdent`, which renames `Any`). Ids are per
   document, so a runnable program with lambdas must be one `JCONV_BUNDLE=1`
   document.

## Working notes (2026-11-18, converter 0.7.9): how to verify fast

- **Owner priority:** keep the translation and `impl/subjects/conversion/conversion.model`
  authoritative before implementing exact JDK library behaviour. Model 1.5.0 now
  carries primitive type/rank/promotion, the full numeric cast matrix, binary32
  rules, context-aware renames and built-in qualification. The C++ loader
  (`sleelactl/convert/conv_model.c`) fails closed on an unknown row kind: add
  each new kind to its `fields_for` table. The current 0.7.9/model 1.5.0 changes
  still need the native sample regression and C++ differential suites; the trusted
  SHA-256 manifests also need regeneration from a full checkout. Do not describe
  these changes as verified until those steps pass.
- **Regression after any converter change (about 3 min):** convert the 17-file JDK
  sample and the samples, then diff against the previous output. Each
  `samples/<dir>/<Name>.java` has a `<Name>.java.expected.txt` from OpenJDK. Convert
  with `JCONV_IN=lib/java/converter/samples JCONV_FILTER=<dir>/ JCONV_SCOPE=all
  JCONV_BUNDLE=1`, run the `.sleela` with `impl/build/sleela`, and diff. Last recorded verified set: `demo` (ConvDemo), `lambda`, `classlit`, `model`,
  `nextname`, `qualify`, all identical before the 0.7.9/model 1.5.0 changes.
- **JDK source:** OpenJDK 25.0.2 `src.zip` is under the mise Java installs; extract
  with Python `zipfile` (a shell `unzip` with globs stalled). The whole public JDK
  (`JCONV_SCOPE=api`) converts in about 4 min with the default `JCONV_BATCH=500`.
- **Sandbox:** a detached background job is killed when the tool call ends. Run long
  conversions in the foreground with a long timeout.
- **Every edit under `lib/**` or `impl/` needs** `python3 tools/generate-sha256-manifest.py
  --root . --output security/sha256-manifest.json`, or `run` refuses to start. A new
  `lib` file also needs the inventory updated (`tools/generate-library-symbols.py`, the
  four `EXPECTED_*` in `test-suites/test-library-inventory.sh`, the `LIBRARY.INDEX.md`
  counts and a revision note). After a VM or compiler change, rebuild `sleela` and
  `nordshrift`, copy them to `working-binaries/`, and update the hashes in its README.
- **VM limits that shape the converter:** plain Strings are never freed (hence
  `JCONV_BATCH`); at most 64 instance fields per class (`JConvRewriter` is near it, so
  put new state in a helper class such as `JConvClassLit`); a bare `next` is the
  integer 1; `require`, `close`, `print`, `record`, `String` are not usable names.
  The measured list is `impl/subjects/conversion/IDENTIFIER-COLLISIONS.md`.
- **Rename rules are context-aware.** Shared `rename` rows carry a context;
  `next` is `value`-only and `JConvRewriter.renameNextVariables` reads that row,
  preserving calls such as `it.next()`. `close` maps to `closeResource`.
- **Open items:**
  - `connect` and `send` still need member-context rewriting for declarations
    and calls on converted source types; qualification alone does not solve
    receiver-independent Munction verbs.
  - Native float compound-assignment lowering and cross-converter float
    differential verification remain open.
  - A type's own Javadoc is dropped from the output.
  - Many `jdk-28` stubs return defaults instead of throwing.
