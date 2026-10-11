# Extended `for` Loops: Thread and VM-Heap Health

Status: parser, AST, semantic Boolean checks, and bytecode lowering are implemented on this feature branch. Runtime health signals remain supplied by the caller/VM host; the compiler does not fabricate thread or heap state. Regression builds and end-to-end tests are still required before merge.

## Compatibility

The existing three-term form remains unchanged:

```sleela
for (let i = 0; i < limit; i = i + 1) {
    work(i);
}
```

The extended forms append one or two Boolean expressions, separated by semicolons:

```sleela
// Four terms: standard iterator + thread-loop stability signal.
for (let i = 0; i < limit; i = i + 1; threadStable) {
    work(i);
}

// Five terms: thread stability + VM GC/heap health.
for (let i = 0; i < limit; i = i + 1; threadStable; heapHealthy) {
    work(i);
}
```

Grammar sketch:

```ebnf
forStatement = "for", "(", init, ";", condition, ";", update,
               [ ";", threadStable, [ ";", heapHealthy ] ],
               ")", statement ;
threadStable = expression ;  (* must have Boolean type *)
heapHealthy  = expression ;  (* must have Boolean type *)
```

The first three terms retain their current meaning. The fourth and fifth terms are Boolean guard expressions evaluated before each body execution, after the loop initializer and before entering the body. A false health guard exits the loop normally unless an explicit runtime policy says otherwise. Health probes must not be evaluated more than once per iteration.

## Runtime contract

- `threadStable` is a caller-visible Boolean signal supplied by a real thread/runtime monitor. It represents whether the nearest relevant thread loop's memory signal is stable. The compiler must not infer stability merely because a variable is named `threadStable`.
- `heapHealthy` is a caller-visible Boolean signal supplied by the VM memory/GC subsystem. False means the VM's configured policy should stop the loop and either set a diagnostic flag or raise a documented memory-health exception.
- The exact exception type and flag representation must be defined by the VM/runtime ABI before code generation is enabled. A compiler must not pretend it can inspect GC state unless the VM exposes that probe.
- The two guards are optional and ordered: the five-term form includes both; the four-term form includes only thread stability. The legacy three-term form has no new guard behavior.
- Both guards must be statically type-checked as Boolean. Invalid arity, non-Boolean guards, or unsupported syntax-version use must produce clear diagnostics.
- The implementation must preserve `break`, `continue`, loop-scope cleanup, and existing evaluation order. In particular, `continue` must still execute the update expression according to current loop semantics.

## Required implementation checklist

1. Lexer: no new reserved word is required if guards are ordinary expressions; preserve semicolon tokenization.
2. Parser/AST: accept optional fourth and fifth terms and store them distinctly from the standard condition/update.
3. Semantic analysis: require Boolean types for both guards and enforce the selected syntax-version gate.
4. Code generation / VM: evaluate guards once at the top of each iteration; connect heap health to a documented runtime probe and flag/exception policy.
5. Tests: preserve three-term behavior and cover four-/five-term parsing, Boolean type errors, false guards, exception/flag behavior, `break`, `continue`, and version rejection.
6. Documentation: update `SLEELA.syntax`, the syntax version table, and compiler/VM compatibility notes only when the end-to-end implementation is available.

## Suggested regression cases

- Three-term loop remains byte-for-byte behaviorally compatible.
- Four-term loop exits before the body when `threadStable` is false.
- Five-term loop exits or raises the configured health exception when `heapHealthy` is false.
- Both guards true permit the body to run.
- Non-Boolean fourth/fifth terms fail semantic analysis.
- Guards are evaluated once per iteration, in thread-then-heap order.
