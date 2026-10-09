# SLeeLa Constructor Series and Joining Structure

## Purpose

Provide one authoritative model for constructor selection, constructor chaining, instance initialization, and allocation. The series joins constructor declarations into a validated initialization plan before IR lowering.

## Joining sequence

`type lookup -> constructor set -> overload resolution -> access/capability check -> allocation plan -> base/delegating constructor chain -> field initializers -> constructor body -> publish initialized instance`

1. **Type lookup:** resolve the declared type and its namespace.
2. **Constructor set:** collect constructors visible for the selected type and language profile.
3. **Overload resolution:** choose one best match using declared parameter types and language conversion rules. Ambiguity is an error.
4. **Validation:** check visibility, capabilities, argument compatibility, and inheritance/base initialization rules.
5. **Chain planning:** represent `this(...)` delegation and base-constructor calls as explicit edges.
6. **Cycle detection:** reject constructor-delegation cycles.
7. **Initialization order:** execute base initialization, instance field initializers, and the selected constructor body in the order defined by the language specification.
8. **Commit object state:** publish the instance only after successful initialization; clean up allocation and owned resources on failure.

## Invariants

- Exactly one constructor path initializes each required portion of the object.
- Delegation cycles are rejected before execution.
- No ambiguous overload is selected by arbitrary ordering.
- `new Type(args)` and resolved `Type(args)` use the same constructor plan.
- A failed constructor cannot expose a partially initialized instance.
- Constructor metadata records source location, parameter signature, visibility, and delegation target.

## Intermediate representation

Represent the series as a directed graph with a single selected entry constructor. Each edge identifies a delegation or base-construction step. The graph must be acyclic; the lowering stage emits a deterministic initialization sequence from the validated graph.

## Diagnostics

Report distinct errors for missing constructors, ambiguous overloads, inaccessible constructors, incompatible arguments, invalid base calls, delegation cycles, capability denial, allocation failure, and initialization failure.