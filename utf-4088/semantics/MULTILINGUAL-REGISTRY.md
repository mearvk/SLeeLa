# Multilingual concept registry

Status: annotation contract, UTF4088-MULTILINGUAL-1.

The 44 C++ concept seeds are language-neutral concept candidates with English
working labels and definitions. They are not translations, and they do not
establish equivalence across English, Korean, and Germanic languages.

Use `multilingual-concept-registry.schema.json` for reviewed annotations.
Each concept may have one mapping per language. Record the intended sense,
dialect/variety where relevant, source URL or bibliographic reference, reviewer,
and status. Do not promote an entry to `reviewed` or `validated` without
documented human review and traceable evidence.

Recommended workflow:

1. Keep the concept ID and key stable.
2. Have a competent speaker/reviewer propose the target-language label.
3. Record sense boundaries and usage notes; do not assume word-for-word
   equivalence.
4. Cite a dictionary, corpus, terminology authority, or primary source and
   record its version/access date.
5. Have a second reviewer check high-impact or ambiguous terms.
6. Preserve the original annotation and record corrections as versioned changes.

The schema intentionally does not seed fabricated Korean or German translations.
The label "Germanic" is a family-level project bucket, not a single language.
A concrete mapping must identify the actual language variety (for example,
German, Dutch, or another named variety) in its annotation metadata or notes.

Glyph geometry, four-dimensional anchors, and graph proximity are not evidence
that two language expressions share a meaning.
