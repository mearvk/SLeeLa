# Vocabulary assets

The public vocabulary implementation remains in the parent directory as `large_language_models_vocabulary.h/.c` and its C++17 wrapper. This folder is the home for model-specific vocabulary exports and metadata when those assets are added.

A vocabulary asset must preserve token-to-ID assignments exactly and identify its tokenizer algorithm/version, special tokens, normalization behavior, and model compatibility. Do not treat the current whitespace encoder as a drop-in tokenizer for arbitrary pretrained models.
