# Tokenizers

A model's tokenizer is part of its model contract. The current `LanguageModelVocabulary` API is an exact-token vocabulary primitive with whitespace splitting; it is not a production subword tokenizer.

Store tokenizer metadata and assets alongside each model, including algorithm and version, normalization rules, special-token definitions, vocabulary IDs, merge/model files, and any byte-fallback rules. A training backend must reject or explicitly migrate mismatched tokenizer/vocabulary versions rather than silently accepting incompatible token IDs.

Future tokenizer implementations should expose a common text-to-token-ID interface and preserve the existing vocabulary API for simple/custom token sets.
