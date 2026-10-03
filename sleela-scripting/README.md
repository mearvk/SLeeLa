<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Scripting

SLeeLa Scripting is the scripting-language layer for SLeeLa. It provides a
small, embeddable, dynamically executable language for automation, build
orchestration, configuration tasks, VM control, and application glue while
remaining able to call explicitly exposed SLeeLa objects and native APIs.

The scripting language is **Sleela Script** and uses the `.sleela-script`
source extension. It is distinct from a normal `.sleela` Wrapper: a Wrapper
is the general SLeeLa program/source unit, while a Sleela Script is optimized
for short procedural automation and interactive execution.

## Goals

- readable procedural scripting;
- variables, expressions, conditionals, loops, functions, and collections;
- explicit process/file/environment operations through an allow-listed host API;
- access to SLeeLa objects and selected runtime services;
- VM 1–11 awareness without hard-coding a particular VM;
- configuration through the unified SLeeLa configuration root;
- embeddability in the C/C++ runtime;
- deterministic behavior in restricted/CI mode;
- clear separation between language semantics and native capabilities.

## Minimal example

```sleela-script
script "hello"

let name = "SLeeLa"
print("Hello, " + name)

for item in ["compiler", "vm", "gc"] {
    print("checking " + item)
}
```

## Core syntax

```text
script "name"

let count = 3
const answer = 42

if count > 0 {
    print("active")
} else {
    print("inactive")
}

while count > 0 {
    count = count - 1
}

fn add(a, b) {
    return a + b
}
```

## Host capabilities

Scripts do not receive unrestricted native execution merely because they can
call a host function. The host exposes named capabilities such as:

- `print`
- `env.get`
- `fs.read`
- `fs.write`
- `process.run` (policy-controlled)
- `vm.select`
- `config.get`
- `sleela.object`

Each capability can be enabled or denied by the embedding application.

## Layout

- `LANGUAGE.md` — language definition and semantics.
- `GRAMMAR.md` — lexical and grammar contract.
- `RUNTIME.md` — interpreter/VM execution model.
- `HOST-API.md` — native capability boundary.
- `SECURITY.md` — sandbox and permission model.
- `CONFIGURATION.md` — integration with unified SLeeLa configuration.
- `examples/` — executable-oriented examples.
- `tests/` — language conformance cases.
- `include/` — planned C embedding interface.
- `src/` — scripting implementation boundary.

## Relationship to `.sleela`

Sleela Script is not intended to replace the full SLeeLa language. A script
may launch, inspect, configure, or generate ordinary SLeeLa programs, and the
host can expose selected SLeeLa objects to scripts.

The long-term execution path is:

**Sleela Script → Script Front End → Script Runtime → SLeeLa Host API → SLeeLa Runtime / VM**

The scripting runtime must use the same unified configuration resolver as the
rest of SLeeLa.

## Turing 5 profile

The current advanced scripting profile is documented in [TURING-5.md](TURING-5.md). It adds a Turing-complete temporary-work model with first-class scientific and engineering computation, calculus, definitions, lookups, typed object-state inspection, known-variable access, controlled object updates, careful result queues, VM condition channels, master sequence indexes/IDs, pause/yield controls, and finite timeouts from seconds through days.

Supporting contracts: [SCIENCE.md](SCIENCE.md), [OBJECT-STATE.md](OBJECT-STATE.md), [CONTROL.md](CONTROL.md), [TIMEOUTS.md](TIMEOUTS.md), [TYPE-SYSTEM.md](TYPE-SYSTEM.md), and [INTEGRATION.md](INTEGRATION.md).


## Demesresmes™ scientific scripting language

The SLeeLa scripting language is named **Demesresmes™**. The ™ mark is used in the language name and documentation; source identifiers remain ASCII-friendly as `Demesresmes`.

Demesresmes™ includes a versioned scientific constants registry under [constants/](constants/). It provides explicit mathematical, physics, chemistry, and engineering constants in both JSON and XML forms. See [CONSTANTS.md](CONSTANTS.md) and [CONSTANTS.SCHEMA.md](CONSTANTS.SCHEMA.md).

Example:

```sleela-script
let c = constant.get("physics.speed_of_light")
let pi = constant.get("math.pi")
let R = constant.get("chemistry.molar_gas")
let G = constant.get("physics.newtonian_gravitational_constant")
```

Canonical names are preferred over short aliases so symbols with multiple scientific meanings remain unambiguous. Registry values retain units, exactness/uncertainty, and provenance metadata.