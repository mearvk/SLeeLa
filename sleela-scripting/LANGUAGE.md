# Sleela Script Language

## Version

Initial language contract: **SLEELA-SCRIPT 1.0**.

## Source

The canonical source extension is `.sleela-script`.

A script begins with:

```
script "name"
```

The name is optional for anonymous interactive input but recommended for
stored scripts.

## Values

The core value types are:

- null;
- boolean;
- integer;
- floating-point number;
- string;
- list;
- map;
- function;
- host handle.

Lists and maps are mutable values. Host handles are opaque and cannot be
constructed directly by script code.

## Variables

```
let name = "Max"
const version = 1
name = "SLeeLa"
```

`let` creates a mutable binding. `const` creates a binding that cannot be
reassigned.

## Operators

Arithmetic:

`+`, `-`, `*`, `/`, `%`

Comparison:

`==`, `!=`, `<`, `<=`, `>`, `>=`

Boolean:

`and`, `or`, `not`

Assignment:

`=`

String concatenation uses `+`.

## Control flow

```
if condition {
    ...
} else {
    ...
}

while condition {
    ...
}

for value in values {
    ...
}
```

`break` exits the nearest loop. `continue` advances to the next iteration.

## Functions

```
fn greet(name) {
    return "Hello, " + name
}
```

Functions are first-class values and use lexical scope.

## Errors

Errors are explicit runtime values internally and become script failures at the
top level unless handled by the host.

A future `try/catch` facility is reserved by the grammar but is not required
for 1.0 conformance.

## Modules

Version 1.0 reserves:

```
import "module"
```

Modules are resolved through the SLeeLa configuration root and the script
module search path. Arbitrary filesystem traversal is not implied.

## Execution modes

A conforming host may provide:

- file execution;
- standard-input execution;
- interactive REPL;
- embedded execution.

The language semantics are identical across modes.

## Determinism

The core language itself has no implicit network, process, clock, or filesystem
side effects. Such operations enter through named host capabilities.
