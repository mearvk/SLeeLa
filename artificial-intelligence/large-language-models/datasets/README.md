# Datasets

The initial interchange format is UTF-8 JSON Lines (JSONL): one JSON object per non-empty line. Use `sllmd_jsonl_read` from `../large_language_models_data.h` to stream records from a connector without loading the entire dataset into memory.

The reader delivers raw record bytes; it does not parse JSON, infer labels, tokenize text, or decide training targets. The application validates each JSON object and its schema, checks provenance and permissions, extracts the text fields, then passes text through the model-compatible tokenizer. Keep records small enough for the configured maximum record size.

A minimal supervised record may contain `{"prompt":"...","completion":"..."}`. Chat records may contain a `messages` array with role/content pairs. Define and version your schema before training. See `examples/sample-instruction.jsonl`.
