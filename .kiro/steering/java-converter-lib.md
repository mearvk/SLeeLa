---
inclusion: always
---

# lib/java/converter: standing decisions (repository owner, 2026-11-13)

Repo-scoped, always-on. `lib/java/converter` is the SLeeLa-language Java to SLeeLa
converter (README: `lib/java/converter/README.md`). It is separate from
`sleelactl java-convert` (`.kiro/steering/java-to-sleela-conversion.md`).

1. **Per-member stubs.** A member that cannot be converted becomes a stub that
   throws `UnsupportedOperationException` naming the reason, with its Java body
   kept as a comment; the rest of the type is still written. Do not switch this
   converter to whole-file refusal.
2. **Output syntax stays 3.1 (also written 3.01, SLEELA.syntax 44.1).** Converted
   files declare `#sleela 3.1` and use only forms up to section 44. Do not adopt
   syntax 3.2 forms (sections 45, 46: typed char text, read-only `a.length`,
   `new T[a][b]`) unless the owner asks for it.
3. **Laws come from the shared model.** Operator, cast, string-conversion and
   result-type laws are read from `impl/subjects/conversion/conversion.model`
   (`JConvModel`). Where both converters apply a law, they use the same row.
   Change a law in the model, not in the converter.
4. **Verify by output.** After a converter change: convert
   `lib/java/converter/samples` with `JCONV_BUNDLE=1`, run it, and diff against
   `samples/ConvDemo.java.expected.txt` (all lines identical); convert the JDK
   sample and confirm **0 syntax errors** in the output. Record numbers in the
   README section 6 with the date. Long suites follow `validation-cadence.md`.
