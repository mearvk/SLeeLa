# SLeeLa Object Creation

## Supported source forms

SLeeLa defines two source forms for creating an instance:

```sleela
let first = new Widget(config);
let second = Widget(config);
```

The explicit `new` form communicates object creation directly. The concise form is allowed only when semantic resolution identifies `Widget` as a constructible type and resolves the arguments to a valid constructor. It is not a blanket reinterpretation of every function call.

## Resolution rules

1. Parse both forms into a common object-creation candidate node while retaining the original source form for diagnostics and tooling.
2. Resolve the name in the current lexical scope and imported namespaces.
3. If the name denotes a type with accessible constructors, resolve the constructor overload by argument count, types, visibility, and the active language's conversion rules.
4. If the concise form resolves only to a function, preserve ordinary function-call semantics.
5. If the concise form is ambiguous between a callable function and a constructible type, emit an ambiguity diagnostic and ask for `new` or qualification.
6. If no valid constructor exists, emit a source-located error; do not fall back to an unrelated function.
7. Both valid object-creation forms lower to the same IR operation and obey the same access, allocation, initialization, exception, and resource rules.

## Style guidance

Use `new Type(...)` when explicitness improves clarity or resolves ambiguity. Use `Type(...)` for concise construction when the type is unambiguous. The compiler accepts the concise form because its meaning is established by semantic analysis—not by guessing from capitalization or naming style.

## Safety and correctness

Construction must respect type visibility, allocation limits, capability checks, constructor chaining, and failure cleanup. Partially initialized objects must not escape. Compiler diagnostics should identify the selected constructor or explain why resolution failed.