<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# Very Senior — BootstrapCompiler

**Goal:** design and validate a bootstrap path for a compiler written in SLeeLa. Writing a compiler in SLeeLa does not, by itself, prove it is self-hosting.

## Stages

- **Stage 0:** a trusted existing toolchain compiles the SLeeLa compiler source.
- **Stage 1:** the resulting compiler compiles that source again.
- **Stage 2:** compare normalized outputs and metadata, accounting for documented nondeterministic fields.
- **Cross-check:** compile a fixed corpus with both stages and compare diagnostics, IR, and artifacts.

The SLeeLa workbench is an interface; the authoritative native frontend currently remains under `/impl/frontend`. Keep that distinction in reports until bridge integration and language coverage are proven end-to-end.

## Required controls

Pin source revision, compiler version, target, flags, and dependencies. Use clean isolated build directories and explicit output paths. Keep native compilation disabled by default, reject path traversal, validate artifact format and instruction set before VM admission, record hashes, and never execute the generated artifact during compilation.

See [BootstrapCompiler.sleela](BootstrapCompiler.sleela) for a state model, not a claim that self-hosting is complete.

## Experiments

- Repeat Stage 0 and compare hashes.
- Compare Stage 0 and Stage 1 normalized outputs.
- Differential-test a fixed source corpus.
- Test invalid source, corrupted IR, unsupported opcode, missing dependencies, and disallowed output paths.
- Bound input size, nesting, memory, and compile time.