# SLeeLa Regex API — Simple Pattern Matching

**Draft design — Regex API 1.0.0**  
**Main program:** Sleelvac 1.5  
**SLeeLa syntax:** 1.3

## Purpose

SLeeLa Regex is designed around a simple promise: asking for a text match should be easier than explaining the machinery behind it. The API gives ordinary text matching a small vocabulary first, then adds expressive power in four deliberately incremental levels.

The design does not discard established regular-expression mathematics. It places a simpler SLeeLa language in front of it and keeps the implementation responsible for the formal details. This reduces the amount of syntax a learner must memorize while preserving deterministic, testable matching semantics.

Unicode is a first-class requirement. Unicode Technical Standard #18 describes progressively stronger levels of Unicode regular-expression support, including code-point notation and character properties. SLeeLa therefore treats Unicode behavior as part of the API contract rather than as an afterthought. citeturn0search9

## The four levels

| Level | Name | Intended question | Core vocabulary |
|---|---|---|---|
| 1 | **Find** | Does this text contain this? | literal text, contains, starts, ends |
| 2 | **Shape** | Does this text have this simple shape? | word, digit, space, any, repeat |
| 3 | **Structure** | Does this text have these alternatives or groups? | choice, group, range, optional, capture |
| 4 | **Expression** | Can I describe a complicated expression precisely? | anchors, properties, bounded repetition, named capture, advanced assertions |

A program may stay at Level 1 forever. Higher levels are additions, not prerequisites.

## Simple syntax

SLeeLa favors readable pattern words over punctuation-heavy expressions:

    #sleela 1.3
    class SimpleMatch {
        void main() {
            print(match("hello world", "contains hello"));
            print(match("12345", "digits+"));
            print(match("hello", "starts hello"));
            print(match("report.txt", "ends .txt"));
        }
    }

The compiler/API may lower these forms to a conventional regex engine or a native deterministic matcher; the source author does not need to know which engine performs the work.

## Exemplar programs

- `LEVEL-1-FIND.sleela` — literal questions and boundaries.
- `LEVEL-2-SHAPE.sleela` — named shapes and repetition.
- `LEVEL-3-STRUCTURE.sleela` — choices, groups, ranges, optional pieces, and captures.
- `LEVEL-4-EXPRESSION.sleela` — compiled patterns, anchors, Unicode properties, and advanced expression vocabulary.

The four files are teaching exemplars and source contracts. The native C foundation currently executes the Level 1/2 subset documented in `impl/REGEX-NATIVE.md`; Level 3/4 remain explicit unsupported runtime levels until their semantics are finalized.

## Level 1 — Find

Level 1 handles everyday cases without regular-expression punctuation.

    contains hello
    starts hello
    ends .txt
    exact hello
    word hello

`contains` searches within the text; `starts` and `ends` constrain the corresponding boundary; `exact` requires the complete text to match.

## Level 2 — Shape

Level 2 introduces named character classes and repetition.

    word+
    digit+
    space+
    any*
    word{3}
    digit{2,4}

Proposed built-in classes:
- `word` — Unicode-aware word characters defined by the SLeeLa Unicode profile.
- `digit` — decimal digits defined by the SLeeLa Unicode profile.
- `space` — whitespace defined by the SLeeLa Unicode profile.
- `any` — one text element/code point under the selected Unicode mode.
- `letter`, `lower`, `upper` — Unicode letter/case concepts.

The implementation must document exactly which Unicode properties each class denotes. Unicode property support matters because character sets vary substantially across languages. citeturn0search9

## Level 3 — Structure

Level 3 introduces grouping and alternatives in readable form:

    choice(cat, dog)
    group("http", optional("s"), "://", word+)
    range(a-z)+
    optional("-", digit+)
    capture("name", word+)

The compiler can lower these structures to conventional groups, alternation, character classes, and quantifiers. Conventional engines expose similar concepts, but syntax and Unicode behavior differ; SLeeLa defines its own source contract. ECMAScript, for example, specifies patterns using alternatives, terms, atoms, quantifiers, assertions, and Unicode modes. citeturn0search0turn0search4

## Level 4 — Expression

Level 4 is for deliberately precise expressions:

    exact group("ID-", capture("number", digit{4,8}))
    start property("Letter")+ end
    choice(group("https://", word+), group("http://", word+))

Advanced facilities may include start/end assertions, Unicode properties, bounded and unbounded repetition, named captures, escaped code points, lookaround or equivalent deterministic assertions, explicit case and normalization modes, and compiled pattern objects.

Level 4 is intentionally not the teaching default.

## Matching modes

Every public operation should make its intent obvious:

- `contains` — substring search
- `starts` — prefix match
- `ends` — suffix match
- `exact` — whole-text match
- `find` — return the next matching span
- `findAll` — return all matching spans

A future API should expose a typed result:

    MatchResult
      matched
      start
      end
      text
      captures
      patternLevel

## Safety and determinism

The API should reject malformed patterns before matching. Compilation and matching should be separate operations for reusable patterns:

    pattern = regex("digit{2,4}")
    result  = pattern.exact("2026")

A production implementation should place explicit limits on pattern size, input size, capture count, and evaluation resources. This is especially important if a backtracking engine is used underneath the simple language.

SLeeLa should prefer deterministic matching engines where practical. Regex is a text operation and should not silently acquire network, filesystem, process, or code-execution behavior.

## Standard relationship

This API is SLeeLa syntax, not a claim that every existing regex standard uses the same surface notation. The underlying implementation may interoperate with established engines, but SLeeLa source semantics remain documented and versioned.

Current ECMAScript specifications define regular-expression literals, patterns, quantifiers, assertions, Unicode modes, and related semantics. SLeeLa can borrow compatible concepts while presenting a simpler source language. citeturn0search0turn0search1

## Education model

1. **Find text.** `contains`, `starts`, `ends`, `exact`.
2. **Describe a shape.** `word`, `digit`, `space`, `any`.
3. **Repeat a shape.** `+`, `*`, `{n}`, `{n,m}`.
4. **Combine shapes.** `choice`, `group`, `optional`, `capture`.
5. **Only then learn advanced expression features.**

The learner should solve useful matching problems before learning formal names such as atoms, assertions, disjunctions, backtracking, automata, or Unicode property tables.

## Newtonness / carefulness principle

SLeeLa treats a pattern as a carefully stated question, not as a puzzle in punctuation. The author says what kind of text is being requested, the runtime determines whether the text satisfies that description, and the result records what was matched.

The implementation should preserve the mathematical discipline of regular languages while making first contact with that discipline calm, readable, and incremental.

## Versioning

- Regex API: **1.0.0 draft**
- Main program: **Sleelvac 1.5**
- SLeeLa syntax: **1.3**
- Compatibility target: Unicode-aware, deterministic pattern matching