# LLM Training Examples

This directory contains small, human-readable examples and JSONL instruction records.

## Curated character and capability set

- `character-capability-progressive.jsonl` contains 39 original prompt/completion records.
- Thirteen themes are represented: apprehension, learning, favor, guild, manner, intelligence, design, conversation, service, delitter, savings, skill, and education.
- Each theme has low, medium, and high importance examples. The tiers indicate increasing consequence, complexity, and reasoning burden; they are not numeric loss weights or claims about a person's worth.

The file preserves the existing two-field JSONL interchange shape:

```json
{"prompt":"[domain:learning] [importance:low] ...","completion":"..."}
```

The records are synthetic, original teaching examples created for this repository. They are not sourced quotations, verified factual reference material, or a trained model. Review and curate them for the intended application before use. Keep provenance and rights metadata when adding external material.

## Suggested use

1. Validate every JSONL line and enforce a maximum record size.
2. Split by theme and importance tier when evaluating generalization; avoid placing near-duplicates across train and evaluation splits.
3. Use the exact tokenizer and model vocabulary required by the selected backend.
4. Balance or sample tiers intentionally; the labels are metadata in the prompt, not automatic training weights.
5. Evaluate for accuracy, respect, autonomy, fairness, privacy, and transfer to unseen scenarios.

The companion `../distinguish/` directory defines an ordered novice curriculum that composes these themes into a single behavioral progression. It is a curriculum specification, not a model checkpoint or proof that training has occurred.
