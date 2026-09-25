# Slecompiler™ Terminology

## Source reconstruction terms

- **Source Output Target** — A language selected for generated decompiler output; Java, Sleela, C, or C++.
- **Source Emitter** — The component that converts recovered analysis evidence into source-oriented text for the selected target.
- **Output Language Selection** — The explicit CLI/API choice of Java, Sleela, C, or C++.
- **Language Projection** — Mapping common recovered analysis into a selected source language.
- **Source Reconstruction** — Evidence-based production of source-like code from recovered native structure; it is not automatically equivalent to original source.
- **Evidence-Preserving Output** — Generated source that marks unresolved or ambiguous native behavior rather than presenting unsupported conclusions as facts.
- **Original-Language Claim** — A claim that a binary was originally written in a particular source language. The selected output target does not establish this claim.

## Existing analysis terms

- **Artifact** — A native binary or binary-like input analyzed by Slecompiler.
- **Instruction** — A decoded native machine instruction.
- **Control-Flow Graph (CFG)** — A graph of basic blocks and possible execution transitions.
- **Function Recovery** — Construction of function candidates and boundaries from available evidence.
- **SLIR** — SLeeLa Intermediate Representation used to represent recovered program behavior independently of one native encoding.
- **Static Analysis** — Analysis performed without executing the target artifact.
- **Provenance** — Evidence tying a recovered fact back to artifact bytes, addresses, sections, or analysis sources.
- **Confidence** — A numerical indication of how strongly a recovered fact is supported by available evidence.
