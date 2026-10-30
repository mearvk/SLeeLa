# Models

Keep model configuration and weights separate from datasets and tokenizer assets. A model package should identify its architecture/version, vocabulary size, context length, layer and attention dimensions, positional encoding, normalization, activation, numeric precision, and weight format.

Suggested package layout:

```text
model-name/
  config.json
  tokenizer/
  weights/
  model-card.md
  checksums.sha256
```

The current repository APIs define provider-neutral inference and training backend contracts; they do not yet implement a transformer architecture or load model weights. Validate configuration dimensions, file sizes, checksums, and tokenizer compatibility before allocating accelerator or host memory.
