# SLeeLa Case Handler

## Purpose

Define consistent handling for the SLeeLa source keyword `case` and the `case` branch of compiler parsing. This handler is distinct from the `new` keyword used for explicit object creation.

## Case-statement contract

The parser recognizes case labels only inside a valid selection construct, such as a supported `switch` or SLeeLa selection form. Each label is parsed as an expression or a permitted constant pattern according to the active language profile.

- A `case` label must belong to an enclosing selection construct.
- Duplicate constant labels are a compile-time diagnostic.
- `default` (if enabled by the language profile) may occur at most once.
- Fall-through is never implicit unless the active language specification explicitly enables it; otherwise each branch terminates or exits by the defined control-flow rules.
- Case expressions are type-checked against the selector before lowering.
- Invalid or unsupported patterns produce a diagnostic with source location; they must not be silently ignored.

## Object creation is a separate concern

The parser and semantic analyzer must not confuse a `case` label with the `new` keyword. Object creation is governed by `OBJECT-CREATION.md` and `CONSTRUCTOR-SERIES.md`:

- Explicit form: `new Type(arguments)`
- Concise form: `Type(arguments)`, permitted only when symbol resolution proves that `Type` denotes a constructible type in that context.
- Ordinary function calls remain function calls; unresolved or ambiguous forms are diagnosed rather than guessed.

## Pipeline

`source -> lexer -> parser -> case AST -> semantic validation -> control-flow IR -> VM lowering`

Object creation follows its own path:

`source -> call/object AST -> symbol resolution -> constructor selection -> initialization chain -> IR`

This separation keeps parsing predictable and makes the language rules testable.