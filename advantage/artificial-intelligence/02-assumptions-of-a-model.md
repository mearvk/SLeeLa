# Lesson 02 — Assumptions of a model

> Series: [index](README.md) · prev: [01](01-evidence-before-trust.md) · next: [03 — Counsel](03-counsel-choosing-an-approach.md)

Every AI component rests on assumptions. The knowledge store records each as
**held** (default) or `status="doubted"`. `trust = held / total`, so doubts
discount the score until verified.

| Component | Example assumption | Where to look |
|---|---|---|
| Neural nets | Inputs are scaled the way training expected | [neural-nets](../../artificial-intelligence/neural-nets/README.md) |
| Character recognition | Glyphs are clean; scripts are European | [character-recognition](../../artificial-intelligence/character-recognition/README.md) |
| Large language models | Training data and model licence permit your use | [large-language-models](../../artificial-intelligence/large-language-models/) |

```xml
<assumptions>
  <assumption>Input scaling matches training.</assumption>
  <assumption status="doubted">Test data resembles production data.</assumption>
</assumptions>
```

The loader fails closed on any status other than `held` or `doubted`.

## Exercise

Write four assumptions for an LLM connector, mark one doubted, and compute trust.
*Answer:* 3 held of 4 gives 0.75.
