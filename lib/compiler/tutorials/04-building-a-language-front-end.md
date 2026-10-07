<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# 04 — Building a Language Front End

This lesson shows how to use the modular multi-language framework in
`/lib/compiler` to build a compiler for any publicly known programming language.
See [`MULTI-LANGUAGE.FRAMEWORK.md`](../MULTI-LANGUAGE.FRAMEWORK.md) for the full
reference.

## The idea

A compiler for a language is a small SLeeLa front end that extends the common
`SLLanguageCompiler` contract. Many independent front ends are registered into
one shared `SLCompilerRegistry`. Every front end lowers its own language toward
the common SLeeLa IR and onward to a VM-ready artifact:

```
<language> source  ->  front end  ->  SLeeLa IR  ->  VM lowering  ->  SLVM/SLJVM
```

Compilation is not execution: a produced artifact is only ever run later across
an explicit SLeeLa VM/OS and security boundary.

## Step 1 — declare identity and phases

Create `frontends/<language>/<Name>LanguageCompiler.sleela` and extend
`SLLanguageCompiler`. In `configure()`, declare the language identity and
`provide(...)` each pipeline phase you implement:

```
class CLanguageCompiler extends SLLanguageCompiler {
  void configure() {
    declare("C", "C-family", "C17", ".c", TARGET_SLVM);
    provide(PHASE_SOURCE); provide(PHASE_LEX); provide(PHASE_PARSE);
    provide(PHASE_RESOLVE); provide(PHASE_SEMANTIC); provide(PHASE_IR);
    provide(PHASE_LOWER); provide(PHASE_CODEGEN);
  }
}
```

## Step 2 — implement the phase hooks

Override only the phases you provide. The base returns whether the phase is
declared; your override drives the language-specific work (e.g. Python's
indentation-significant lexing, Rust's borrow checking in the semantic phase).

## Step 3 — register and plan

```
SLCompilerRegistry registry = new SLCompilerRegistry();
registry.add(new CLanguageCompiler());

SLCompileRequest request = new SLCompileRequest();
request.configure("C", "util.c", SLLanguageCompiler.TARGET_SLVM);

SLCompilePlan plan = new CLanguageCompiler().planCompile(request);
boolean ready = plan.isVMReady();
```

The plan is **fail-closed**: it reports the first missing phase and never
guesses. A front end that stops at IR can still reach the `SLIR` target but is
not VM-ready for `SLVM`; a `strict` request fails closed when any required phase
is absent; a reference-only module is never asked to emit.

## Targets

| Target | Use |
|---|---|
| `SLVM`   | native SLeeLa VM artifact |
| `SLJVM`  | JVM-family languages (e.g. Java) |
| `SLIR`   | stop at SLeeLa IR for analysis/library work |
| `NATIVE` | native plan only; emission gated by the VM/OS boundary |

## Example and self-test

- Runnable example: [`examples/multi-language-registry.sleela`](examples/multi-language-registry.sleela)
- Native behavioral self-test: `make -C lib/compiler selftest`

**SLeeLa — MEARVK LLC — 2026**
