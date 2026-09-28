# SLeeLa Regex API Design Notes

**Draft review document — Regex API 1.0.0**

## Design goal

Make a text match feel like a carefully worded question rather than a memorization exercise.

## Four-level progression

**1 — Find:** literal matching and position: contains, starts, ends, exact.

**2 — Shape:** named character concepts and repetition: word, digit, space, any, plus, star, bounded counts.

**3 — Structure:** composition: choice, group, range, optional, capture.

**4 — Expression:** precision features: anchors, Unicode properties, named captures, explicit modes, and advanced assertions.

## Proposed source style

Prefer:

    exact digit{4}
    contains "error"
    choice(cat, dog)
    capture("account", digit{6})

over forcing beginners to start with punctuation such as `^\\d{4}$`.

## Compatibility principle

The implementation can lower the simple SLeeLa pattern language to an existing regex engine, but the SLeeLa source language remains the stable contract. ECMAScript's current regular-expression grammar demonstrates the breadth of conventional concepts that can sit underneath a higher-level interface. citeturn0search0turn0search4

Unicode support should follow a documented profile. Unicode Technical Standard #18 describes Unicode regex levels, code-point notation, character properties, and interoperability concerns. citeturn0search9

## Review questions

- Should Level 1 use only literal words, or also allow quoted strings?
- Should `word`, `digit`, and `space` be Unicode-property based by default?
- Should Level 3 use function-like construction exclusively, or permit a compact infix form?
- Which advanced assertions belong in Level 4?
- What resource limits should compiled patterns and matches have?
- Should a pattern expose its lowered conventional-regex form for interoperability?

## Status

This is a design draft. It establishes vocabulary and teaching direction before native implementation is committed.